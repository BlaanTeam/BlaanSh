/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 23:49:06 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 23:57:56 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **av, t_venv **venv)
{
	if (av[0])
		return (cmd_error("env", "too many arguments", NULL));
	display_env(venv_export_array(*venv));
	g_global.status = set_status(0);
}
