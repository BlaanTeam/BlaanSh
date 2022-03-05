/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 23:42:02 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 16:36:06 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_basename(char *path)
{
	int	len;

	if (!path)
		return ("minishell");
	len = ft_strlen(path);
	while (len && path[len] != '/')
		len--;
	path = path + len + 1;
	return (path);
}

void	set_status(int status)
{
	g_global.status = status * 256;
}

int	get_status(void)
{
	if (WIFSIGNALED(g_global.status))
		return (128 + WTERMSIG(g_global.status));
	else if (WIFSTOPPED(g_global.status))
		return (128 + WSTOPSIG(g_global.status));
	else if (WIFEXITED(g_global.status))
		return (WEXITSTATUS(g_global.status));
	return (1);
}

void	check_status(void)
{
	if (WIFSIGNALED(g_global.status) || WIFSTOPPED(g_global.status))
	{
		if (WTERMSIG(g_global.status) != SIGPIPE)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
}

void	close_pipe(int fds[2])
{
	close(fds[READ_END]);
	close(fds[WRITE_END]);
}
