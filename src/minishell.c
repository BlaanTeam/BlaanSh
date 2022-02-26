/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/26 16:05:09 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//* names of tokens (for debuging)
const char *token_names[] = {
	"CMDBEGIN",
	"ENDOFCMD",
	"WSPACE",
	"WORD",
	"PATH",
	"PIPE",
	"AND",
	"OR",
	"OPAR",
	"CPAR",
	"BG",
	"FG",
	"DSEMI",
	"VAR",
	"TILDE",
	"WILDC",
	"SQUOTE",
	"DQUOTE",
	"LESS",
	"DLESS",
	"GREAT",
	"DGREAT",
};

t_global	g_global = {.gc = 0, \
						.program_name = NULL, \
						.status = 0, \
						.is_running = 0};

#include <string.h>
#include <math.h>
void	disp(t_node *top)
{
	while (top)
	{
		printf("tok = %-40s val = %s\n", token_names[(int)log2(top->token)], top->val);
		top = top->next;
	}
}

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
			printf("\n");
		cmdline = readline("minishell$ ");
		if (!cmdline)
			break ;
		av = ft_split(cmdline, ' ');
		if (av[0])
		{
			if (strcmp(av[0], "pwd") == 0)
				pwd(av + 1, &g_global.venv);
			else if (strcmp(av[0], "cd") == 0)
				cd(av + 1, &g_global.venv);
			else if (strcmp(av[0], "echo") == 0)
				echo(av +1);
			else if (strcmp(av[0], "exit") == 0)
				ft_exit(av + 1);
			else if (strcmp(av[0], "unset") == 0)
				unset(av + 1, &g_global.venv);
			else if (strcmp(av[0], "env") == 0)
				ft_env(av + 1, &g_global.venv);
			else if (strcmp(av[0], "$?") == 0)
				printf("%d\n", get_status());
			else if (strcmp(av[0], "export") == 0)
				export(av + 1, &g_global.venv);
		}
		gc_append(g_global.gc, cmdline, GC_TMP);
		tokens = lexer(cmdline);
		if (tokens)
			disp(tokens->top);
		gc_clean(&g_global.gc, GC_TMP);
	}
	exit_with_cleanup();
}