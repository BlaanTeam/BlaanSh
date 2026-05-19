#!/usr/bin/env bash
# Black-box test runner for minishell.
# Feeds commands via stdin, filters the prompt lines, and compares output.

set -u

MS="${MS:-./minishell}"

if [ ! -x "$MS" ]; then
	printf 'error: %s not found or not executable\n' "$MS" >&2
	exit 1
fi

TESTDIR="$(mktemp -d -t blaansh.XXXXXX)"
trap 'rm -rf "$TESTDIR"' EXIT

PROMPT_RE='^minishell-1\.0\$'

PASS=0
FAIL=0
TOTAL=0
FAILED_NAMES=()

if [ -t 1 ]; then
	C_GREEN=$'\033[32m'
	C_RED=$'\033[31m'
	C_OFF=$'\033[0m'
else
	C_GREEN=''
	C_RED=''
	C_OFF=''
fi

# Run minishell on the given input, append `exit`, strip prompt lines.
run_ms() {
	local input="$1"
	{ printf '%s\n' "$input"; printf 'exit\n'; } \
		| "$MS" 2>&1 \
		| grep -vE "$PROMPT_RE" || true
}

# Run minishell, propagate the last command's exit code via `exit $?`.
run_ms_exit_code() {
	local input="$1"
	{ printf '%s\n' "$input"; printf 'exit $?\n'; } \
		| "$MS" > /dev/null 2>&1
	echo $?
}

test_output() {
	local name="$1" input="$2" expected="$3"
	TOTAL=$((TOTAL + 1))
	local actual
	actual=$(run_ms "$input")
	if [ "$actual" = "$expected" ]; then
		PASS=$((PASS + 1))
		printf '%s[PASS]%s %s\n' "$C_GREEN" "$C_OFF" "$name"
	else
		FAIL=$((FAIL + 1))
		FAILED_NAMES+=("$name")
		printf '%s[FAIL]%s %s\n' "$C_RED" "$C_OFF" "$name"
		printf '       input:    %s\n' "${input//$'\n'/ \\n }"
		printf '       expected: %s\n' "${expected//$'\n'/ \\n }"
		printf '       actual:   %s\n' "${actual//$'\n'/ \\n }"
	fi
}

test_exit() {
	local name="$1" input="$2" expected="$3"
	TOTAL=$((TOTAL + 1))
	local actual
	actual=$(run_ms_exit_code "$input")
	if [ "$actual" = "$expected" ]; then
		PASS=$((PASS + 1))
		printf '%s[PASS]%s %s\n' "$C_GREEN" "$C_OFF" "$name"
	else
		FAIL=$((FAIL + 1))
		FAILED_NAMES+=("$name")
		printf '%s[FAIL]%s %s (expected exit %s, got %s)\n' \
			"$C_RED" "$C_OFF" "$name" "$expected" "$actual"
	fi
}

# Asserts the raw (unfiltered) output contains the given substring.
# Useful for behavior that vanishes when filtering prompt lines —
# e.g. `echo -n` having no trailing newline causes the next prompt
# to land on the same physical line.
test_contains() {
	local name="$1" input="$2" needle="$3"
	TOTAL=$((TOTAL + 1))
	local actual
	actual=$({ printf '%s\n' "$input"; printf 'exit\n'; } | "$MS" 2>&1)
	if [[ "$actual" == *"$needle"* ]]; then
		PASS=$((PASS + 1))
		printf '%s[PASS]%s %s\n' "$C_GREEN" "$C_OFF" "$name"
	else
		FAIL=$((FAIL + 1))
		FAILED_NAMES+=("$name")
		printf '%s[FAIL]%s %s\n' "$C_RED" "$C_OFF" "$name"
		printf '       needle:  %s\n' "${needle//$'\n'/ \\n }"
		printf '       output:  %s\n' "${actual//$'\n'/ \\n }"
	fi
}

# ---------- echo ----------
test_output "echo basic" \
	"echo hello world" \
	"hello world"
