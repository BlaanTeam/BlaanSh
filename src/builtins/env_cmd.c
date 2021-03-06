/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 23:49:06 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 22:27:11 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **av, t_venv **venv)
{
	if (av[0])
		return (_error("env", "too many arguments", NULL, 2));
	display_env(venv_export_array(*venv));
	set_status(0);
}
