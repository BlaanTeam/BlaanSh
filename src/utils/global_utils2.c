/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:07:07 by asabani           #+#    #+#             */
/*   Updated: 2022/02/24 15:14:01 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*gc_filter(void *ptr, t_gc_flag append_flag)
{
	if (!ptr)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	if (!gc_append(g_global.gc, ptr, append_flag))
		exit_with_code(EXIT_FAILURE, "malloc", false);
	return (ptr);
}

char	*ft_strndup(char *str, int n)
{
	char	*ret;

	ret = (char *)malloc(n);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, str, n);
	return (ret);
}

char	*ft_chardup(char c)
{
	char	*ret;

	ret = (char *)malloc(2);
	if (!ret)
		return (NULL);
	ret[0] = c;
	ret[1] = 0;
	return (ret);
}

bool	is_option(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-' && str[i + 1])
		return (true);
	return (false);
}
