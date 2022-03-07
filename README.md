# BlaanSh
BlaanSh is a Unix-like shell written in [C](https://en.wikipedia.org/wiki/C_(programming_language)) language by [@os-moussao](https://github.com/os-moussao) and [@awbx](https://github.com/awbx), this shell was inspired by [Bash](https://en.wikipedia.org/wiki/Bash_(Unix_shell)).
## Content
- [Features](#features)
- [Brief Summary](#brief-summary) 
- [Lexical Analyser](#lexical-analyser)
- [Parser](#parser)
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
cd	# Change the shell working directory.
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
# Parser
The parsing algorithm used in this project is a [recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser#:~:text=In%20computer%20science%2C%20a%20recursive,the%20nonterminals%20of%20the%20grammar.). Its job is to navigate through the list of tokens produced by the lexer function, and recursively validate each rule defined in the [grammar](#grammar) that it recognizes.

## Grammar
The shell (or BlaanSh) grammar written in [Extended Backus–Naur form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form):

```
<cmdline>	::= <block>
		| <block> (";" | "&") <cmdline>

<block>		::= <pipeline> {("&&" | "||") <pipeline>}

<pipeline>	::= <command> {"|" <command>}

<command>	::= <cmdlist>
		| "(" <cmdline> ")" <redir>	(* subshell *)

<cmdlist>	::= <redir>+			(* at least one redirect - without WORDs *)
		| <redir> {<arg> <redir>}+	(* at least one WORD - zero or more <redir> in any place *)

<redir>		::= {("<" | "<<" | ">" | ">>") <filename>}	(* a delimiter in case of heredoc *)

<arg>		::= token WORD | token VAR | token GROUP

<filename>	::= token WORD | token VAR | token GROUP
```

The [parser](/src/parser) scans the list token by token, if it fails at some point (which rarely happens because of the syntax analyser phase) it outputs an error, and returns NULL pointer. Otherwise it returns a pointer to the generated abstract syntax tree.\
\
If you read the parser source code you can notice how closely it mirrors the grammar above. That's why defining a well structured grammar is a very critical step that preceeds parser implementation.

```c
// calls the starting rule <parse_cmdline> which itself invokes the other rules
t_cmdtree	*parser(t_lexer *tokens);

t_cmdtree	*parse_cmdline(t_node **tokp);	// parses <cmdline>
t_cmdtree	*parse_block(t_node **tokp);	// parses <block>
t_cmdtree	*parse_pipeline(t_node **tokp);	// parses <pipeline>
t_cmdtree	*parse_command(t_node **tokp);	// parses <command>
t_cmdtree	*parse_cmdlist(t_node **tokp);	// parses <cmdlist>
t_cmdtree	*parse_redir(t_cmdtree *cmdtree, t_node **tokp);	// parses <redir>
```
## Abstract Syntax Tree
The tree structure `t_cmdtree` used in this project has multiple node types. And the `t_cmdtree*` pointer is casted to the corresponding type whenever we want to access its elements.\
\
Node types:
- [Subshell Node](/include/parser.h#L33): This node has a pointer to another command tree, which is ment to be executed in a child process.
- [Connector Node](/include/parser.h#L39): This markes one of these connectors `&&`, `||`, `|`, `;` or `&`. It has a pointer to a left command tree, and a pointer to a right command tree.
- [Redirect Node](/include/parser.h#L52): This type has many information about the redirection mode, source and destination, and a pointer to the tree ment to be executed under the redirection.
- [Command list Node](/include/parser.h#L46): This node type has no childs (a leaf), it has a pointer to a list of command arguments ment to be executed. This is equivalent to the numbers in a math expression.
# Executor
The [executor](/src/executor/executor.c#L15)'s job is to recursively walk the syntax tree and execute each node accordingly. You can see that there is a function run_ for each node type.
```c
// executes the whole tree by calling one of the runners depending on the node_type.
void	executor(t_cmdtree *tree);

// NODE_PIPE runner: it creates a pipe, and executes the left and the right trees.
void	run_pipeline(t_connector *connector);

// runs NODE_CMDLIST type: converts the `t_list *cmdvec` element into a char ** array and executes the array in a child process by calling execve.
void	run_cmdlist(t_cmdlist *cmdlist);

// NODE_AND, NODE_OR runner: it checks the status after executing the left tree, and decides whether or not to execute the right tree.
void	run_logical_connector(t_connector *connector, int node_type);

// NODE_SUBSH runner: it executes the subtree in a child process and waits for it
void	run_subshell(t_subsh *subshell);

// NODE_BG runner: it executes the left subtree in the background (forking without waiting for the child) and executes the right tree immediately (if it exists)
void	run_bg_connector(t_connector *connector);

// NODE_FG runner: it runs the left command tree, after its execution is finished, it executes the right tree (if it exists)
void	run_fg_connector(t_connector *connecter);

// NODE_REDIR runner: it implements the redirection logic and then executes the subtree
int		run_redirection(t_redir	*redir, int exec);
```
# Resources
- [Shell knowledge](https://www.youtube.com/playlist?list=PLFAC320731F539902)
- [CS61 Section 7 (Sell)](https://cs61.seas.harvard.edu/site/2019/Section7/), [CS61 Section 8 (Pipes)](https://cs61.seas.harvard.edu/site/2019/Section8/)
- [Recursive Descent Parsing](https://en.wikipedia.org/wiki/Recursive_descent_parser#:~:text=In%20computer%20science%2C%20a%20recursive,the%20nonterminals%20of%20the%20grammar.)
- [Learn BNF Grammar](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_form)
- [XV6's Shell Code Explained - A great source of inspiration](https://www.youtube.com/playlist?list=PLbtzT1TYeoMhF4hcpEiCsOeN13zqrzBJq)