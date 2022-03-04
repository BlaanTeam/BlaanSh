/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 17:53:48 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 19:25:57 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getcwd(void)
{
	char	path[PATH_MAX];

	errno = 0;
	if (!getcwd(path, PATH_MAX))
		set_status(1);
	return (gc_filter(ft_strdup(path), GC_ALL));
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
	term_restore();
	gc_clean(&g_global.gc, GC_DESTROY_SELF);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
