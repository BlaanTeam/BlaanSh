/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 17:09:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 21:08:03 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The grammar recognized by the parser:

// <cmdline>    ::=  <block> {(";" | "&") <block>} [(";" | "&")]
// 
// <block>      ::=  <pipeline> {("&&" | "||") <pipeline>}
//
// <pipeline>   ::=  <command> {"|" <command>}
//
// <command>    ::=  <cmdlist>
//              |    "(" <cmdline> ")" <redir>  (* subshell *)
//
// <cmdlist>    ::=  <redir> {<arg> <redir>}+
//
// <redir>      ::=  {("<" | "<<" | ">" | ">>") <filename>}
//
// <arg> <filename> ::= token WORD | token PATH

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
