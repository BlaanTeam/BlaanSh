/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:20:51 by asabani           #+#    #+#             */
/*   Updated: 2022/02/24 01:18:24 by asabani          ###   ########.fr       */
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

void	cmd_error(char *cmd, char *msg, char *extra)
{	
	int	out_fd;

	out_fd = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	printf("%s: %s: %s", g_global.program_name, cmd, msg);
	if (extra)
		printf(": %s", extra);
	printf("\n");
	set_status(1);
	dup2(out_fd, STDOUT_FILENO);
}
