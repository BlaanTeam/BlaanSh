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

# Strip the main prompt and the heredoc continuation prompt (`> `).
PROMPT_RE='^(minishell-1\.0\$|> )'

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

# ---------- complex pipes ----------
test_output "3-stage pipe" \
	"echo abc | cat | cat" \
	"abc"
test_output "8-stage pipe" \
	"echo deepwater | cat | cat | cat | cat | cat | cat | cat" \
	"deepwater"
test_output "pipe + sort + uniq" \
	"echo c
echo a
echo b
echo a" \
	"c
a
b
a"
test_output "pipe into wc -l (count lines)" \
	"echo line | cat
echo line | cat
echo line | cat | wc -l | tr -d ' '" \
	"line
line
1"

# ---------- semicolon / FG sequence ----------
test_output "semicolon runs both" \
	"echo a; echo b" \
	"a
b"
test_output "semicolon with mix of cmds" \
	"echo a; false; echo b" \
	"a
b"
test_output "semicolon preserves \$? of last" \
	"true; false
echo \$?" \
	"1"

# ---------- subshells ----------
test_output "subshell sequence" \
	"(echo a; echo b)" \
	"a
b"
test_output "subshell piped to cat" \
	"(echo a; echo b) | cat" \
	"a
b"
test_output "subshell writes to file" \
	"(echo a; echo b) > $TESTDIR/sub.txt
cat $TESTDIR/sub.txt" \
	"a
b"
test_output "subshell does not leak cd" \
	"(cd /tmp) && pwd | grep -c $(basename $(pwd -P))" \
	"1"

# ---------- precedence ----------
test_output "|| then && evaluates left-to-right" \
	"false || true && echo ok" \
	"ok"
test_output "&& then || short-circuits" \
	"true && false || echo recovered" \
	"recovered"
test_output "parens override precedence" \
	"false && (true || echo never) || echo good" \
	"good"
test_output "deep parens chain" \
	"((echo deep))" \
	"deep"

# ---------- complex redirections ----------
test_output "last > wins, first file is truncated" \
	"echo lastwin > $TESTDIR/r1.txt > $TESTDIR/r2.txt
cat $TESTDIR/r2.txt" \
	"lastwin"
test_output "first > of multi-redir is truncated to empty" \
	"echo lastwin > $TESTDIR/m1.txt > $TESTDIR/m2.txt
cat $TESTDIR/m1.txt
echo END" \
	"END"
test_output "pipe with redirect on writer" \
	"echo content | cat > $TESTDIR/pw.txt
cat $TESTDIR/pw.txt" \
	"content"
test_output "input + output redirect on same cmd" \
	"echo source > $TESTDIR/src.txt
cat < $TESTDIR/src.txt > $TESTDIR/dst.txt
cat $TESTDIR/dst.txt" \
	"source"
test_output "redirect + && chain doesn't leak (regression)" \
	"echo first > $TESTDIR/chain1.txt && echo second > $TESTDIR/chain2.txt
echo afterward
cat $TESTDIR/chain1.txt
cat $TESTDIR/chain2.txt" \
	"afterward
first
second"

# ---------- heredoc ----------
test_output "heredoc reads up to delimiter" \
	"cat << DELIM
line1
line2
DELIM
echo done" \
	"line1
line2
done"
test_output "heredoc with empty body" \
	"cat << END
END
echo after" \
	"after"
test_output "heredoc piped to grep" \
	"grep needle << END
no
needle here
also no
END" \
	"needle here"

# ---------- variable expansion edge cases ----------
test_output "var concat with surrounding text" \
	"export X=middle
echo pre\$X post" \
	"premiddle post"
test_output "undefined var collapses to empty" \
	"echo [\$DEFINITELY_NOT_SET_XYZ]" \
	"[]"
test_output "many adjacent vars" \
	"export A=1
export B=2
export C=3
echo \$A\$B\$C" \
	"123"
test_output "\$? in middle of pipeline" \
	"true
echo \$?:\$?:\$?" \
	"0:0:0"

# ---------- builtin: env ----------
test_output "env contains exported var" \
	"export ZZZ_MARKER=present
env | grep ZZZ_MARKER" \
	"ZZZ_MARKER=present"

# ---------- error cases (must not crash) ----------
test_exit "unclosed single quote sets \$? to 2" \
	"echo 'oops" 2
test_exit "unclosed double quote sets \$? to 2" \
	"echo \"oops" 2
test_exit "syntax error: pipe at start sets \$? to 2" \
	"| echo bad" 2
test_exit "syntax error: dangling && sets \$? to 2" \
	"echo a &&" 2
test_exit "syntax error: empty parens" \
	"()" 2
test_exit "exit with non-numeric arg returns 255" \
	"exit notanumber" 255

# ---------- whitespace / empty ----------
test_output "blank line is no-op" \
	"
echo afterblank" \
	"afterblank"
test_output "lots of spaces between args" \
	"echo     a     b     c" \
	"a b c"
test_output "leading whitespace is fine" \
	"   echo indented" \
	"indented"

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
