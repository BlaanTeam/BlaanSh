/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:41:47 by asabani           #+#    #+#             */
/*   Updated: 2022/02/10 23:58:34 by asabani          ###   ########.fr       */
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
# include "libft.h"
# define PATH_MAX 1024

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

// tokens
typedef enum e_token
{
	WHITESPACE,
	TK_NEWLINE,
	WORD,
	PATH,
	PIPE,
	AND_IF,
	OR_IF,
	O_PARENTHESESE,
	C_PARENTHESESE,
	EQUAL,
	AMPERSAND,
	SEMICL,
	DSEMICL,
	VAR_EXPANSION,
	TILDE_EXPANSION,
	WILDCARD_EXPANSION,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	LESS,
	DLESS,
	GREAT,
	DGREAT
}	t_token;

typedef struct s_node
{
	t_token			token;
	char			*val;
	struct s_node	*next;
	struct s_node	*prev;
}				t_node;

typedef struct s_list
{
	int		len;
	t_node	*top;
	t_node	*bottom;
}				t_list;

typedef struct s_venv
{
	char			*key;
	char			*value;
	struct s_venv	*next;
}	t_venv;

typedef struct s_global {
	t_gc	*gc;
	int		status;
	bool	is_running;
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

// termios utils
void	term_init(void);
void	term_restore(void);

// env utils
t_venv	*venv_init(char **env);
char	**venv_export_array(t_venv	*venv_head);
void	venv_insert(t_venv **venv_head, char *key, char *value, bool overwrite);
t_venv	*venv_find(t_venv *venv_head, char *key);
bool	venv_remove(t_venv **venv_head, char *key);
void	display_venv(t_venv *venv);
void	display_env(char **env);

// error utils
void	exit_with_code(int status, char *msg, bool silently);
void	alloc_error(void);
void	cmd_error(char *cmd, char *msg, char *extra);

/*
**	doubly linked list utils
*/
t_list	*new_list(void);
void	push_front(t_list *list, t_token token, char *val);
void	push_back(t_list *list, t_token token, char *val);
int		del_front(t_list *list);
int		del_back(t_list *list);
void	list_clear(t_list *list);

t_list	*tokenize(char *line);

// builtin commands
void	cd(char **av, t_venv **venv);
void	pwd(char **av, t_venv **venv);
void	ft_env(char **av, t_venv **venv);
void	unset(char **av, t_venv **venv);
void	echo(char **av);
void	ft_exit(char **av);
int		set_status(int status);
int		check_option(char *cmd, char *arg);

#endif