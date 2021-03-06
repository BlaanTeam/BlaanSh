/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 18:43:43 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 00:42:20 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	venv_remove(t_venv **venv_head, char *key)
{
	t_venv	*prev;
	t_venv	*tmp;

	tmp = *venv_head;
	if (tmp && ft_memcmp(tmp->key, key, ft_strlen(key) + 1) == 0)
	{
		*venv_head = tmp->next;
		return (true);
	}
	while (tmp && ft_memcmp(tmp->key, key, ft_strlen(key) + 1) != 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return (false);
	prev->next = tmp->next;
	return (true);
}

char	*getvenv(char *key)
{
	t_venv	*node;

	if (!key)
		return (NULL);
	if (ft_memcmp(key, "?", 2) == 0)
		return (gc_filter(ft_itoa(get_status()), GC_TMP));
	else if (ft_memcmp(key, "0", 2) == 0)
		return (g_global.program_name);
	node = venv_find(g_global.venv, key);
	if (node)
		return (node->value);
	return (NULL);
}
