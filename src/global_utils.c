/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:53:48 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 17:55:41 by asabani          ###   ########.fr       */
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
