/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:19:43 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 20:13:01 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrput_handler(int sig)
{
	(void)sig;
	if (g_global.is_running)
		return ;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	term_init(void)
{
	struct termios	new_term;

	if (signal(SIGINT, interrput_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR || \
		signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		exit_with_code(EXIT_FAILURE, "signal", false);
	if (tcgetattr(STDIN_FILENO, &new_term) != 0)
		exit_with_code(EXIT_FAILURE, "tcgetattr", false);
	new_term.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0)
		exit_with_code(EXIT_FAILURE, "tcsetattr", false);
}

void	term_restore(void)
{
	struct termios	old_term;

	if (signal(SIGINT, SIG_DFL) == SIG_ERR || \
		signal(SIGQUIT, SIG_DFL) == SIG_ERR || \
		signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		exit_with_code(EXIT_FAILURE, "signal", false);
	if (tcgetattr(STDIN_FILENO, &old_term) != 0)
		exit_with_code(EXIT_FAILURE, "tcgetattr", false);
	old_term.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &old_term) != 0)
		exit_with_code(EXIT_FAILURE, "tcsetattr", false);
}
