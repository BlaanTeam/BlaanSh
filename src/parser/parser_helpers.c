/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:57:47 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 23:45:00 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Current: get current token
t_token	current(t_node *tokp)
{
	return (tokp->token);
}

// Peek: see next token
t_token	peek(t_node	*tokp)
{
	return (tokp->next->token);
}

// Scan: scan current token
t_token	scan(t_node **tokp)
{
	t_token	scanned_token;

	scanned_token = (*tokp)->token;
	*tokp = (*tokp)->next;
	return (scanned_token);
}

// Accept: if current token is @tok scan it
int	accept(t_node **tokp, t_token tok)
{
	if ((*tokp)->token == tok)
	{
		scan(tokp);
		return (1);
	}
	return (0);
}

// Expect: if @expected is not accepted print error
int	expect(t_node **tokp, t_token expected)
{
	if (accept(tokp, expected))
		return (1);
	_error("expect: unexpected token", 158);
	return (0);
}