/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 17:08:16 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 22:22:22 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/* Possible values of node_type */
# define NODE_CMDLST	0
# define NODE_SUBSH		1
# define NODE_PIPE		2
# define NODE_AND		3
# define NODE_OR		4
# define NODE_FG		5
# define NODE_BG		6
# define NODE_REDIR		7

typedef struct s_cmdtree
{
	int	node_type;
}				t_cmdtree;

typedef struct s_subsh
{
	int			node_type;
	t_cmdtree	*cmdtree;
}				t_subsh;

typedef struct s_connector
{
	int			node_type;
	t_cmdtree	*lcmdtree;
	t_cmdtree	*rcmdtree;
}				t_connector;

typedef struct s_cmdlist
{
	int		node_type;
	t_list	*cmdvec;
}				t_cmdlist;

typedef struct s_redir
{
	int			node_type;
	t_token		redir_type;
	int			io_src;
	int			io_dst;
	int			oflag;
	t_node		*filenode;
	t_cmdtree	*cmdtree;
}				t_redir;

// Parser
t_cmdtree	*parser(t_lexer *tokens);

t_cmdtree	*parse_cmdline(t_node **tokp);
t_cmdtree	*parse_block(t_node **tokp);
t_cmdtree	*parse_pipeline(t_node **tokp);
t_cmdtree	*parse_command(t_node **tokp);
t_cmdtree	*parse_cmdlist(t_node **tokp);
t_cmdtree	*parse_redir(t_cmdtree *cmdtree, t_node **tokp);

// Helpers
t_token		current(t_node *tokp);
t_token		scan(t_node **tokp);
int			accept(t_node **tokp, t_token tok);

// Constructors
t_cmdtree	*new_subsh(t_cmdtree *cmdtree);
t_cmdtree	*new_connector(int node_type, t_cmdtree *left, t_cmdtree *right);
t_cmdtree	*new_cmdlist(void);
t_cmdtree	*new_redir(t_cmdtree *cmdtree);

#endif