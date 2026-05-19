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

/*
 * Save stdin/stdout around a redirection chain so that any redirects
 * applied to a builtin do not leak into commands that follow on the
 * same line (e.g. `echo a > /tmp/x && echo b`).
 * For commands that run in a child process (external cmds, subshells,
 * pipelines) the restore happens in the parent, so it is harmless.
 */
static void	exec_redirection(t_cmdtree *tree)
{
	int	saved[2];

	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	run_redirection((t_redir *)tree, 1);
	if (saved[0] != -1)
	{
		dup2(saved[0], STDIN_FILENO);
		close(saved[0]);
	}
	if (saved[1] != -1)
	{
		dup2(saved[1], STDOUT_FILENO);
		close(saved[1]);
	}
}

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
		exec_redirection(tree);
}
