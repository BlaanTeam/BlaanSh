/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 14:09:45 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/03 18:35:59 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_list	*new_list(void)
{
	t_list	*list;

	list = (t_list *)malloc(sizeof(t_list));
	if (!list)
	{
		perror("malloc");
		exit(1);
	}
	list->len = 0;
	list->top = NULL;
	list->bottom = NULL;
	return (list);
}

void	push_front(t_list *list, t_token token, char *val)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
	{
		perror("malloc");
		exit(1);
	}
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

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
	{
		perror("malloc");
		exit(1);
	}
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
	t_node	*to_free;

	if (list->len == 0)
		return (0);
	to_free = list->top;
	list->top = list->top->next;
	if (list->top)
		list->top->prev = NULL;
	(list->len)--;
	if (to_free->val)
		free(to_free->val);
	free(to_free);
	return (1);
}

int	del_back(t_list *list)
{
	t_node	*to_free;

	if (list->len == 0)
		return (0);
	to_free = list->bottom;
	list->bottom = list->bottom->prev;
	if (list->bottom)
		list->bottom->next = NULL;
	(list->len)--;
	if (to_free->val)
		free(to_free->val);
	free(to_free);
	return (1);
}

void	list_clear(t_list *list)
{
	while (list->len)
		del_front(list);
	free(list);
}