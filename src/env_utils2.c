/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 01:22:28 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 23:56:10 by asabani          ###   ########.fr       */
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
	env = (char **)ft_malloc(sizeof(char *) * (len + 1));
	gc_append(g_global.gc, env, GC_ALL);
	while (venv_head && ++i < len)
	{
		str = ft_strjoin(venv_head->key, "=");
		if (!str)
			alloc_error();
		gc_append(g_global.gc, str, GC_ALL);
		str = ft_strjoin(str, venv_head->value);
		if (!str)
			alloc_error();
		gc_append(g_global.gc, str, GC_ALL);
		env[len - i - 1] = str;
		venv_head = venv_head->next;
	}
	return (env[len] = NULL, env);
}

t_venv	*venv_find(t_venv *venv_head, char *key)
{
	while (venv_head)
	{
		if (ft_memcmp(venv_head->key, key, ft_strlen(key) + 1) == 0)
			return (venv_head);
		venv_head = venv_head->next;
	}
	return (NULL);
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
		printf("%s=%s\n", venv->key, venv->value);
		venv = venv->next;
	}
}

void	display_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
}
