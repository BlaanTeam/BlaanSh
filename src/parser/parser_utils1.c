/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 23:25:01 by omoussao         ###   ########.fr       */
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
			return (new_connector(NODE_FG, ret, parse_cmdline(tokp)));
		return (new_connector(NODE_BG, ret, parse_cmdline(tokp)));
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
	while (current(*tokp) == PIPE)
	{
		scan(tokp);
		ret = new_connector(NODE_PIPE, ret, NULL);
		((t_connector *)ret)->rcmdtree = parse_command(tokp);
		if (((t_connector *)ret)->rcmdtree == NULL)
			return (NULL);
	}
	return (ret);
}