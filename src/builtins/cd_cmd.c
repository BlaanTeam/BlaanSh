/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 20:05:02 by asabani           #+#    #+#             */
/*   Updated: 2022/03/06 16:23:18 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *cmd, char *arg)
{
	if (arg[0] == '-')
	{
		if (arg[1] == '\0')
			return (1);
		else
			return (_error(cmd, arg, "invalid option", 2), -1);
	}
	return (0);
}

static void	change_dir(char *next_path, t_venv **venv)
{
	char	*current_path;

	current_path = ft_getcwd();
	if (chdir(next_path) == -1)
		_error("cd", next_path, strerror(errno), 1);
	else
	{
		ft_getcwd();
		if ((ft_memcmp(next_path, ".", 2) == 0 || \
		ft_memcmp(next_path, "..", 3) == 0) && errno == ENOENT)
			return (_error("cd", "error retrieving current directory: \
getcwd: cannot access parent directories", strerror(errno), 1));
		venv_insert(venv, "OLDPWD", current_path, E_GLOBAL);
		venv_insert(venv, "PWD", ft_getcwd(), E_GLOBAL);
		set_status(0);
	}
}

void	cd(char **av, t_venv **venv)
{
	int		opt;
	char	*var;

	if (av[0] == NULL)
	{
		var = getvenv("HOME");
		if (!var)
			return (_error("cd", "HOME not set", NULL, 1));
		return (change_dir(var, venv));
	}
	opt = check_option("cd", av[0]);
	if (opt == 1)
	{
		var = getvenv("OLDPWD");
		if (!var)
			return (_error("cd", "OLDPWD not set", NULL, 1));
		return (change_dir(var, venv));
	}
	else if (opt == 0)
		return (change_dir(av[0], venv));
	return ;
}
