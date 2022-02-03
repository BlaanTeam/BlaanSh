/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:19:43 by asabani           #+#    #+#             */
/*   Updated: 2022/02/03 20:38:48 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrput_handler(int sig)
{
	(void)sig;
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
		exit_with_error("signal");
	if (tcgetattr(STDIN_FILENO, &new_term) != 0)
		exit_with_error("tcgetattr");
	new_term.c_lflag &= ~(ECHOCTL);
	new_term.c_cc[VQUIT] = 255;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0)
		exit_with_error("tcsetattr");
}

void	term_restore(void)
{
	struct termios	old_term;

	if (signal(SIGINT, SIG_DFL) == SIG_ERR || \
		signal(SIGQUIT, SIG_DFL) == SIG_ERR || \
		signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		exit_with_error("signal");
	if (tcgetattr(STDIN_FILENO, &old_term) != 0)
		exit_with_error("tcgetattr");
	old_term.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &old_term) != 0)
		exit_with_error("tcsetattr");
}
