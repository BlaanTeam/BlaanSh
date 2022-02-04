/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 00:51:15 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 00:51:19 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	s1_len;
	size_t	s2_len;
	char	*new_str;

	if (!s1 || !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_str = (char *)malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!new_str)
		return ((char *)0);
	i = -1;
	j = 0;
	while (++i < s1_len)
		*(new_str + j++) = *(s1 + i);
	i = -1;
	while (++i < s2_len)
		*(new_str + j++) = *(s2 + i);
	*(new_str + j) = '\0';
	return (new_str);
}
