/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/12 23:56:46 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//* names of tokens (for debuging)
const char *token_names[] = {
	"WHITESPACE",
	"TK_NEWLINE",
	"WORD",
	"PATH",
	"PIPE",
	"AND_IF",
	"OR_IF",
	"O_PARENTHESESE",
	"C_PARENTHESESE",
	"EQUAL",
	"AMPERSAND",
	"SEMICL",
	"DSEMICL",
	"VAR_EXPANSION",
	"TILDE_EXPANSION",
	"WILDCARD_EXPANSION",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"LESS",
	"DLESS",
	"GREAT",
	"DGREAT"
};

t_global	g_global = {.gc = 0, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;
	t_venv	*venv;
	t_list	*tokens;

	(void)ac;
	g_global.gc = gc_init();
	if (!g_global.gc)
		exit_with_code(EXIT_FAILURE, "malloc", false);
	set_program_name(av[0]);
	venv = venv_init(env);
	while (true)
	{
		term_init();
		if (WIFSIGNALED(g_global.status))
			printf("\n");
		cmdline = readline("minishell$ ");
		if (!cmdline)
			break ;
		av = ft_split(cmdline, ' ');
		if (av[0])
		{
			if (strcmp(av[0], "pwd") == 0)
				pwd(av + 1, &venv);
			else if (strcmp(av[0], "cd") == 0)
				cd(av + 1, &venv);
			else if (strcmp(av[0], "echo") == 0)
				echo(av +1);
			else if (strcmp(av[0], "exit") == 0)
				ft_exit(av + 1);
			else if (strcmp(av[0], "unset") == 0)
				unset(av + 1, &venv);
			else if (strcmp(av[0], "env") == 0)
				ft_env(av + 1, &venv);
			else if (strcmp(av[0], "$?") == 0)
				printf("%d\n", WEXITSTATUS(g_global.status));
			else if (strcmp(av[0], "export") == 0)
				export(av + 1, &venv);
		}
		gc_append(g_global.gc, cmdline, GC_TMP);
		add_history(cmdline);

		//* debug: show tokens
		tokens = tokenize(cmdline);
		t_node	*top = tokens->top;
		while (top)
		{
			printf("tok = %-40s val = %-20s len = %zu\n", token_names[top->token], top->val, top->val? ft_strlen(top->val): 0);
			top = top->next;
		}
		// list_clear(tokens);
		gc_clean(&g_global.gc, GC_TMP);
		//* end debug
	}
	exit_with_cleanup();
}
