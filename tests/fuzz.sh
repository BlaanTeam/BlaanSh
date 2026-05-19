#!/usr/bin/env bash
# Fuzz the parser by piping randomized input from /dev/urandom into the
# binary. A crash is any exit with a fatal signal (SEGV/ABRT/BUS/ILL).
# Sandboxed via env -i with PATH=/nonexistent so commands can't resolve.

set -u

ITERATIONS="${ITERATIONS:-200}"
LINES_PER_RUN="${LINES_PER_RUN:-40}"
LINE_LEN="${LINE_LEN:-80}"
TIMEOUT_S="${TIMEOUT_S:-5}"

# Resolve the binary path to an absolute path before we cd into the sandbox.
MS_RAW="${MS:-./minishell}"
case "$MS_RAW" in
	/*)	MS_PATH="$MS_RAW" ;;
	*)	MS_PATH="$(pwd)/$MS_RAW" ;;
esac
if [ ! -x "$MS_PATH" ]; then
	printf 'error: %s not found or not executable\n' "$MS_PATH" >&2
	exit 1
fi

SANDBOX="$(mktemp -d -t blaansh-fuzz.XXXXXX)"
cleanup_sandbox() { rm -rf "$SANDBOX"; }
trap cleanup_sandbox EXIT

cd "$SANDBOX"

# Alphabet weighted toward shell metacharacters. tr treats `-` as a range
# marker — keep it at the end of the set so it stays literal.
ALPHABET='a-zA-Z0-9 |&<>;()$*?~=:./_"[]{}'
ALPHABET="${ALPHABET}$(printf '\t')"
ALPHABET="${ALPHABET}\\"
ALPHABET="${ALPHABET}'"
ALPHABET="${ALPHABET}-"

gen_input() {
	local lines=$1
	local maxlen=$2
	LC_ALL=C tr -dc "$ALPHABET" < /dev/urandom \
		| fold -w "$maxlen" \
		| head -n "$lines"
	printf 'exit\n'
}

# Portable timeout: spawn the binary, race against a sleep-then-kill watcher.
# Returns the binary's wait status (128+signo if killed).
run_with_timeout() {
	local secs=$1
	local input=$2
	(
		env -i PATH=/nonexistent HOME="$SANDBOX" TERM=dumb \
			"$MS_PATH" < "$input" > /dev/null 2>&1 &
		local pid=$!
		(
			sleep "$secs"
			kill -TERM "$pid" 2>/dev/null
			sleep 0.5
			kill -KILL "$pid" 2>/dev/null
		) >/dev/null 2>&1 &
		local watcher=$!
		wait "$pid" 2>/dev/null
		local rc=$?
		kill "$watcher" 2>/dev/null
		wait "$watcher" 2>/dev/null
		exit "$rc"
	)
}

TOTAL=0
CRASHES=0
TIMEOUTS=0
declare -a KEPT_INPUTS=()

printf 'Fuzzing %s — %d iterations, %d lines each (timeout %ds)\n' \
	"$MS_PATH" "$ITERATIONS" "$LINES_PER_RUN" "$TIMEOUT_S"

for i in $(seq 1 "$ITERATIONS"); do
	INPUT="$SANDBOX/inp_$i.txt"
	gen_input "$LINES_PER_RUN" "$LINE_LEN" > "$INPUT"

	run_with_timeout "$TIMEOUT_S" "$INPUT"
	rc=$?
	TOTAL=$((TOTAL + 1))

	case $rc in
		139|134|138|132)
			# 139 SIGSEGV, 134 SIGABRT, 138 SIGBUS, 132 SIGILL
			CRASHES=$((CRASHES + 1))
			KEPT_INPUTS+=("$INPUT")
			printf '\n[CRASH] iter=%d rc=%d (sig %d) — input preserved at %s\n' \
				"$i" "$rc" "$((rc - 128))" "$INPUT"
			printf -- '----- input -----\n'
			cat "$INPUT"
			printf -- '\n-----------------\n'
			;;
		137|143)
			# Killed by our watcher (SIGKILL / SIGTERM)
			TIMEOUTS=$((TIMEOUTS + 1))
			;;
	esac

	# Don't accumulate input files we no longer need
	if [ "$rc" -ne 139 ] && [ "$rc" -ne 134 ] && [ "$rc" -ne 138 ] && [ "$rc" -ne 132 ]; then
		rm -f "$INPUT"
	fi

	if [ $((i % 25)) -eq 0 ]; then
		printf '  ... %d/%d  (crashes=%d, timeouts=%d)\n' \
			"$i" "$ITERATIONS" "$CRASHES" "$TIMEOUTS"
	fi
done

printf '\nFuzz summary: %d runs, %d crashes, %d timeouts\n' \
	"$TOTAL" "$CRASHES" "$TIMEOUTS"

if [ "$CRASHES" -gt 0 ]; then
	# Keep sandbox so crash inputs can be inspected.
	trap - EXIT
	printf 'Crash inputs preserved in: %s\n' "$SANDBOX"
	exit 1
fi
