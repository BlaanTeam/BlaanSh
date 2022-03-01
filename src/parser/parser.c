/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 17:09:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/01 17:14:59 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Parser
t_cmdtree	*parser(t_lexer *tokens)
{
	t_cmdtree	*syntax_tree;
	t_node		*tokp;

	if (!tokens || tokens->len <= 1)
		return (NULL);
	tokp = tokens->top;
	syntax_tree = parse_cmdline(&tokp);
	if (!syntax_tree
		|| (tokp && current(tokp) != ENDOFCMD))
	{
		_error("parser", "Syntax Error", NULL, 2);
		return (NULL);
	}
	return (syntax_tree);
}
