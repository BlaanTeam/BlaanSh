#!/usr/bin/env bash
# Runs a stress scenario through the shell and asserts no memory leaks.
#
# - On Linux: valgrind --leak-check=full. Errors only on definite leaks
#   (readline leaves "still reachable" allocations we don't want to
#   chase) and exits 42 if any are found.
# - On macOS: `leaks` attached to a live process while the shell is
#   sleeping between fed commands.
# - Otherwise: skipped (returns 0 with a note).

set -u

MS="${MS:-./minishell}"
if [ ! -x "$MS" ]; then
	printf 'error: %s not found or not executable\n' "$MS" >&2
	exit 1
fi

WORKDIR=$(mktemp -d -t blaansh-leak.XXXXXX)
trap 'rm -rf "$WORKDIR"' EXIT

# A scenario that exercises the bulk of the allocator surface: builtins,
# pipes, redirections including heredocs, subshells, conditionals,
# wildcards, env mutation across commands.
make_stress_input() {
	local i
	# env mutation, repeated to grow the perm arena
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15; do
		echo "export VAR$i=value$i"
		echo "echo \$VAR$i"
	done
	for i in 1 2 3 4 5 6 7; do
		echo "unset VAR$i"
	done
	# overwrite-and-append a var several times — perm arena should keep
	# growing without trouble
	echo "export GROW=base"
	for i in 1 2 3 4 5 6 7 8 9 10; do
		echo "export GROW+=,$i"
	done
	echo "echo \$GROW"
	# pipelines of various depths
	echo "echo a | cat"
	echo "echo a | cat | cat | cat"
	echo "echo a | cat | cat | cat | cat | cat | cat | wc -l"
	# subshells, nested, redirected
	echo "(echo nested; echo deep) | cat"
	echo "((echo doublenest))"
	echo "(echo redir-subshell) > $WORKDIR/sub.txt"
	echo "cat $WORKDIR/sub.txt"
	# conditional combinations
	echo "true && echo yes || echo no"
	echo "false || echo recovered"
	echo "false && (true || echo never) || echo good"
	# redirections including heredoc
	echo "echo redir1 > $WORKDIR/file1"
	echo "echo redir2 >> $WORKDIR/file1"
	echo "cat $WORKDIR/file1"
	echo "cat < $WORKDIR/file1"
	echo "cat << END"
	echo "heredoc line 1"
	echo "heredoc line 2"
	echo "with \$VAR8 expanded"
	echo "END"
	# wildcards
	echo "cd $WORKDIR && touch a.txt b.txt c.txt"
	echo "echo *.txt"
	# tilde
	echo "echo ~"
	# command-not-found path (exercises the find_path / strtok cleanup)
	echo "doesnotexist_zzz_xyz"
	echo "echo \$?"
	# quoting variations
	echo "echo \"abc\"'def'ghi"
	echo "echo \"\$GROW\""
	# many short commands to repeatedly reset the tmp arena
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
		echo "echo iter$i"
	done
	echo "exit"
}

# ---------- Linux: valgrind ----------
if command -v valgrind >/dev/null 2>&1; then
	make_stress_input > "$WORKDIR/input"
	VALGRIND_LOG="$WORKDIR/valgrind.log"
	echo "Running under valgrind..."
	valgrind \
		--leak-check=full \
		--errors-for-leak-kinds=definite \
		--error-exitcode=42 \
		--log-file="$VALGRIND_LOG" \
		"$MS" < "$WORKDIR/input" > /dev/null 2>&1
	rc=$?
	# Show the leak summary regardless of outcome
	grep -E "definitely lost|indirectly lost|ERROR SUMMARY" "$VALGRIND_LOG" || true
	if [ "$rc" -eq 42 ]; then
		echo
		echo "DEFINITE LEAKS DETECTED — full log below:"
		echo
		cat "$VALGRIND_LOG"
		exit 1
	fi
	echo "no definite leaks"
	exit 0
fi

# ---------- macOS: leaks attach to live process ----------
if command -v leaks >/dev/null 2>&1; then
	# Feed slowly enough that `leaks` has time to attach mid-run, and
	# end with a long-ish sleep so the process is still alive when we
	# sample it.
	echo "Running under leaks (live attach)..."
	{
		make_stress_input | while IFS= read -r line; do
			printf '%s\n' "$line"
			sleep 0.05
		done
		sleep 5
	} | "$MS" > /dev/null 2>&1 &
	MS_PID=$!
	# Wait for the shell to be mid-scenario
	sleep 2
	LEAK_REPORT="$WORKDIR/leaks.txt"
	leaks "$MS_PID" > "$LEAK_REPORT" 2>&1 || true
	wait "$MS_PID" 2>/dev/null
	if grep -q "0 leaks for 0 total leaked bytes" "$LEAK_REPORT"; then
		grep -E "leaks for|Process [0-9]+:" "$LEAK_REPORT" | head -3
		exit 0
	fi
	echo
	echo "LEAKS DETECTED:"
	cat "$LEAK_REPORT"
	exit 1
fi

echo "warn: no leak detector available (need valgrind or leaks); skipping"
exit 0
