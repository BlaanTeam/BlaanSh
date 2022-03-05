/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 00:07:28 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 17:07:53 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*expand_tilde(t_node *node)
{
	char	*home;

	home = getvenv("HOME");
	if (home)
		node->val = gc_filter(ft_strjoin(home, node->val + 1), GC_TMP);
	node->token = WORD;
	return (node->next);
}

t_list	*delete_quotes(t_list *tokens)
{
	t_node	*top;
	t_node	*next;

	next = NULL;
	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		next = top->next;
		if (top->token & (DQUOTE | SQUOTE))
			next = del_node(tokens, top)->next;
		top = next;
	}
	return (tokens);
}

t_node	*handle_redirects(t_node *node)
{
	t_node	*right;

	right = get_right(node);
	if (right->token & ~STRING)
		return (node->next);
	if (node->token & DLESS)
	{
		if (right->token == GROUP)
		{
			right->val = right->val_grp->top->val;
			del_front(right->val_grp);
			while (right->val_grp->len)
			{
				right->val = gc_filter(ft_strjoin(right->val,
							right->val_grp->top->val), GC_TMP);
				del_front(right->val_grp);
			}
		}
		right->token = WORD;
	}
	else if (right->token & WILDC)
		right->token = WORD;
	return (right);
}
