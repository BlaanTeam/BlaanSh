/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 20:34:56 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.gc = NULL, \
						.program_name = NULL, \
						.ttyname = NULL, \
						.status = 0, \
						.is_running = 0};

static void	global_setup(int ac, char **av, char **env)
{
	(void)ac;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	load_ttyname();
	g_global.venv = venv_init(env);
	g_global.program_name = av[0];
}

int	main(int ac, char **av, char **env)
{
	int			io[2];
	char		*prompt;
	char		*cmdline;
	t_cmdtree	*tree;

	global_setup(ac, av, env);
	save_io(io);
	prompt = gc_filter(ft_strjoin(get_basename(av[0]), "-1.0$ "), GC_ALL);
	while (true)
	{
		term_init();
		cmdline = readline(prompt);
		if (!cmdline)
			break ;
		gc_append(g_global.gc, cmdline, GC_TMP);
		g_global.is_running = 1;
		tree = parser(lexer(cmdline));
		executor(tree);
		g_global.is_running = 0;
		gc_clean(&g_global.gc, GC_TMP);
		reset_io(io);
	}
	exit_with_cleanup();
}
