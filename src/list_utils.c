/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 14:09:45 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/19 15:10:49 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*new_list(void)
{
	t_list	*list;

	list = (t_list *)gc_filter(malloc(sizeof(t_list)), GC_TMP);
	list->len = 0;
	list->top = NULL;
	list->bottom = NULL;
	return (list);
}

t_node	*new_node(t_token token, char *val)
{
	t_node	*new;

	new = (t_node *)gc_filter(malloc(sizeof(t_node)), GC_TMP);
	new->token = token;
	new->val = val;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

t_node	*push_front(t_list *list, t_token token, char *val)
{
	t_node	*new;

	new = new_node(token, val);
	new->next = list->top;
	if (list->top)
		list->top->prev = new;
	else
		list->bottom = new;
	list->top = new;
	(list->len)++;
	return (new);
}

t_node	*push_back(t_list *list, t_token token, char *val)
{
	t_node	*new;

	new = new_node(token, val);
	new->prev = list->bottom;
	if (list->bottom)
		list->bottom->next = new;
	else
		list->top = new;
	list->bottom = new;
	(list->len)++;
	return (new);
}

t_node	*insert_node(t_list *list, t_node *new, t_node *prev)
{
	if (!list)
		return (NULL);
	(list->len)++;
	new->prev = prev;
	new->next =	prev->next;
	if (!prev || !prev->next)
	{
		if (prev == NULL)
			list->top = new;
		if (prev->next == NULL)
			list->bottom = new;
	}
	else
	{
		prev->next->prev = new;
		prev->next = new;
	}
	return (new);
}

int	del_front(t_list *list)
{
	if (list->len == 0)
		return (0);
	list->top = list->top->next;
	if (list->top)
		list->top->prev = NULL;
	(list->len)--;
	return (1);
}

int	del_back(t_list *list)
{
	if (list->len == 0)
		return (0);
	list->bottom = list->bottom->prev;
	if (list->bottom)
		list->bottom->next = NULL;
	(list->len)--;
	return (1);
}

void	list_clear(t_list *list)
{
	while (list->len)
		del_front(list);
}