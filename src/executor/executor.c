/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 16:12:41 by asabani           #+#    #+#             */
/*   Updated: 2022/03/03 00:45:43 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor(t_cmdtree *tree)
{
	if (!tree)
		return ;
	if (tree->node_type == NODE_CMDLST)
		return (run_cmdlist((t_cmdlist *)tree));
	else if (tree->node_type == NODE_PIPE)
		return (run_pipeline((t_connector *)tree));
	else if (tree->node_type == NODE_AND || tree->node_type == NODE_OR)
		return (run_logical_connector((t_connector *)tree, tree->node_type));
	else if (tree->node_type == NODE_SUBSH)
		return (run_subshell((t_subsh *)tree));
	else if (tree->node_type == NODE_FG)
		return (run_fg_connector((t_connector *)tree));
	else if (tree->node_type == NODE_BG)
		return (run_bg_connector((t_connector *)tree));
	else if (tree->node_type == NODE_REDIR)
		run_redirection((t_redir *)tree, 1);
}
