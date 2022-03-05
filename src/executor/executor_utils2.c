/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 00:48:59 by asabani           #+#    #+#             */
/*   Updated: 2022/03/03 20:31:20 by asabani          ###   ########.fr       */
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

static void	redirection_exec(t_redir *redir)
{
	t_redir	*tmp;

	tmp = redir;
	while (tmp->cmdtree && tmp->cmdtree->node_type == NODE_REDIR)
		tmp = (t_redir *)tmp->cmdtree;
	executor(tmp->cmdtree);
}

int	get_io_dst(t_redir	*redir)
{
	t_node	*filenode;
	char	*filename;
	int		io_dst;

	filenode = redir->filenode;
	if (filenode->token == WORD)
		filename = filenode->val;
	else if (filenode->token == GROUP)
		filename = expand_group(filenode->val_grp);
	else
	{
		filename = getvenv(filenode->val + 1);
		if (!filename || !*filename)
			return (_error(filenode->val, "ambiguous redirect", NULL, 1),
				-1);
	}
	io_dst = open(filename, redir->oflag, FILE_PERM);
	if (io_dst == -1)
		return (_error(filename, strerror(errno), NULL, 1), -1);
	return (io_dst);
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
		ft_dup2(redir->io_dst, redir->io_src);
		close(redir->io_dst);
		if (exec)
			redirection_exec(redir);
	}
	else
		return (set_status(1), 0);
	return (1);
}
