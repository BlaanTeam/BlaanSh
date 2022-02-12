/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 23:46:32 by asabani           #+#    #+#             */
/*   Updated: 2022/02/12 18:06:35 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getkey(char *var)
{
	int	i;

	i = 0;
	while (var[i] && (var[i] != '+' || var[i + 1] != '=') && var[i] != '=')
		i++;
	return (gc_filter(ft_strndup(var, i + 1), GC_ALL));
}

char	*getvalue(char *var)
{
	int	i;

	i = 0;
	while (var[i] && (var[i] != '+' || var[i + 1] != '=') && var[i] != '=')
		i++;
	if (var[i] == '+' && var[i + 1] == '=')
		i++;
	else if (var[i] == '+' && var[i + 1] != '=')
		return (NULL);
	i++;
	return (gc_filter(ft_strdup(var + i), GC_ALL));
}

static t_venv	*venv_new_node(char *key, char *value, short eflag)
{
	t_venv	*venv_node;

	venv_node = (t_venv *)ft_malloc(sizeof(t_venv));
	gc_append(g_global.gc, venv_node, GC_ALL);
	venv_node->key = key;
	venv_node->value = value;
	venv_node->eflag = eflag;
	venv_node->next = NULL;
	return (venv_node);
}

void	venv_insert(t_venv **venv_head, char *key, char *value, short eflag)
{
	t_venv	*venv_node;

	if (!key || !value)
		return ;
	venv_node = venv_find(*venv_head, key);
	if (venv_node)
	{
		if (eflag & E_GLOBAL)
			venv_node->eflag = E_GLOBAL;
		if (eflag & E_LOCAL)
			venv_node->eflag = E_LOCAL;
		venv_node->value = value;
		return ;
	}
	venv_node = venv_new_node(key, value, eflag);
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
		venv_insert(&venv_head, key, getenv(key), E_GLOBAL);
	}
	venv_insert(&venv_head, "SHLVL", \
	gc_filter(ft_itoa(ft_atoi(shlvl) + 1), GC_ALL), E_GLOBAL);
	venv_insert(&venv_head, "PWD", ft_getcwd(), E_GLOBAL);
	if (errno == ENOENT)
		printf("shell-init: error retrieving current directory: \
getcwd: cannot access parent directories: %s\n", strerror(errno));
	return (venv_head);
}
