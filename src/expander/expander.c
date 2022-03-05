/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 13:35:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 17:02:58 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*expander(t_list *tokens)
{
	t_node	*top;
	t_node	*next;

	tokens = delete_quotes(tokens);
	tokens = make_groups(tokens);
	tokens = concat_words(tokens);
	next = NULL;
	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		next = top->next;
		if (top->token & REDIRECT)
			next = handle_redirects(top);
		else if (top->token & TILDE)
			next = expand_tilde(top);
		else if (top->token & WILDC)
			next = expand_wildcards(tokens, top);
		else
			next = top->next;
		top = next;
	}
	return (tokens);
}
