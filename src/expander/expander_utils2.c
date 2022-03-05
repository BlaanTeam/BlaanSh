/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 16:32:25 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 17:06:19 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concat_group(t_node *node)
{
	t_node	*top;

	top = node->val_grp->top;
	if (top->token != WORD)
		return ;
	node->val = top->val;
	top = top->next;
	while (top)
	{
		if (!(top->token & (WORD | WILDC | TILDE)))
			return ;
		node->val = gc_filter(ft_strjoin(node->val, top->val), GC_TMP);
		top = top->next;
	}
	node->token = WORD;
}

t_list	*concat_words(t_list *tokens)
{
	t_node	*top;

	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token == GROUP)
			concat_group(top);
		top = top->next;
	}
	return (tokens);
}

t_list	*make_groups(t_list *tokens)
{
	t_node	*top;

	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token & STRING)
		{
			while (top->next->token & STRING)
			{
				if (top->token != GROUP)
				{
					top->val_grp = new_list();
					push_back(top->val_grp, top->token, top->val);
					top->token = GROUP;
					continue ;
				}
				push_back(top->val_grp, top->next->token, top->next->val);
				del_node(tokens, top->next);
			}
		}
		if (top->token == WSPACE)
			del_node(tokens, top);
		top = top->next;
	}
	return (tokens);
}

// Expander utils used in execution
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
