/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:24:15 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 18:52:16 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
grammer rules :
	
	! I-GRAMMAR:
	cmdline := pipeline | pipeline AND cmdline | pipeline OR cmdline

	pipeline := command | command PIPE pipeline
	
	command := [assignment] simple-command [arguments] [redirection:(*1)] | assignment

	simple-command := WORD | PATH
	
	arguments := WORD | WORD arguments
	
	redirection := LESS filename | DLESS delimiter | GREAT filename | DGREAT filename
	
	assignment := WORD [PLUS] EQUAL [WORD]

	(*1): could also be in any place before or after the simple-command or between the arguments

	! II-SYNTAX:
	* UNEXPECTED TOKENS:
		- `;;' (this token is always unrecognized in bash and zsh shells)

	* AND, OR, PIPE, FG, BG:
		- left: [WSPACE] (STRING | CPAR)
		- right: [WSPACE] (STRING | REDIRECT | OPAR | if <FG, BG> ENDOFCMD)

	* OPAR "(":
		- left: CMDBEGIN | [WSPACE] (AND | OR | PIPE | OPAR)
		- right: [WSPACE] (STRING | REDIRECT | OPAR)

	* CPAR ")":
		- left: [WSPACE] (STRING | CPAR)
		- right: [WSPACE] (AND | OR | PIPE | CPAR | ENDOFCMD)

	* REDIRECT:
		- right: [WSPACE] STRING

	* PARENTHESES MATCHING AND QUOTING:
		- inside each pair parentheses sould not be an empthy command
		- every open parentheses has to have a matching closing parentheses
		- every single/double quote have to be closed
*/

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
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
		return (false);
	}
	return (true);
}

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
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
		return (false);
	}
	else if ((tok & (FG | BG)) && (right->token & (ENDOFCMD | CPAR)))
		return (true);
	else if (!(right->token & (STRING | OPAR | REDIRECT)))
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
	if (tok != OPAR && tok != CPAR)
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	if (tok == OPAR)
	{
		if (!(left->token & (CMDBEGIN | CONNECTOR | OPAR)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		else if (!(right->token & (STRING | OPAR | REDIRECT)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
			return (false);
		}
	}
	else if (tok == CPAR)
	{
		if (!(left->token & (STRING | CPAR | BG | FG)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		else if (!(right->token & (ENDOFCMD | CONNECTOR | CPAR | REDIRECT)))
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
	// else if (right->token == VAR && tok != DLESS && (!getvenv(right->val) || !*getvenv(right->val)))
	// {
	// 	fprintf(stderr, "minishell: $%s: ambiguous redirect\n", right->val);
	// 	return (false);
	// }
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
		par_matching += (top->token == OPAR);
		par_matching -= (top->token == CPAR);
		dquotes += (top->token == DQUOTE);
		squotes += (top->token == SQUOTE);
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
		check = check_unexpected(tokp) && check_connectors(tokp)
			&& check_parentheses(tokp) && check_redirections(tokp);
		if (!check)
			return (false);
		tokp = tokp->next;
	}
	return (check_matching_quotes_parentheses(tokens));
}