/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:24:15 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 18:42:43 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_connectors(t_node *tokp)
{
	t_node	*left;
	t_node	*right;
	t_token	tok;

	tok = tokp->token;
	if (!(tok & CONNECTOR))
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	if (!(left->token & (STRING | CPAR)))
		return (_error(UNEXPECTED_TOK, tokp->val, NULL, 2), false);
	else if ((tok & (FG | BG)) && (right->token & (ENDOFCMD | CPAR)))
		return (true);
	else if (!(right->token & (STRING | OPAR | REDIRECT)))
		return (_error(UNEXPECTED_TOK, right->val, NULL, 2), false);
	return (true);
}

bool	check_parentheses(t_node *tokp)
{
	t_node	*left;
	t_node	*right;
	t_token	tok;

	tok = tokp->token;
	if (tok != OPAR && tok != CPAR)
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	if (tok == OPAR)
	{
		if (!(left->token & (CMDBEGIN | CONNECTOR | OPAR)))
			return (_error(UNEXPECTED_TOK, tokp->val, NULL, 2), false);
		else if (!(right->token & (STRING | OPAR | REDIRECT)))
			return (_error(UNEXPECTED_TOK, right->val, NULL, 2), false);
	}
	else if (tok == CPAR)
	{
		if (!(left->token & (STRING | CPAR | BG | FG)))
			return (_error(UNEXPECTED_TOK, tokp->val, NULL, 2), false);
		else if (!(right->token & (ENDOFCMD | CONNECTOR | CPAR | REDIRECT)))
			return (_error(UNEXPECTED_TOK, right->val, NULL, 2), false);
	}
	return (true);
}

bool	check_redirections(t_node *tokp)
{
	t_node	*right;
	t_token	tok;

	tok = tokp->token;
	if (!(tok & REDIRECT))
		return (true);
	right = get_right(tokp);
	if (!(right->token & STRING))
		return (_error(UNEXPECTED_TOK, right->val, NULL, 2), false);
	return (true);
}

bool	check_matching_quotes_parentheses(t_list *tokens)
{
	t_node	*top;
	int		quotes[2];
	int		par_matching;

	quotes[0] = 0;
	quotes[1] = 0;
	par_matching = 0;
	top = tokens->top;
	while (top->token != ENDOFCMD)
	{
		par_matching += (top->token == OPAR);
		par_matching -= (top->token == CPAR);
		quotes[0] += (top->token == SQUOTE);
		quotes[1] += (top->token == DQUOTE);
		if (par_matching < 0)
			return (_error(UNEXPECTED_TOK, ")", NULL, 2), false);
		top = top->next;
	}
	if (par_matching)
		return (_error(UNEXPECTED_EOF, ")", NULL, 2), false);
	else if (quotes[0] & 1)
		return (_error(UNCLOSED_SQ, NULL, NULL, 2), false);
	else if (quotes[1] & 1)
		return (_error(UNCLOSED_DQ, NULL, NULL, 2), false);
	return (true);
}

bool	validate_syntax(t_list *tokens)
{
	t_node	*tokp;
	bool	check;

	tokp = tokens->top->next;
	while (tokp->token != ENDOFCMD)
	{
		check = (check_unexpected(tokp) && check_connectors(tokp) \
		&& check_parentheses(tokp) && check_redirections(tokp));
		if (!check)
			return (false);
		tokp = tokp->next;
	}
	return (check_matching_quotes_parentheses(tokens));
}
