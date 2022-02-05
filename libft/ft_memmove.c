/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/01 19:56:58 by omoussao          #+#    #+#             */
/*   Updated: 2021/11/02 15:59:02 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = -1;
	if (dest > src)
		while (n--)
			*(char *)(dest + n) = *(char *)(src + n);
	else if (dest < src)
		while (++i < n)
			*(char *)(dest + i) = *(char *)(src + i);
	return (dest);
}
