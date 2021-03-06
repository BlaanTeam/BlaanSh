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
		((t_connector *)ret)->rcmdtree = parse_cmdline(tokp);
		if (((t_connector *)ret)->rcmdtree == NULL)
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
		((t_connector *)ret)->rcmdtree = parse_pipeline(tokp);
		if (((t_connector *)ret)->rcmdtree == NULL)
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
		((t_connector *)ret)->rcmdtree = parse_command(tokp);
		if (((t_connector *)ret)->rcmdtree == NULL)
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
		((t_subsh *)ret)->cmdtree = parse_cmdline(tokp);
		if (((t_subsh *)ret)->cmdtree == NULL || !accept(tokp, CPAR))
			return (NULL);
		return (parse_redir(ret, tokp));
	}
	return (parse_cmdlist(tokp));
}

// <cmdlist> ::=  <redir>+
//           |    <redir> {<arg> <redir>}+
t_cmdtree	*parse_cmdlist(t_node **tokp)
{
	t_cmdtree	*ret;
	t_cmdlist	*cmdlist;

	if (current(*tokp) == ENDOFCMD)
		return (NULL);
	ret = new_cmdlist();
	cmdlist = (t_cmdlist *)ret;
	ret = parse_redir(ret, tokp);
	if (!ret)
		return (NULL);
	while (current(*tokp) & (WORD | VAR | GROUP))
	{
		push_back(cmdlist->cmdvec, (*tokp)->token, (*tokp)->val);
		cmdlist->cmdvec->bottom->val_grp = (*tokp)->val_grp;
		scan(tokp);
		ret = parse_redir(ret, tokp);
		if (!ret)
			return (NULL);
	}
	if (ret == (t_cmdtree *)cmdlist && !cmdlist->cmdvec->len)
		ret = NULL;
	return (ret);
}
