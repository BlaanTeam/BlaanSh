/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 16:32:25 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 16:34:44 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minshell.h"

char	*expand_group(t_list *group)
{
	char	*ret;
	char	*tmp;

	ret = "";
	while (group->len)
	{
		if (group->top->token == VAR)
			tmp = getvenv(group->top->val + 1);
		else
			tmp = group->top->val;
		if (!tmp || !*tmp)
		{
			del_front(group);
			continue ;
		}
		ret = gc_filter(ft_strjoin(ret, tmp), GC_TMP);
		del_front(group);
	}
	return (ret);
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
		if (head->token == VAR)
		{
			arr[i] = getvenv(head->val + 1);
			if (!arr[i])
				arr[i] = "";
		}
		else if (head->token == GROUP)
			arr[i] = expand_group(head->val_grp);
		head = head->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
