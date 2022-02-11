/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 23:46:32 by asabani           #+#    #+#             */
/*   Updated: 2022/02/11 15:52:15 by asabani          ###   ########.fr       */
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

void	venv_insert(t_venv **venv_head, char *key, char *value, \
bool overwrite)
{
	t_venv	*venv_node;

	if (!key || !value)
		return ;
	venv_node = venv_find(*venv_head, key);
	if (venv_node)
	{
		if (overwrite)
			venv_node->value = value;
		return ;
	}
	venv_node = venv_new_node(key, value);
	venv_node->next = *venv_head;
	*venv_head = venv_node;
}

t_venv	*venv_init(char **env)
{
	int		i;
	char	*key;
	char	*shlvl;
	t_venv	*venv_head;

	i = -1;
	venv_head = NULL;
	shlvl = "0";
	while (env[++i])
	{
		key = getkey(env[i]);
		if (!ft_memcmp(key, "SHLVL", sizeof("SHLVL")))
			shlvl = getenv(key);
		else if (!ft_memcmp(key, "OLDPWD", sizeof("OLDPWD")))
			continue ;
		venv_insert(&venv_head, key, getenv(key), true);
	}
	venv_insert(&venv_head, "SHLVL", \
	gc_filter(ft_itoa(ft_atoi(shlvl) + 1), GC_ALL), true);
	venv_insert(&venv_head, "PWD", ft_getcwd(), true);
	if (errno == ENOENT)
		printf("shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: %s\n", strerror(errno));
	return (venv_head);
}
