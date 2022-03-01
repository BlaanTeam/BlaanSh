/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 23:42:02 by asabani           #+#    #+#             */
/*   Updated: 2022/02/24 20:40:17 by asabani          ###   ########.fr       */
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

