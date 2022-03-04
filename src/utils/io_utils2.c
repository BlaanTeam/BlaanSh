/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 20:29:17 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 20:29:52 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	load_ttyname(void)
{
	int		io[3];
	int		i;

	io[0] = STDIN_FILENO;
	io[1] = STDOUT_FILENO;
	io[2] = STDERR_FILENO;
	i = -1;
	while (++i < 3)
	{
		g_global.ttyname = ttyname(io[i]);
		if (g_global.ttyname)
			return ;
	}
}

void	save_io(int io[2])
{
	io[0] = dup(STDIN_FILENO);
	io[1] = dup(STDOUT_FILENO);
	if (io[0] == -1 || io[1] == -1)
		_error("dup", strerror(errno), NULL, 1);
}

void	reset_io(int io[2])
{
	if (dup2(io[0], STDIN_FILENO) == -1 || dup2(io[1], STDOUT_FILENO) == -1)
		_error("dup2", strerror(errno), NULL, 1);
}
