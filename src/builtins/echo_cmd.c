/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 01:03:58 by asabani           #+#    #+#             */
/*   Updated: 2022/08/29 16:22:37 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_option(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-' && str[i+1] != '\0')
	{
		while (str[++i])
			if (str[i] != 'n')
				return (0);
		return (1);
	}
	return (0);
}

void	echo(char **av)
{
	int		i;
	bool	check;
	bool	nl;

	i = -1;
	nl = true;
	check = true;
	set_status(0);
	while (av[++i])
	{
		if (is_valid_option(av[i]) && check)
		{
			nl = false;
			continue ;
		}
		else
			check = false;
		ft_putstr_fd(av[i], STDOUT_FILENO);
		if (av[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
}
