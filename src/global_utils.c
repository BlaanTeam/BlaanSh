/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:53:48 by asabani           #+#    #+#             */
/*   Updated: 2022/02/09 17:23:25 by omoussao         ###   ########.fr       */
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
		exit_with_error("malloc");
	gc_append(g_global.gc, ptr, append_flag);
	return (ptr);
}

int	set_status(int status)
{
	return (status * 256);
}

char	*ft_getcwd()
{
	char	*path;

	path = gc_filter(malloc(PATH_MAX), GC_ALL);
	if (!getcwd(path, PATH_MAX))
		g_global.status = set_status(1);
	return (path);
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

/**
 * BASH - manual 
 * A  word  consisting  only  of alphanumeric characters and under‐
 * scores, and beginning with an alphabetic character or an  under‐
 * score.  Also referred to as an identifier.
 * 
 **/
bool	is_identifier(char *identifier)
{
	int	i;

	if (!identifier)
		return (false);
	i = 0;
	if (ft_isalpha(identifier[i]) || identifier[i] == '_')
	{
		while (identifier[++i])
			if (!(ft_isalnum(identifier[i]) || identifier[i] == '_'))
				return (false);
		return (true);
	}
	return (false);
}
