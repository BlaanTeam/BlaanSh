/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astree_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 16:49:07 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 22:19:51 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmdtree	*alloc_cmdtree(t_node_kind kind)
{
	t_cmdtree	*node;

	node = gc_filter(malloc(sizeof(t_cmdtree)), GC_TMP);
	node->kind = kind;
	return (node);
}

t_cmdtree	*new_subsh(t_cmdtree *subtree)
{
	t_cmdtree	*node;

	node = alloc_cmdtree(NODE_SUBSH);
	node->u.subsh.cmdtree = subtree;
	return (node);
}

// Connector covers PIPE / AND / OR / FG / BG — same shape, different kind.
t_cmdtree	*new_connector(t_node_kind kind,
	t_cmdtree *left, t_cmdtree *right)
{
	t_cmdtree	*node;

	node = alloc_cmdtree(kind);
	node->u.conn.lcmdtree = left;
	node->u.conn.rcmdtree = right;
	return (node);
}

t_cmdtree	*new_cmdlist(void)
{
	t_cmdtree	*node;

	node = alloc_cmdtree(NODE_CMDLST);
	node->u.cmdlist.cmdvec = new_list();
	return (node);
}

// Redirect node — parse_redir fills in redir_type / io_src / oflag / filenode.
t_cmdtree	*new_redir(t_cmdtree *cmdtree)
{
	t_cmdtree	*node;

	node = alloc_cmdtree(NODE_REDIR);
	node->u.redir.cmdtree = cmdtree;
	node->u.redir.filenode = NULL;
	return (node);
}
