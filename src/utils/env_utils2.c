/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 01:22:28 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 15:48:08 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	venv_count(t_venv *venv_head)
{
	int	counter;

	counter = 0;
	while (venv_head)
	{
		if (!(venv_head->eflag & E_EMPTY))
			counter++;
		venv_head = venv_head->next;
	}
	return (counter);
}

char	**venv_export_array(t_venv	*venv_head)
{
	int		i;
	int		len;
	char	*str;
	char	**env;

	i = 0;
	len = venv_count(venv_head);
	env = gc_filter(malloc(sizeof(char *) * (len + 1)), GC_TMP);
	while (venv_head && i < len)
	{	
		if (!(venv_head->eflag & E_EMPTY))
		{
			str = gc_filter(ft_strjoin(venv_head->key, "="), GC_TMP);
			str = gc_filter(ft_strjoin(str, venv_head->value), GC_TMP);
			env[len - i++ - 1] = str;
		}
		venv_head = venv_head->next;
	}
	env[len] = NULL;
	return (env);
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
