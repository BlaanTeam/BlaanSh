/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 19:42:58 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 16:15:46 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// tokens
typedef enum e_token
{
	CMDBEGIN = 1 << 0,
	ENDOFCMD = 1 << 1,
	WSPACE = 1 << 2,
	WORD = 1 << 3,
	PIPE = 1 << 4,
	AND = 1 << 5,
	OR = 1 << 6,
	OPAR = 1 << 7,
	CPAR = 1 << 8,
	BG = 1 << 9,
	FG = 1 << 10,
	DSEMI = 1 << 11,
	VAR = 1 << 12,
	TILDE = 1 << 13,
	WILDC = 1 << 14,
	SQUOTE = 1 << 15,
	DQUOTE = 1 << 16,
	LESS = 1 << 17,
	DLESS = 1 << 18,
	GREAT = 1 << 19,
	DGREAT = 1 << 20,
	GROUP = 1 << 21,
	STRING = (GROUP | WORD | TILDE | WILDC | \
	VAR | SQUOTE | DQUOTE),
	REDIRECT = (LESS | DLESS | GREAT | DGREAT),
	CONNECTOR = (AND | OR | PIPE | FG | BG)
}	t_token;

typedef struct s_node
{
	t_token			token;
	char			*val;
	struct s_list	*val_grp;
	struct s_node	*next;
	struct s_node	*prev;
}				t_node;

typedef struct s_list
{
	int		len;
	t_node	*top;
	t_node	*bottom;
}				t_list;

typedef t_list	t_lexer;

// lexer function
t_lexer	*lexer(char *cmdline);

// lexer utils
t_list	*tokenizer(char *line);
bool	validate_syntax(t_list *tokens);
t_list	*expander(t_list *tokens);

// lexer helpers
t_node	*get_right(t_node *tokp);
t_node	*get_left(t_node *tokp);

// list constructors
t_list	*new_list(void);
t_node	*new_node(t_token token, char *val);
t_node	*push_front(t_list *list, t_token token, char *val);
t_node	*push_back(t_list *list, t_token token, char *val);
t_node	*insert_node(t_list *list, t_node *new, t_node *prev);
t_node	*concat_nodes(t_list *list, t_node *start, t_node *end);

// list destructors
t_node	*del_front(t_list *list);
t_node	*del_back(t_list *list);
t_node	*del_node(t_list *list, t_node *node);
void	list_clear(t_list *list);

#endif