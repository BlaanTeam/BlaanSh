/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/03/03 00:19:05 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.gc = NULL, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

int	main(int ac, char **av, char **env)
{
	int			io[2];
	char		*cmdline;
	t_cmdtree	*tree;

	(void)ac;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	set_program_name(av[0]);
	g_global.venv = venv_init(env);
	io[0] = dup(STDIN_FILENO);
	io[1] = dup(STDOUT_FILENO);
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
		// display_tree(tree, 0);
		g_global.is_running = 0;
		gc_clean(&g_global.gc, GC_TMP);
		dup2(io[0], STDIN_FILENO);
		dup2(io[1], STDOUT_FILENO);
	}
	exit_with_cleanup();
}
