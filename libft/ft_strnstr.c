/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 21:29:37 by omoussao          #+#    #+#             */
/*   Updated: 2021/11/04 16:46:48 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *h, const char *n, size_t len)
{
	size_t	i;
	size_t	j;

	if (len == 0 && n[0])
		return (NULL);
	if (!n[0])
		return ((char *)h);
	i = -1;
	while (++i < len && h[i])
	{
		j = -1;
		while (n[++j] && i + j < len)
		{
			if (n[j] != h[i + j])
				break ;
		}
		if (!n[j])
			return ((char *)(h + i));
	}
	return (NULL);
}
