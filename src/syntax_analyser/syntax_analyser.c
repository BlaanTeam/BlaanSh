/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:24:15 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/24 19:33:17 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
grammer rules :
	
	! I-GRAMMAR:
	cmdline := pipeline | pipeline AND_IF cmdline | pipeline OR_IF cmdline

	pipeline := command | command PIPE pipeline
	
	command := [assignment] simple-command [arguments] [redirection:(*1)] | assignment

	simple-command := WORD | PATH
	
	arguments := WORD | WORD arguments
	
	redirection := LESS filename | DLESS delimiter | GREAT filename | DGREAT filename
	
	assignment := WORD [PLUS] EQUAL [WORD]

	(*1): could also be in any place before or after the simple-command or between the arguments

	! II-SYNTAX:
	* UNEXPECTED TOKENS:
		- `;' | `;;' | `&'

	* AND_IF, OR_IF, PIPE:
		- left: [WHITESPACE] (STRING | CPAR)
		- right: [WHITESPACE] (STRING | REDIRECT | OPAR)

	* OPAR "(":
		- left: CMDBEGIN | [WHITESPACE] (AND_IF | OR_IF | PIPE | OPAR)
		- right: [WHITESPACE] (STRING | REDIRECT | OPAR)

	* CPAR ")":
		- left: [WHITESPACE] (STRING | CPAR)
		- right: [WHITESPACE] (AND_IF | OR_IF | PIPE | CPAR | ENDOFCMD)

	* REDIRECT:
		- right: [WHITESPACE] STRING

	* PARENTHESES MATCHING AND QUOTING:
		- inside each pair parentheses sould not be an empthy command
		- every open parentheses has to have a matching closing parentheses
		- every single/double quote have to be closed
*/

t_node	*get_right(t_node *tokp)
{
	t_node	*right;

	right = tokp->next;
	while (right->token == WHITESPACE)
		right = right->next;
	return (right);
}

t_node	*get_left(t_node *tokp)
{
	t_node	*left;

	left = tokp->prev;
	while (left->token == WHITESPACE)
		left = left->prev;
	return (left);
}

bool	check_unexpected(t_node *tokp)
{
	t_token	tok;

	tok = tokp->token;
	// if (tok == AMPERSAND || tok == SEMICL || tok == DSEMICL)
	if (tok & (AMPERSAND | SEMICL | DSEMICL))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
		return (false);
	}
	return (true);
}

bool	check_statements(t_node *tokp)
{
	t_node	*left;
	t_node	*right;
	t_token	tok;

	tok = tokp->token;
	if (!(tok & (PIPE | OR_IF | AND_IF)))
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	if (!(left->token & (STRING | C_PARENTHESESE)))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
		return (false);
	}
	else if (!(right->token & (STRING | O_PARENTHESESE | REDIRECT)))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
		return (false);
	}
	return (true);
}

bool	check_parentheses(t_node *tokp)
{
	t_node	*left;
	t_node	*right;
	t_token	tok;

	tok = tokp->token;
	if (tok != O_PARENTHESESE && tok != C_PARENTHESESE)
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	if (tok == O_PARENTHESESE)
	{
		if (!(left->token & (CMDBEGIN | AND_IF | OR_IF | PIPE | O_PARENTHESESE)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		else if (!(right->token & (STRING | O_PARENTHESESE | REDIRECT)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
			return (false);
		}
	}
	else if (tok == C_PARENTHESESE)
	{
		if (!(left->token & (STRING | C_PARENTHESESE)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		else if (!(right->token & (ENDOFCMD | AND_IF | OR_IF | PIPE | C_PARENTHESESE | REDIRECT)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
			return (false);
		}
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
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
		return (false);
	}
	else if (right->token == VAR_EXPANSION && (!getvenv(right->val) || !*getvenv(right->val)))
	{
		fprintf(stderr, "minishell: $%s: ambiguous redirect\n", right->val);
		return (false);
	}
	return (true);
}

bool	check_matching_quotes_parentheses(t_list *tokens)
{
	t_node	*top;
	int		dquotes;
	int		squotes;
	int		par_matching;

	dquotes = 0;
	squotes = 0;
	par_matching = 0;
	top = tokens->top;
	while (top->token != ENDOFCMD)
	{
		par_matching += (top->token == O_PARENTHESESE);
		par_matching -= (top->token == C_PARENTHESESE);
		dquotes += (top->token == DOUBLE_QUOTE);
		squotes += (top->token == SINGLE_QUOTE);
		if (par_matching < 0)
			return (fprintf(stderr, "minishell: syntax error near unexpected token `)'\n"), false);
		top = top->next;
	}
	if (par_matching)
		fprintf(stderr, "minishell: unexpected EOF while looking for matching `)'\n");
	else if (dquotes & 1)
		fprintf(stderr, "minishell: unclosed double quotes\n");
	else if (squotes & 1)
		fprintf(stderr, "minishell: unclosed single quotes\n");
	return (!(par_matching) && !(dquotes & 1) && !(squotes & 1));
}

bool	validate_syntax(t_list *tokens)
{
	t_node	*tokp;
	bool	check;

	tokp = tokens->top->next;
	while (tokp->token != ENDOFCMD)
	{
		check = check_unexpected(tokp) && check_statements(tokp)
			&& check_parentheses(tokp) && check_redirections(tokp);
		if (!check)
			return (false);
		tokp = tokp->next;
	}
	return (check_matching_quotes_parentheses(tokens));
}