/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 18:27:12 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 18:30:39 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*get_right(t_node *tokp)
{
	t_node	*right;

	right = tokp->next;
	while (right->token == WSPACE)
		right = right->next;
	return (right);
}

t_node	*get_left(t_node *tokp)
{
	t_node	*left;

	left = tokp->prev;
	while (left->token == WSPACE)
		left = left->prev;
	return (left);
}

bool	check_unexpected(t_node *tokp)
{
	t_token	tok;

	tok = tokp->token;
	if (tok & DSEMI)
		return (_error(UNEXPECTED_TOK, tokp->val, NULL, 2), false);
	return (true);
}
