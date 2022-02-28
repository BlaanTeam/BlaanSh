/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 17:09:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/28 16:30:14 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The grammar recognized by this parser:

// <cmdline>    ::= <block>
//              |   <block> (";" | "&") <cmdline>
// 
// <block>      ::=  <pipeline> {("&&" | "||") <pipeline>}
//
// <pipeline>   ::=  <command> {"|" <command>}
//
// <command>    ::=  <cmdlist>
//              |    "(" <cmdline> ")" <redir>  (* subshell *)
//
// <cmdlist>    ::=  <redir>+                    (* at least one redirect - without WORDs *)
//              |    <redir> {<arg> <redir>}+    (* at least one WORD - zero or more <redir> in any place *)
//
// <redir>      ::=  {("<" | "<<" | ">" | ">>") <filename>}
//
// <arg> <filename> ::= token WORD

// Parser
t_cmdtree	*parser(t_lexer *tokens)
{
	t_cmdtree	*abstract_syntax_tree;
	t_node		*tokp;

	tokp = tokens->top;
	abstract_syntax_tree = parse_cmdline(&tokp);
	if (!abstract_syntax_tree
		|| (tokp && current(tokp) != ENDOFCMD))
	{
		_error("Parser: Syntax Error", 158);
		return (NULL);
	}
	return (abstract_syntax_tree);
}
