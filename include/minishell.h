/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 16:41:47 by asabani           #+#    #+#             */
/*   Updated: 2022/02/21 20:45:40 by omoussao         ###   ########.fr       */
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
# define E_GLOBAL 0x1
# define E_LOCAL 0x2
# define E_EMPTY 0x4

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
	CMDBEGIN = 1 << 0,
	ENDOFCMD = 1 << 1,
	WHITESPACE = 1 << 2,
	WORD = 1 << 3,
	PATH =  1 << 4,
	PIPE =  1 << 5,
	AND_IF =  1 << 6,
	OR_IF = 1 << 7,
	O_PARENTHESESE = 1 << 8,
	C_PARENTHESESE = 1 << 9,
	EQUAL = 1 << 10,
	ASSIGNMENT = 1 << 11,
	AMPERSAND = 1 << 12,
	SEMICL = 1 << 13,
	DSEMICL = 1 << 14,
	VAR_EXPANSION = 1 << 15,
	TILDE_EXPANSION = 1 << 16,
	WILDCARD_EXPANSION = 1 << 17,
	SINGLE_QUOTE = 1 << 18,
	DOUBLE_QUOTE = 1 << 19,
	LESS = 1 << 20,
	DLESS = 1 << 21,
	GREAT = 1 << 22,
	DGREAT = 1 << 23,
	STRING = (WORD | PATH | TILDE_EXPANSION | ASSIGNMENT | WILDCARD_EXPANSION | SINGLE_QUOTE | DOUBLE_QUOTE)
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
	short			eflag;
	struct s_venv	*next;
}	t_venv;

typedef struct s_global {
	t_gc	*gc;
	char	*program_name;
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
bool	is_option(char *str);
void	set_program_name(char *path);

// termios utils
void	term_init(void);
void	term_restore(void);

// env utils
t_venv	*venv_init(char **env);
char	**venv_export_array(t_venv	*venv_head);
void	venv_insert(t_venv **venv_head, char *key, char *value, short eflag);
t_venv	*venv_find(t_venv *venv_head, char *key);
bool	venv_remove(t_venv **venv_head, char *key);
void	display_venv(t_venv *venv);
void	display_env(char **env);
char	*getkey(char *var);
char	*getvalue(char *var);

// error utils
void	exit_with_code(int status, char *msg, bool silently);
void	alloc_error(void);
void	cmd_error(char *cmd, char *msg, char *extra);

/*
**	parsing utils
*/
t_list	*new_list(void);
t_node	*new_node(t_token token, char *val);
t_node	*push_front(t_list *list, t_token token, char *val);
t_node	*push_back(t_list *list, t_token token, char *val);
t_node	*insert_node(t_list *list, t_node *new, t_node *prev);
t_node	*concat_nodes(t_list *list, t_node *start, t_node *end);
t_node	*del_front(t_list *list);
t_node	*del_back(t_list *list);
t_node	*del_node(t_list *list, t_node *node);
void	list_clear(t_list *list);

t_list	*tokenize(char *line);
bool	validate_syntax(t_list *tokens);

// builtin commands
void	cd(char **av, t_venv **venv);
void	pwd(char **av, t_venv **venv);
void	ft_env(char **av, t_venv **venv);
void	unset(char **av, t_venv **venv);
void	export(char **av, t_venv **venv);
void	echo(char **av);
void	ft_exit(char **av);
int		set_status(int status);
int		check_option(char *cmd, char *arg);

#endif