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

typedef enum e_node_kind
{
	NODE_CMDLST = 0,
	NODE_SUBSH,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_FG,
	NODE_BG,
	NODE_REDIR,
}	t_node_kind;

typedef struct s_cmdtree	t_cmdtree;

typedef struct s_subsh
{
	t_cmdtree	*cmdtree;
}	t_subsh;

typedef struct s_connector
{
	t_cmdtree	*lcmdtree;
	t_cmdtree	*rcmdtree;
}	t_connector;

typedef struct s_cmdlist
{
	t_list	*cmdvec;
}	t_cmdlist;

typedef struct s_redir
{
	t_token		redir_type;
	int			io_src;
	int			io_dst;
	int			oflag;
	t_node		*filenode;
	t_cmdtree	*cmdtree;
}	t_redir;

struct s_cmdtree
{
	t_node_kind	kind;
	union
	{
		t_subsh		subsh;
		t_connector	conn;
		t_cmdlist	cmdlist;
		t_redir		redir;
	}	u;
};

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
t_cmdtree	*new_connector(t_node_kind kind,
				t_cmdtree *left, t_cmdtree *right);
t_cmdtree	*new_cmdlist(void);
t_cmdtree	*new_redir(t_cmdtree *cmdtree);

#endif