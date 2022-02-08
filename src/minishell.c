/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/08 16:04:10 by omoussao         ###   ########.fr       */
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
						.status = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;
	t_list	*tokens;

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
		list_clear(tokens);
		//* end debug
	}
	gc_clean(&g_global.gc, GC_ALL);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
