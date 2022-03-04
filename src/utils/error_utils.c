/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:20:51 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 00:50:07 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_code(int status, char *msg, bool silently)
{
	if (!silently)
		perror(msg);
	gc_clean(&g_global.gc, GC_DESTROY_SELF);
	rl_clear_history();
	exit(status);
}

void	alloc_error(void)
{
	errno = ENOMEM;
	exit_with_code(EXIT_FAILURE, "malloc", false);
}

void	_error(char *title, char *msg, char *extra, int status)
{
	ft_putstr_fd(get_basename(g_global.program_name), STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(title, STDERR_FILENO);
	if (msg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(msg, STDERR_FILENO);
	}
	if (extra)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(extra, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
	set_status(status);
}
