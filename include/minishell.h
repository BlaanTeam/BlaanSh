/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:41:47 by asabani           #+#    #+#             */
/*   Updated: 2022/02/05 01:27:30 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "readline/readline.h"
# include "readline/history.h"
# include <errno.h>
# include <sys/types.h>
# include <termios.h>
# include <signal.h>
# include "gc.h"

/*
grammer rules :
	cmdline:  (pipeline [io_modifier]) | comment | varible_assignment | WHITESPACE
	pipeline : pipeline [PIPE cmd_and_args] | cmd_and_args | EMPTY
	cmd_and_args : WHITESPACE (cmd_name | cmd_path) cmd_arg*
	cmd_name : WORD
	cmd_path : PATH
	cmd_arg: WORD
	comment : HASH TEXT
*/

typedef struct s_venv
{
	char			*key;
	char			*value;
	struct s_venv	*next;
}	t_venv;

typedef struct s_global {
	t_gc	*gc;
	int		status;
}	t_global;

// declare global variable
extern t_global	g_global;

// global utils
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);

// termios utils
void	term_init(void);
void	term_restore(void);

// env utils
t_venv	*venv_init(char **env);
char	**venv_export_array(t_venv	*venv_head);
void	display_venv(t_venv *venv);
void	display_env(char **env);

// error utils
void	exit_with_error(char *msg);
void	alloc_error(void);

#endif