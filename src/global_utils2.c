/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:07:07 by asabani           #+#    #+#             */
/*   Updated: 2022/02/11 19:05:49 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_malloc(size_t len)
{
	void	*block;

	block = malloc(len);
	if (!block)
		alloc_error();
	return (block);
}

void	*gc_filter(void *ptr, t_gc_flag append_flag)
{
	if (!ptr)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	gc_append(g_global.gc, ptr, append_flag);
	return (ptr);
}

char	*ft_strndup(char *str, int n)
{
	char	*ret;

	ret = (char *)ft_malloc(n);
	ft_strlcpy(ret, str, n);
	return (ret);
}

char	*ft_chardup(char c)
{
	char	*ret;

	ret = (char *)ft_malloc(2);
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
