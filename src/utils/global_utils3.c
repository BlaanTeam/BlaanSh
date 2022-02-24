/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 23:42:02 by asabani           #+#    #+#             */
/*   Updated: 2022/02/12 23:50:59 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_program_name(char *path)
{
	int	len;

	len = ft_strlen(path);
	while (len && path[len] != '/')
		len--;
	path = path + len + 1;
	g_global.program_name = path;
}
