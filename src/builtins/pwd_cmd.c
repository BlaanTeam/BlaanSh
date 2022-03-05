/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 15:20:06 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 20:29:21 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_pwd(void)
{
	ft_putstr_fd(ft_getcwd(), STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
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
