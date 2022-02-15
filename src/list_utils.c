/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 14:09:45 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/15 15:49:51 by omoussao         ###   ########.fr       */
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

void	push_front(t_list *list, t_token token, char *val)
{
	t_node	*new;

	new = (t_node *)gc_filter(malloc(sizeof(t_node)), GC_TMP);
	new->token = token;
	new->val = val;
	new->next = list->top;
	new->prev = NULL;
	if (list->top)
		list->top->prev = new;
	else
		list->bottom = new;
	list->top = new;
	(list->len)++;
}

void	push_back(t_list *list, t_token token, char *val)
{
	t_node	*new;

	new = (t_node *)gc_filter(malloc(sizeof(t_node)), GC_TMP);
	new->token = token;
	new->val = val;
	new->prev = list->bottom;
	new->next = NULL;
	if (list->bottom)
		list->bottom->next = new;
	else
		list->top = new;
	list->bottom = new;
	(list->len)++;
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