test_output "echo empty" \
	"echo" \
	""
# `echo -n` skips the trailing newline; we detect this by checking that
# the next prompt lands directly after the output ("hello" + prompt) with
# no newline in between.
test_contains "echo -n suppresses newline" \
	"echo -n hello" \
	"hellominishell-1.0\$"
test_contains "echo -nnn collapses" \
	"echo -nnn hello" \
	"hellominishell-1.0\$"
test_output "echo with multiple spaces" \
	"echo a   b" \
	"a b"

# ---------- pwd ----------
test_output "pwd matches caller cwd" \
	"pwd" \
	"$(pwd -P)"

# ---------- quoting ----------
test_output "single quotes literal" \
	"echo 'a b c'" \
	"a b c"
test_output "double quotes literal" \
	'echo "a b c"' \
	"a b c"
test_output "concat adjacent quotes" \
	'echo "abc"'"'"'def'"'"'ghi' \
	"abcdefghi"
test_output "single quotes don't expand vars" \
	"echo '\$HOME'" \
	'$HOME'
test_output "double quotes do expand vars" \
	'export FOO=bar
echo "$FOO"' \
	"bar"

# ---------- env / export / unset ----------
test_output "export then expand" \
	'export FOO=bar
echo $FOO' \
	"bar"
test_output "unset clears variable" \
	'export FOO=bar
unset FOO
echo [$FOO]' \
	"[]"
test_output "exit-status \$? after true" \
	'true
echo $?' \
	"0"
test_output "exit-status \$? after false" \
	'false
echo $?' \
	"1"

# ---------- logical connectors ----------
test_output "&& runs RHS on success" \
	"true && echo yes" \
	"yes"
test_output "&& skips RHS on failure" \
	"false && echo nope" \
	""
test_output "|| skips RHS on success" \
	"true || echo nope" \
	""
test_output "|| runs RHS on failure" \
	"false || echo yes" \
	"yes"

# ---------- pipes ----------
test_output "pipe to cat" \
	"echo hello | cat" \
	"hello"
test_output "pipe to grep match" \
	"echo needle | grep needle" \
	"needle"
test_output "pipe to grep no-match exit 1" \
	"echo foo | grep bar
echo \$?" \
	"1"

# ---------- redirections ----------
test_output "redirect > writes to file" \
	"echo hi > $TESTDIR/r.txt
cat $TESTDIR/r.txt" \
	"hi"
test_output "redirect > does not leak to next cmd (regression)" \
	"echo a > $TESTDIR/leak.txt && echo b" \
	"b"
test_output "redirect >> appends" \
	"echo a > $TESTDIR/app.txt
echo b >> $TESTDIR/app.txt
cat $TESTDIR/app.txt" \
	"a
b"
test_output "redirect < reads from file" \
	"echo content > $TESTDIR/in.txt
cat < $TESTDIR/in.txt" \
	"content"

# ---------- wildcards ----------
test_output "wildcard matches single file" \
	"cd $TESTDIR
touch unique.txt
echo unique.t*" \
	"unique.txt"
test_output "wildcard no match keeps literal" \
	"cd $TESTDIR
echo nope*.zzz" \
	"nope*.zzz"

# ---------- tilde ----------
test_output "tilde expands to HOME" \
	"echo ~" \
	"$HOME"

# ---------- exit codes ----------
test_exit "exit with explicit code" "exit 42" 42
test_exit "exit defaults to 0 on EOF" "" 0
test_exit "command not found is 127" "doesnotexist_xyz" 127

# ---------- summary ----------
echo
printf 'Tests: %d  Passed: %d  Failed: %d\n' "$TOTAL" "$PASS" "$FAIL"
if [ "$FAIL" -gt 0 ]; then
	printf 'Failed:\n'
	for n in "${FAILED_NAMES[@]}"; do
		printf '  - %s\n' "$n"
	done
	exit 1
fi
