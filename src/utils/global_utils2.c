/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:07:07 by asabani           #+#    #+#             */
/*   Updated: 2022/02/24 15:14:01 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Thin wrappers around libarena that target the two global arenas. They
 * abort the process on OOM (consistent with the previous gc_filter
 * behavior) so call sites don't need a NULL check on every allocation.
 */
static void	*oom_check(void *p)
{
	if (!p)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	return (p);
}

void	*xalloc(size_t n)
{
	return (oom_check(arena_alloc(&g_global.tmp, n)));
}

void	*xalloc_perm(size_t n)
{
	return (oom_check(arena_alloc(&g_global.perm, n)));
}

char	*xstrdup(const char *s)
{
	return (oom_check(arena_strdup(&g_global.tmp, s)));
}

char	*xstrdup_perm(const char *s)
{
	return (oom_check(arena_strdup(&g_global.perm, s)));
}

char	*xstrndup(const char *s, size_t n)
{
	return (oom_check(arena_strndup(&g_global.tmp, s, n)));
}

char	*xstrndup_perm(const char *s, size_t n)
{
	return (oom_check(arena_strndup(&g_global.perm, s, n)));
}

char	*xstrjoin(const char *a, const char *b)
{
	return (oom_check(arena_strjoin(&g_global.tmp, a, b)));
}

char	*xstrjoin_perm(const char *a, const char *b)
{
	return (oom_check(arena_strjoin(&g_global.perm, a, b)));
}

char	*xchardup(char c)
{
	return (oom_check(arena_chardup(&g_global.tmp, c)));
}

char	*xitoa(int n)
{
	return (oom_check(arena_itoa(&g_global.tmp, n)));
}

char	*xitoa_perm(int n)
{
	return (oom_check(arena_itoa(&g_global.perm, n)));
}

void	child_exit(int status)
{
	arena_destroy(&g_global.tmp);
	arena_destroy(&g_global.perm);
	exit(status);
}

bool	is_option(char *str)
{
	int	i;

	i = 0;
	if (str[i] && str[i] == '-' && str[i + 1])
		return (true);
	return (false);
}
