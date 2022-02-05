/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 19:27:06 by omoussao          #+#    #+#             */
/*   Updated: 2021/11/02 19:32:58 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t	i;
	char	*last_occ;

	last_occ = NULL;
	i = 0;
	while (str[i])
		if (str[i++] == (char) c)
			last_occ = (char *)str + i - 1;
	if (!c)
		return ((char *)str + i);
	return (last_occ);
}
