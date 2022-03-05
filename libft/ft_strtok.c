/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 17:05:50 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 23:29:51 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, char *sep)
{
	static char	*remainder;
	char		*ret;
	int			len;

	if (str)
		remainder = str;
	if (!remainder)
		return (NULL);
	while (*remainder && ft_strchr(sep, *remainder))
		remainder++;
	len = 0;
	while (remainder[len] && !ft_strchr(sep, remainder[len]))
		len++;
	if (!len)
		return (NULL);
	ret = ft_substr(remainder, 0, len);
	remainder += len;
	if (*remainder && ft_strchr(sep, *remainder))
		remainder++;
	return (ret);
}
