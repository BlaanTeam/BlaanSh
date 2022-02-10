/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:23:39 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 17:03:16 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **av)
{
	if (!av[0])
	{
		gc_clean(&g_global.gc, GC_DESTROY_SELF);
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	if (!is_numeric(av[0]))
		return (cmd_error("exit", av[0], "numeric argument required"), \
		exit(255));
	else if (av[1])
		return (cmd_error("exit", "too many arguments", NULL));
	else
		exit(ft_atoi(av[0]));
}
