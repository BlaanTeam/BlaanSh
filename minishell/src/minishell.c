/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 22:13:08 by asabani           #+#    #+#             */
/*   Updated: 2022/02/02 14:11:55 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interrput_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	char	*cmdline;

	(void)ac;
	(void)av;
	(void)env;
	signal(SIGINT, interrput_handler);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		cmdline = readline("minishell$ ");
		if (!cmdline)
			return (0);
		add_history(cmdline);
		printf("%s\n", cmdline);
		free(cmdline);
	}
}
