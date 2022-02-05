/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 12:31:13 by omoussao          #+#    #+#             */
/*   Updated: 2021/11/03 15:29:16 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	len;
	size_t	i;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	len = dest_len;
	if (size <= dest_len)
		return (size + src_len);
	i = 0;
	while (src[i] && len < size - 1)
		dest[len++] = src[i++];
	dest[len] = 0;
	return (dest_len + src_len);
}
