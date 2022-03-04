/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 00:07:28 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 00:22:58 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
tests:
<< $PATH
<< *$PATH"--HELLO--"
"M"*"~"/Desktop
ls -la HELLO$PATH

execution notes:
- in redirects:
	- access filenode
	if WORD:
		- open file
	if VAR:
		- getvenv (ambiguous errro if NULL or empthy)
	if GROUP:
		- run expand_group()
		- if 
		- open file
		
*/

char	*expand_group(t_list *group)
{
	char	*ret;
	char	*tmp;
	t_node	*top;

	ret = group->top->val;
	del_front(group);
	while (group->len)
	{
		if (group->top->token == VAR)
			tmp = getvenv(group->top->val);
		else
			tmp = group->top->val;
		if (!top || !*tmp)
		{
			del_front(group);
			continue ;
		}
		ret = gc_filter(ft_strjoin(ret, tmp), GC_TMP);
		del_front(group);
	}
	return (ret);
}
