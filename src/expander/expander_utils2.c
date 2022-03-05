/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 16:32:25 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 16:32:39 by omoussao         ###   ########.fr       */
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
