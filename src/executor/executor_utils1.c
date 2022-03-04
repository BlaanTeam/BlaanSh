/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 00:45:47 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 18:18:23 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	run_pipe(t_connector *connector, int fds[2], int side)
{
	pid_t		pid;
	int			end;
	int			fileno;
	t_cmdtree	*cmdtree;

	end = WRITE_END;
	fileno = STDOUT_FILENO;
	cmdtree = connector->lcmdtree;
	if (side & RIGHT_SIDE)
	{
		end = READ_END;
		fileno = STDIN_FILENO;
		cmdtree = connector->rcmdtree;
	}
	pid = ft_fork();
	if (pid == 0)
	{
		ft_dup2(fds[end], fileno);
		close_pipe(fds);
		executor(cmdtree);
		exit(get_status());
	}
	return (pid);
}

void	run_pipeline(t_connector *connector)
{
	int	status;
	int	pids[2];
	int	fds[2];

	if (ft_pipe(fds) == -1)
		return ;
	pids[0] = run_pipe(connector, fds, LEFT_SIDE);
	if (pids[0] == -1)
		return ;
	pids[1] = run_pipe(connector, fds, RIGHT_SIDE);
	if (pids[1] == -1)
		return ;
	close_pipe(fds);
	if (wait(&status) == pids[1])
		g_global.status = status;
	if (wait(&status) == pids[1])
		g_global.status = status;
}

void	run_logical_connector(t_connector *connector, int node_type)
{
	executor(connector->lcmdtree);
	if (node_type == NODE_AND)
	{
		if (WIFEXITED(g_global.status) && WEXITSTATUS(g_global.status) == 0)
			executor(connector->rcmdtree);
	}
	else
	{
		if (WIFEXITED(g_global.status) && WEXITSTATUS(g_global.status) != 0)
			executor(connector->rcmdtree);
	}
}

void	run_bg_connector(t_connector *connector)
{
	pid_t	pid;

	pid = ft_fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		executor(connector->lcmdtree);
		exit(get_status());
	}
	executor(connector->rcmdtree);
}

void	run_fg_connector(t_connector *connecter)
{
	executor(connecter->lcmdtree);
	executor(connecter->rcmdtree);
}
