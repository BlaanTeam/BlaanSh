/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 19:42:58 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/26 23:42:54 by asabani          ###   ########.fr       */
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
	WHITESPACE = 1 << 2,
	WORD = 1 << 3,
	PATH = 1 << 4,
	PIPE = 1 << 5,
	AND_IF = 1 << 6,
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
	STRING = (WORD | PATH | TILDE_EXPANSION | \
	WILDCARD_EXPANSION | VAR_EXPANSION | SINGLE_QUOTE | DOUBLE_QUOTE),
	REDIRECT = (LESS | DLESS | GREAT | DGREAT)
}	t_token;

typedef struct s_node
{
	t_token			token;
	char			*val;
	struct s_node	*next;
	struct s_node	*prev;
}				t_node;

typedef t_node	t_lexeme;

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
char	**list_export_array(t_list *list);

#endif