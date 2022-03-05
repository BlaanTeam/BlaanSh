/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_destructor_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:31:42 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 01:08:43 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*del_front(t_list *list)
{
	t_node	*ret;

	if (list->len == 0)
		return (NULL);
	ret = list->top;
	list->top = list->top->next;
	if (list->top)
		list->top->prev = NULL;
	(list->len)--;
	return (ret);
}

t_node	*del_back(t_list *list)
{
	t_node	*ret;

	if (list->len == 0)
		return (NULL);
	ret = list->bottom;
	list->bottom = list->bottom->prev;
	if (list->bottom)
		list->bottom->next = NULL;
	(list->len)--;
	return (ret);
}

t_node	*del_node(t_list *list, t_node *node)
{
	if (!node)
		return (NULL);
	if (!node->prev)
		return (del_front(list));
	if (!node->next)
		return (del_back(list));
	(list->len)--;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	return (node);
}

void	list_clear(t_list *list)
{
	while (list->len)
		del_front(list);
}
