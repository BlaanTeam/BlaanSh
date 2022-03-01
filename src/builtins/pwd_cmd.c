/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:20:06 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 22:39:22 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_pwd(void)
{
	ft_putendl_fd(ft_getcwd(), STDOUT_FILENO);
	set_status(0);
}

void	pwd(char **av, t_venv **venv)
{
	int	opt;
	int	i;

	(void)venv;
	if (av[0] == NULL)
		return (print_pwd());
	opt = 0;
	i = -1;
	while (opt == 0 && av[++i])
		opt = check_option("pwd", av[i]);
	if (opt >= 0)
		return (print_pwd());
}
