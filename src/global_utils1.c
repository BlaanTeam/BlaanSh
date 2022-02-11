/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:53:48 by asabani           #+#    #+#             */
/*   Updated: 2022/02/11 15:43:27 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_status(int status)
{
	return (status * 256);
}

char	*ft_getcwd(void)
{
	char	*path;

	errno = 0;
	path = gc_filter(malloc(PATH_MAX), GC_ALL);
	if (!getcwd(path, PATH_MAX))
		g_global.status = set_status(1);
	return (path);
}

/**
 * BASH - manual 
 * A  word  consisting  only  of alphanumeric characters and under‐
 * scores, and beginning with an alphabetic character or an  under‐
 * score.  Also referred to as an identifier.
 * 
 **/
bool	is_identifier(char *identifier)
{
	int	i;

	if (!identifier)
		return (false);
	i = 0;
	if (ft_isalpha(identifier[i]) || identifier[i] == '_')
	{
		while (identifier[++i])
			if (!(ft_isalnum(identifier[i]) || identifier[i] == '_'))
				return (false);
		return (true);
	}
	return (false);
}

bool	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (!ft_isdigit(str[i + 1]))
			return (false);
		i++;
	}
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (false);
	return (true);
}

void	exit_with_cleanup(void)
{
	gc_clean(&g_global.gc, GC_DESTROY_SELF);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
