/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 20:32:47 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 20:35:47 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dup2(int dest, int src)
{
	close(src);
	dup2(dest, src);
}

pid_t	ft_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (_error("fork", strerror(errno), NULL, 1), -1);
	return (pid);
}

int	ft_pipe(int fildes[2])
{
	int	ret;

	ret = pipe(fildes);
	if (ret == -1)
		_error("pipe", strerror(errno), NULL, 1);
	return (ret);
}
