# BlaaTeam Shell
Making a shell

## Syntax
The base syntax used in the [syntax_analyser.c](/src/syntax_analyser/syntax_analyser.c):
```
* UNEXPECTED TOKENS:
	- `;' | `;;' | `&'
	(we do not handle foreground or background tokens)

* AND_IF, OR_IF, PIPE:
	- left: [WHITESPACE] (STRING | CPAR)
	- right: [WHITESPACE] (STRING | REDIRECT | OPAR)

* OPAR "(":
	- left: CMDBEGIN | [WHITESPACE] (AND_IF | OR_IF | PIPE | OPAR)
	- right: [WHITESPACE] (STRING | REDIRECT | OPAR)

* CPAR ")":
	- left: [WHITESPACE] (STRING | CPAR)
	- right: [WHITESPACE] (AND_IF | OR_IF | PIPE | CPAR | ENDOFCMD)

* REDIRECT:
	- right: [WHITESPACE] STRING

* PARENTHESES MATCHING AND QUOTING:
	- inside each pair parentheses sould not be an empthy command
	- every open parentheses has to have a matching closing parentheses
	- every single/double quote have to be closed
```