/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/03/02 00:46:11 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.gc = NULL, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

int	main(int ac, char **av, char **env)
{
	char		*cmdline;
	t_cmdtree	*tree;

	(void)ac;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	set_program_name(av[0]);
	g_global.venv = venv_init(env);
	while (true)
	{
		term_init();
		cmdline = readline("minishell$ ");
		if (!cmdline)
			break ;
		gc_append(g_global.gc, cmdline, GC_TMP);
		tree = parser(lexer(cmdline));
		g_global.is_running = 1;
		executor(tree);
		g_global.is_running = 0;
		gc_clean(&g_global.gc, GC_TMP);
	}
	exit_with_cleanup();
}
