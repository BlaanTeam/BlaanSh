/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:20:51 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 16:54:08 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_error(char *msg)
{
	perror(msg);
	gc_clean(&g_global.gc, GC_ALL);
	rl_clear_history();
	exit(EXIT_FAILURE);
}

void	alloc_error(void)
{
	errno = ENOMEM;
	exit_with_error("malloc");
}

void	cmd_error(char *cmd, char *msg, char *extra)
{	
	int	out_fd;

	out_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("bash: %s: %s", cmd, msg);
	if (extra)
		printf(": %s", extra);
	printf("\n");
	g_global.status = set_status(1);
	dup2(out_fd, STDOUT_FILENO);
}
