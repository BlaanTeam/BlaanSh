/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:00:14 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 22:33:24 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define APPEND_MODE 1
#define ASSIGN_MODE 2

static void	display_prefixed_venv(t_venv *venv)
{
	if (venv)
	{
		display_prefixed_venv(venv->next);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(venv->key, STDOUT_FILENO);
		if (venv->value && *venv->value)
		{
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(venv->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

int	is_assignment(char *var, short *mode)
{
	int	i;

	if (!var)
		return (false);
	i = 0;
	if (ft_isalpha(var[i]) || var[i] == '_')
	{
		while (var[++i] && \
		(var[i] != '+' || var[i + 1] != '=') && var[i] != '=')
			if (!(ft_isalnum(var[i]) || var[i] == '_'))
				return (false);
		if (var[i] == '+' && var[i + 1] == '=')
			*mode = APPEND_MODE;
		else if (var[i] == '=')
			*mode = ASSIGN_MODE;
		else
			return (false);
		return (true);
	}
	return (false);
}

static void	declare(t_venv **venv, short eflag, char *var, int mode)
{
	t_venv	*venv_node;
	char	*key;
	char	*value;

	key = getkey(var);
	value = getvalue(var);
	venv_node = venv_find(*venv, key);
	if (venv_node)
	{
		if (mode & APPEND_MODE)
			venv_node->value = \
			gc_filter(ft_strjoin(venv_node->value, value), GC_ALL);
		else if (mode & ASSIGN_MODE)
		{
			venv_node->value = value;
			venv_node->eflag = eflag;
		}
		if (eflag & E_GLOBAL)
			venv_node->eflag |= E_GLOBAL;
		return ;
	}
	venv_insert(venv, key, value, eflag);
}

void	export(char **av, t_venv **venv)
{
	int		i;
	bool	check;
	short	mode;

	set_status(0);
	if (!av[0])
		return (display_prefixed_venv(*venv));
	i = -1;
	mode = -1;
	check = true;
	while (av[++i])
	{
		if (is_option(av[i]) && check)
			return (_error("export", av[i], "invalid option", 2));
		else if (is_identifier(av[i]))
			declare(venv, E_GLOBAL | E_EMPTY, av[i], 0);
		else if (is_assignment(av[i], &mode))
			declare(venv, E_GLOBAL, av[i], mode);
		else
			_error("export", av[i], "not a valid identifier", 1);
		check = false;
	}
}
