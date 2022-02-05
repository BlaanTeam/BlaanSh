/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 17:59:44 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char *tokens[] = {
	"WHITESPACE",
	"WORD",
	"PATH",
	"PIPE",
	"AND",
	"OR",
	"PARENTHESESE",
	"ASSIGNMENT",
	"EXPANSION",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"WRITE_MODE",
	"APPEND_MODE",
	"HERE_DOC",
	"READ_MODE"
};

t_global	g_global = {.gc = 0, \
						.status = 0};

int	main(int ac, char **av, char **env)
{
	char	*cmdline;
	t_list	*cmdlist;

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
		add_history(cmdline);
		cmdlist = tokenize(cmdline);
		t_node	*top = cmdlist->top;
		while (top) {
			printf("tok = %-40s val = %-20s len = %zu\n", tokens[top->token], top->val, top->val? ft_strlen(top->val): 1);
			top = top->next;
		}
		list_clear(cmdlist);
	}
	gc_clean(g_global.gc);
	return (EXIT_SUCCESS);
}
