/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 00:48:59 by asabani           #+#    #+#             */
/*   Updated: 2022/03/03 00:51:37 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_cmdlist(t_cmdlist *cmdlist)
{
	char	**argv;

	if (!cmdlist->cmdvec->len)
		return ;
	argv = list_export_array(cmdlist->cmdvec);
	exec_cmd(argv[0], argv);
}

void	run_subshell(t_subsh *subshell)
{
	pid_t	pid;

	pid = ft_fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		executor(subshell->cmdtree);
		exit(get_status());
	}
	waitpid(pid, &g_global.status, 0);
}

int	run_redirection(t_redir	*redir, int exec)
{
	int	open_;

	open_ = 1;
	if (redir->cmdtree && redir->cmdtree->node_type == NODE_REDIR)
		open_ = run_redirection((t_redir *)redir->cmdtree, 0);
	if (open_)
	{
		if (!(redir->redir_type & DLESS))
			redir->io_dst = open(redir->filename, redir->oflag, FILE_PERM);
		if (redir->io_dst == -1 && redir->redir_type & DLESS)
			return (set_status(1), 0);
		else if (redir->io_dst == -1)
			return (_error(redir->filename, strerror(errno), NULL, 1), 0);
		(dup2(redir->io_dst, redir->io_src), close(redir->io_dst));
		if (exec)
		{
			while (redir->cmdtree && redir->cmdtree->node_type == NODE_REDIR)
				redir = (t_redir *)redir->cmdtree;
			executor(redir->cmdtree);
		}
	}
	else
		return (set_status(1), 0);
	return (1);
}
