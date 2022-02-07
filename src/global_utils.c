/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:53:48 by asabani           #+#    #+#             */
/*   Updated: 2022/02/07 14:21:48 by omoussao         ###   ########.fr       */
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

char	*ft_getcwd(void)
{
	static char	path[PATH_MAX];

	getcwd(path, PATH_MAX);
	return (path);
}

char	*ft_strndup(char *str, int n)
{
	char	*ret;

	ret = (char *)ft_malloc(n);
	ft_strlcpy(ret, str, n);
	return (ret);
}