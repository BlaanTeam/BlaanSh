# BlaaTeam Shell
Making a shell

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