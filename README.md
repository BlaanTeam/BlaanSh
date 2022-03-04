# BlaanSh
BlaanSh is a Unix-like shell written in [C](https://en.wikipedia.org/wiki/C_(programming_language)) language by [@os-moussao](https://github.com/os-moussao) and [@awbx](https://github.com/awbx), this shell was inspired by [Bash](https://en.wikipedia.org/wiki/Bash_(Unix_shell)).

## Syntax
The base syntax used in the [syntax_analyser.c](/src/syntax_analyser/syntax_analyser.c):
```
* UNEXPECTED TOKENS:
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
	- inside each pair parentheses sould not be an empthy command
	- every open parentheses has to have a matching closing parentheses
	- every single/double quote have to be closed
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