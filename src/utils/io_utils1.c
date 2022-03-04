/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 20:19:43 by asabani           #+#    #+#             */
/*   Updated: 2022/03/04 20:29:57 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrput_handler(int sig)
{
	(void)sig;
	if (g_global.is_running)
		return ;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_status(1);
}

int	get_tty_fd(void)
{
	int	fileno;

	if (!g_global.ttyname)
		return (-1);
	fileno = open(g_global.ttyname, O_RDONLY);
	if (fileno == -1)
		return (fileno);
	else if (!isatty(fileno))
		return (-1);
	return (fileno);
}

void	term_init(void)
{
	struct termios	new_term;
	int				fileno;

	if (signal(SIGINT, interrput_handler) == SIG_ERR || \
		signal(SIGQUIT, SIG_IGN) == SIG_ERR || \
		signal(SIGTSTP, SIG_IGN) == SIG_ERR)
		_error("signal", strerror(errno), NULL, 1);
	fileno = get_tty_fd();
	if (tcgetattr(fileno, &new_term) != 0)
		return (_error("tcgetattr", strerror(errno), NULL, 1));
	new_term.c_lflag &= ~(ECHOCTL);
	if (tcsetattr(fileno, TCSANOW, &new_term) != 0)
		return (_error("tcsetattr", strerror(errno), NULL, 1));
}

void	term_restore(void)
{
	struct termios	old_term;
	int				fileno;

	if (signal(SIGINT, SIG_DFL) == SIG_ERR || \
		signal(SIGQUIT, SIG_DFL) == SIG_ERR || \
		signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		_error("signal", strerror(errno), NULL, 1);
	fileno = get_tty_fd();
	if (tcgetattr(fileno, &old_term) != 0)
		return (_error("tcgetattr", strerror(errno), NULL, 1));
	old_term.c_lflag |= ECHOCTL;
	if (tcsetattr(fileno, TCSANOW, &old_term) != 0)
		return (_error("tcsetattr", strerror(errno), NULL, 1));
}
