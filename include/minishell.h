/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:41:47 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 00:49:27 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>

# include <unistd.h>
# include <termios.h>
# include <signal.h>
# include <dirent.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>

# include <paths.h>
# include <limits.h>

// Dependencies: GNU Readline
# include "readline/readline.h"
# include "readline/history.h"

// Local header files
# include "gc.h"
# include "libft.h"
# include "lexer.h"
# include "parser.h"
# include "executor.h"

# define E_GLOBAL 0x1
# define E_LOCAL 0x2
# define E_EMPTY 0x4
# define READ_END 0
# define WRITE_END 1

typedef struct s_venv
{
	char			*key;
	char			*value;
	short			eflag;
	struct s_venv	*next;
}	t_venv;

typedef struct s_global {
	t_gc	*gc;
	t_venv	*venv;
	char	*program_name;
	int		status;
	bool	is_running;
	char	*ttyname;
}				t_global;

// declare global variable
extern t_global	g_global;

// global utils
void	*gc_filter(void *ptr, t_gc_flag append_flag);
void	*ft_malloc(size_t len);
char	*ft_getcwd(void);
char	*ft_strndup(char *str, int n);
char	*ft_chardup(char c);
bool	is_identifier(char *identifier);
bool	is_numeric(char *str);
void	exit_with_cleanup(void);
bool	is_option(char *str);
char	*get_basename(char *path);
void	set_status(int status);
int		get_status(void);
void	close_pipe(int fds[2]);
void	ft_dup2(int dest, int src);
pid_t	ft_fork(void);
int		ft_pipe(int fildes[2]);
void	check_status(void);

// termios utils
void	term_init(void);
void	term_restore(void);
void	load_ttyname(void);
void	save_io(int io[2]);
void	reset_io(int io[2]);

// env utils
t_venv	*venv_init(char **env);
char	**venv_export_array(t_venv	*venv_head);
void	venv_insert(t_venv **venv_head, char *key, char *value, short eflag);
t_venv	*venv_find(t_venv *venv_head, char *key);
bool	venv_remove(t_venv **venv_head, char *key);
char	*getkey(char *var);
char	*getvalue(char *var);
char	*getvenv(char *key);

// error utils
void	exit_with_code(int status, char *msg, bool silently);
void	alloc_error(void);
void	_error(char *title, char *msg, char *extra, int status);

// builtin commands
void	cd(char **av, t_venv **venv);
void	pwd(char **av, t_venv **venv);
void	ft_env(char **av, t_venv **venv);
void	unset(char **av, t_venv **venv);
void	export(char **av, t_venv **venv);
void	echo(char **av);
void	ft_exit(char **av);
int		check_option(char *cmd, char *arg);

// display utils (for debugging)
void	display_tokens(t_node *top);
void	display_venv(t_venv *venv);
void	display_env(char **env);
void	display_tree(t_cmdtree *tree, int ident_level);

#endif