/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_destructor_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:31:42 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/26 23:44:11 by asabani          ###   ########.fr       */
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

char	**list_export_array(t_list *list)
{	
	int		i;
	char	**arr;
	t_node	*head;

	if (!list)
		return (NULL);
	arr = (char **)gc_filter(malloc(sizeof(char *) * (list->len + 1)), GC_TMP);
	head = list->top;
	i = 0;
	while (head)
	{
		arr[i] = head->val;
		head = head->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
