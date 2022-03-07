# BlaanSh
BlaanSh is a Unix-like shell written in [C](https://en.wikipedia.org/wiki/C_(programming_language)) language by [@os-moussao](https://github.com/os-moussao) and [@awbx](https://github.com/awbx), this shell was inspired by [Bash](https://en.wikipedia.org/wiki/Bash_(Unix_shell)).
## Content
- [Features](#features)
- [Brief Summary](#brief-summary) 
- [Lexical Analyser](#lexicat-analyser)
- [Recursive Descent Parser](#recursive-descent-parser)
- [Executor](#executor)
- [Resources](#resources)

# Features
### Foreground and Background (`;` and `&`)
```bash
$> touch file ; ls -la file ; ping -c 1 google.com &
```

### Conditionals (`&&` and `||`)
```bash
$> false && echo success || echo failure
```

### Pipes `|` 
```bash
$> cat /etc/passwd | cut -d':' -f1 | sort | uniq # get unique user's
```

### I/O Redirections (`<`, `<<`, `>` and `>>`)
```bash
$> cat << "EOF" > outfile ; < infile cat >> outfile
```

### Subshell `( <cmd> )`
```bash
$> cd / ; (cd bin && ./ls -G .); pwd # The current working directory is still `/`
```

### Built-in Commands
```bash
echo	# Write arguments to the standard output.
cd 		# Change the shell working directory.
pwd 	# Print the name of the current working directory.
export 	# Set export attribute for shell variables.
unset 	# Unset values and attributes of shell variables.
env 	# Set environment and execute command, or print environment.
exit	# Exit the shell.
```

# Brief Summary
Before getting any input, we first have to handle Ctrl-C, Ctrl-D, Ctrl-Z and ctrl-\ which should behave like in bash. After that, the program is continuously prompting for a new `command line` input.\
This `command line` goes through many phases before being executed and showing the result.\
First it goes through a [lexer](/src/tokenizer/lexer.c), which splits the input into a "valid" list of tokens. Second, the [parser](/src/parser/parser.c) uses that list to produce an [abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree). And last but not the least, the [executor](/src/executor/executor.c) walks this tree recursively and executes each node accordingly.

# Lexical Analyser
The [lexer](https://en.wikipedia.org/wiki/Lexical_analysis#:~:text=A%20program%20that%20performs%20lexical,web%20pages%2C%20and%20so%20forth.) (or lexical analyser) is the first stage of parsing the `command line` input.
- It consists three important steps:
	- [Tokenizer](/src/tokenizer/tokenizer.c#L89): It converts the command line into an "initial" list of tokens which contains every detail of the `char *cmdline`. The tokenizer does not check for syntax errors inside the `cmdline`, this task is left to the syntax analyser.
	- [Syntax analyser](/src/syntax_analyser/syntax_analyser.c#L106): It goes through the doubly linked list of tokens, and checks for syntax errors at every node. This is done by looking at the left and the right of the current node, if some strange or some unexpected token is encountered, an error is printed to `stderr` and a falsy value is returned to the lexer function. [Bellow](#syntax-rules) are the rules of the syntax analyser.
	- [Expander](/src/expander/expander.c#L15): If the syntax analyser does not produce any error, the list is then passed to an expander which removes quotes and whitespaces, and expands the tilde and the wildcard patterns to their values. Variable expansion is done during execution.

## Syntax Rules
````
- UNEXPECTED TOKENS:
	- `;;' (this token is always unrecognized in bash and zsh shells)

* AND, OR, PIPE, FG, BG:
	- left: [WSPACE] (STRING | CPAR)
	- right: [WSPACE] (STRING | REDIRECT | OPAR | if <FG, BG> ENDOFCMD)

* OPAR "(":
	- left: CMDBEGIN | [WSPACE] (AND | OR | PIPE | OPAR)
	- right: [WSPACE] (STRING | REDIRECT | OPAR)

* CPAR ")":
	- left: [WSPACE] (STRING | CPAR)
	- right: [WSPACE] (AND | OR | PIPE | CPAR | ENDOFCMD)

* REDIRECT:
	- right: [WSPACE] STRING

* PARENTHESES MATCHING AND QUOTING:
	- inside each pair parentheses should not be an empthy command.
	- every open parentheses has to have a matching closing parentheses.
	- every single/double quote have to be closed.
````

```c
// inside the lexer function
tokens = tokenizer(cmdline);
if (!validate_syntax(tokens))
{
	set_status(2);
	return (NULL);
}
tokens = expander(tokens);
```

## Grammar
The grammar in [Extended Backus–Naur](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form) form:
```
<cmdline>	::= <block>
		| <block> (";" | "&") <cmdline>

<block>		::= <pipeline> {("&&" | "||") <pipeline>}

<pipeline>	::= <command> {"|" <command>}

<command>	::= <cmdlist>
		| "(" <cmdline> ")" <redir>	(* subshell *)

<cmdlist>	::= <redir>+			(* at least one redirect - without WORDs *)
		| <redir> {<arg> <redir>}+	(* at least one WORD - zero or more <redir> in any place *)

<redir>		::= {("<" | "<<" | ">" | ">>") <filename>}

<arg>		::= WORD

<filename>	::= WORD
```