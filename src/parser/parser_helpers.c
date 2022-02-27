/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:57:47 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/27 19:40:46 by omoussao         ###   ########.fr       */
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