/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 22:42:26 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global = {.gc = 0, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;
	t_list	*tokens;

	(void)ac;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	set_program_name(av[0]);
	g_global.venv = venv_init(env);
	while (true)
	{
		term_init();
		if (WIFSIGNALED(g_global.status))
			ft_putstr_fd("\n", STDOUT_FILENO);
		cmdline = readline("minishell$ ");
		if (!cmdline)
			break ;
		gc_append(g_global.gc, cmdline, GC_TMP);
		tokens = lexer(cmdline);
		if (tokens)
		{
			g_global.is_running = 1;
			av = list_export_array(tokens);
			av[tokens->len-1] = NULL;
			exec_cmd(av[0], av);
			wait(&g_global.status);
			g_global.is_running = 0;
			// disp(tokens->top);
		}
		gc_clean(&g_global.gc, GC_TMP);
	}
	exit_with_cleanup();
}