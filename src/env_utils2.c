/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 01:22:28 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 01:28:25 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	venv_count(t_venv *venv_head)
{
	int	counter;

	counter = 0;
	while (venv_head)
	{
		venv_head = venv_head->next;
		counter++;
	}
	return (counter);
}

char	**venv_export_array(t_venv	*venv_head)
{
	int		i;
	int		len;
	char	*str;
	char	**env;

	i = -1;
	len = venv_count(venv_head);
	env = (char **)malloc(sizeof(char *) * (len + 1));
	if (!env)
		alloc_error();
	gc_append(g_global.gc, env);
	while (venv_head && ++i < len)
	{
		str = ft_strjoin(venv_head->key, "=");
		if (!str)
			alloc_error();
		gc_append(g_global.gc, str);
		str = ft_strjoin(str, venv_head->value);
		if (!str)
			alloc_error();
		gc_append(g_global.gc, str);
		env[len - i - 1] = str;
		venv_head = venv_head->next;
	}
	return (env[len] = NULL, env);
}

/*
	These functions for debugging :
		- display_venv
		- display_env
*/

void	display_venv(t_venv *venv)
{
	printf("-----------------The venv variables-----------------\n");
	while (venv)
	{
		printf("%s:%s\n", venv->key, venv->value);
		venv = venv->next;
	}
}

void	display_env(char **env)
{
	printf("-----------------The env variables-----------------\n");
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
