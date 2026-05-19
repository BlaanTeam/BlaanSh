/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 22:25:35 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// <cmdline> ::=  <block>
//           |    <block> (";" | "&") <cmdline>
t_cmdtree	*parse_cmdline(t_node **tokp)
{
	t_cmdtree	*ret;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	ret = parse_block(tokp);
	if (!ret)
		return (NULL);
	if (current(*tokp) & (FG | BG))
	{
		if (scan(tokp) == FG)
			ret = new_connector(NODE_FG, ret, NULL);
		else
			ret = new_connector(NODE_BG, ret, NULL);
		if (current(*tokp) & (ENDOFCMD | CPAR))
			return (ret);
		ret->u.conn.rcmdtree = parse_cmdline(tokp);
		if (ret->u.conn.rcmdtree == NULL)
			return (NULL);
	}
	return (ret);
}

// <block> ::=  <pipeline> {("&&" | "||") <pipeline>}
t_cmdtree	*parse_block(t_node **tokp)
{
	t_cmdtree	*ret;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	ret = parse_pipeline(tokp);
	if (!ret)
		return (NULL);
	while (current(*tokp) & (AND | OR))
	{
		if (scan(tokp) == AND)
			ret = new_connector(NODE_AND, ret, NULL);
		else
			ret = new_connector(NODE_OR, ret, NULL);
		ret->u.conn.rcmdtree = parse_pipeline(tokp);
		if (ret->u.conn.rcmdtree == NULL)
			return (NULL);
	}
	return (ret);
}

// <pipeline> ::=  <command> {"|" <command>}
t_cmdtree	*parse_pipeline(t_node **tokp)
{
	t_cmdtree	*ret;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	ret = parse_command(tokp);
	if (!ret)
		return (NULL);
	while (accept(tokp, PIPE))
	{
		ret = new_connector(NODE_PIPE, ret, NULL);
		ret->u.conn.rcmdtree = parse_command(tokp);
		if (ret->u.conn.rcmdtree == NULL)
			return (NULL);
	}
	return (ret);
}

// <command>  ::=  <cmdlist>
//            |    "(" <cmdline> ")" <redir>  (* subshell *)
t_cmdtree	*parse_command(t_node **tokp)
{
	t_cmdtree	*ret;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	if (accept(tokp, OPAR))
	{
		ret = new_subsh(NULL);
		ret->u.subsh.cmdtree = parse_cmdline(tokp);
		if (ret->u.subsh.cmdtree == NULL || !accept(tokp, CPAR))
			return (NULL);
		return (parse_redir(ret, tokp));
	}
	return (parse_cmdlist(tokp));
}

// <cmdlist> ::=  <redir>+
//           |    <redir> {<arg> <redir>}+
//
// `cmdlist_node` keeps a handle on the original NODE_CMDLST node so we can
// push args into its cmdvec — `ret` may have been wrapped in NODE_REDIR by
// parse_redir.
t_cmdtree	*parse_cmdlist(t_node **tokp)
{
	t_cmdtree	*ret;
	t_cmdtree	*cmdlist_node;
	t_list		*cmdvec;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	cmdlist_node = new_cmdlist();
	cmdvec = cmdlist_node->u.cmdlist.cmdvec;
	ret = parse_redir(cmdlist_node, tokp);
	if (!ret)
		return (NULL);
	while (current(*tokp) & (WORD | VAR | GROUP))
	{
		push_back(cmdvec, (*tokp)->token, (*tokp)->val);
		cmdvec->bottom->val_grp = (*tokp)->val_grp;
		scan(tokp);
		ret = parse_redir(ret, tokp);
		if (!ret)
			return (NULL);
	}
	if (ret == cmdlist_node && !cmdvec->len)
		ret = NULL;
	return (ret);
}
