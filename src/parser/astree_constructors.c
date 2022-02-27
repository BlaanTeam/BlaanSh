/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astree_constructors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 16:49:07 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 19:36:39 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Subshell node constructor
t_cmdtree	*new_subsh(t_cmdtree *subtree)
{
	t_subsh	*subshell;

	subshell = gc_filter(malloc(sizeof(t_subsh)), GC_TMP);
	subshell->node_type = NODE_SUBSH;
	subshell->cmdtree = subtree;
	return ((t_cmdtree *)subshell);
}

// Connector node constructor
// Connector might be: AND, OR, PIPE, FG, BG
t_cmdtree	*new_connector(int node_type, t_cmdtree *left, t_cmdtree *right)
{
	t_connector	*connector;

	connector  = gc_filter(malloc(sizeof(t_connector)), GC_TMP);
	connector->node_type = node_type;
	connector->lcmdtree = left;
	connector->rcmdtree = right;
	return ((t_cmdtree *)connector);
}

// Command list node constructor
t_cmdtree	*new_cmdlist(t_list *cmdvec)
{
	t_cmdlist	*cmdlist;

	cmdlist = gc_filter(malloc(sizeof(t_cmdlist)), GC_TMP);
	cmdlist->node_type = NODE_CMDLST;
	cmdlist->cmdvec = new_list();
	cmdlist->cmdargs = NULL;
	return ((t_cmdtree *)cmdlist);
}

// Redirect node constructor
// Fill only node_type, and cmdtree pointer
// Other data will depend on parse_redir
t_cmdtree	*new_redir(t_cmdtree *cmdtree)
{
	t_redir	*redir;

	redir = gc_filter(malloc(sizeof(t_redir)), GC_TMP);
	redir->node_type = NODE_REDIR;
	redir->cmdtree = cmdtree;
	return ((t_cmdtree *)redir);
}