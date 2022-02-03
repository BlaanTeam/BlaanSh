/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/03 22:36:42 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.gc = 0, \
						.status = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;

	(void)ac;
	(void)av;
	(void)env;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_error("malloc");
	while (true)
	{
		term_init();
		cmdline = readline("minishell$ ");
		term_restore();
		if (!cmdline)
			break ;
		gc_append(g_global.gc, cmdline);
	}
	gc_clean(g_global.gc);
	return (EXIT_SUCCESS);
}
