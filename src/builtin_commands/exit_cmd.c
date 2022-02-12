/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:23:39 by asabani           #+#    #+#             */
/*   Updated: 2022/02/12 18:02:36 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **av)
{
	if (!av[0])
		exit_with_cleanup();
	if (!is_numeric(av[0]))
		return (cmd_error("exit", av[0], "numeric argument required"), \
		exit_with_code(255, NULL, true));
	else if (av[1])
		return (cmd_error("exit", "too many arguments", NULL));
	else
		exit_with_code(ft_atoi(av[0]), NULL, true);
}
