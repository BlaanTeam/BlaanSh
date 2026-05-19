/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lookup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 18:43:43 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 00:42:20 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Walk the list with a pointer-to-pointer so the head and middle cases
 * collapse into the same code path — no separate prev tracking, and
 * the analyzer can't claim `prev` is read uninitialized.
 */
bool	venv_remove(t_venv **venv_head, char *key)
{
	size_t	keylen;

	keylen = ft_strlen(key) + 1;
	while (*venv_head)
	{
		if (ft_memcmp((*venv_head)->key, key, keylen) == 0)
		{
			*venv_head = (*venv_head)->next;
			return (true);
		}
		venv_head = &(*venv_head)->next;
	}
	return (false);
}

char	*getvenv(char *key)
{
	t_venv	*node;

	if (!key)
		return (NULL);
	if (ft_memcmp(key, "?", 2) == 0)
		return (xitoa(get_status()));
	else if (ft_memcmp(key, "0", 2) == 0)
		return (g_global.program_name);
	node = venv_find(g_global.venv, key);
	if (node)
		return (node->value);
	return (NULL);
}
