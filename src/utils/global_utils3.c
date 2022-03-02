/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 23:42:02 by asabani           #+#    #+#             */
/*   Updated: 2022/03/02 01:22:42 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_program_name(char *path)
{
	int	len;

	len = ft_strlen(path);
	while (len && path[len] != '/')
		len--;
	path = path + len + 1;
	g_global.program_name = path;
}

void	set_status(int status)
{
	g_global.status = status * 256;
}

int	get_status(void)
{
	int	sig;

	if (WIFSIGNALED(g_global.status))
	{
		sig = WTERMSIG(g_global.status);
		return (128 + sig);
	}
	return (WEXITSTATUS(g_global.status));
}

void	close_pipe(int fds[2])
{
	close(fds[READ_END]);
	close(fds[WRITE_END]);
}

void	ft_dup2(int dest, int src)
{
	close(src);
	dup2(dest, src);
}

void	check_status(void)
{
	if (WIFSIGNALED(g_global.status))
	{
		ft_putstr_fd(strsignal(WTERMSIG(g_global.status)), STDOUT_FILENO); // TODO: remove in production
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (_error("fork", strerror(errno), NULL, 1), -1);
	return (pid);
}
