/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 23:17:37 by omoussao         ###   ########.fr       */
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
