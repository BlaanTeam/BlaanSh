/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 23:10:31 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 23:42:35 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	bool	is_option(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-' && str[i + 1])
		return (true);
	return (false);
}

void	unset(char **av, t_venv **venv)
{
	int	i;

	i = -1;
	while (av[++i])
	{
		if (is_option(av[i]))
			return (cmd_error("unset", av[i], "invalid option"));
		else if (!is_identifier(av[i]))
			return (cmd_error("unset", av[i], "not a valid identifier"));
		venv_remove(venv, av[i]);
	}
	g_global.status = set_status(0);
}
