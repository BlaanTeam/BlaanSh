/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 17:05:50 by asabani           #+#    #+#             */
/*   Updated: 2022/02/27 20:07:55 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char	*str, char *sep)
{
	static char	*remainder;
	char		*result;
	int			start;
	int			i;

	if (str)
		remainder = str;
	if (!remainder)
		return (NULL);
	i = -1;
	while (remainder[++i])
	{
		if (ft_strchr(sep, remainder[i]) || i == 0)
			start = i;
		if (!ft_strchr(sep, remainder[i]) && ft_strchr(sep, remainder[i + 1]))
		{
			if (start || ft_strchr(sep, remainder[start]))
				start++;
			result = ft_substr(remainder, start, i - start + 1);
			remainder = remainder + i + 1;
			return (result);
		}
	}
	return (NULL);
}
