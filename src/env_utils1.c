/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 23:46:32 by asabani           #+#    #+#             */
/*   Updated: 2022/02/06 00:35:51 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*getkey(char *str)
{
	int		i;
	char	*key;

	i = 0;
	key = ft_strdup(str);
	if (!key)
		alloc_error();
	gc_append(g_global.gc, key, GC_ALL);
	while (str[i] && str[i] != '=')
		i++;
	key[i] = 0x0;
	return (key);
}

static t_venv	*venv_new_node(char *key, char *value)
{
	t_venv	*venv_node;

	venv_node = (t_venv *)ft_malloc(sizeof(t_venv));
	gc_append(g_global.gc, venv_node, GC_ALL);
	venv_node->key = key;
	venv_node->value = value;
	venv_node->next = NULL;
	return (venv_node);
}

static void	venv_insert(t_venv **venv_head, char *key, char *value)
{
	t_venv	*venv_node;

	venv_node = venv_new_node(key, value);
	venv_node->next = *venv_head;
	*venv_head = venv_node;
}

t_venv	*venv_init(char **env)
{
	int		i;
	char	*key;
	t_venv	*venv_head;

	i = -1;
	key = NULL;
	venv_head = NULL;
	while (env[++i])
	{
		key = getkey(env[i]);
		venv_insert(&venv_head, key, getenv(key));
	}
	return (venv_head);
}
