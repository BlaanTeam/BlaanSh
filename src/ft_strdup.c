/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 00:51:09 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 00:51:10 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*copy;
	int		length;

	length = ft_strlen(s1) + 1;
	copy = malloc(length * sizeof(char));
	if (!copy)
		return ((char *)0);
	i = 0;
	while (i < length - 1)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
