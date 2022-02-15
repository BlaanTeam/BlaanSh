/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_analyser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 16:24:15 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/15 16:12:25 by omoussao         ###   ########.fr       */
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
		- tokens that shoud cause an error: `;' | `;;' | `&'

	* AND_IF, OR_IF, PIPE:
		- left: [WHITESPACE] (WORD | PATH | ASSIGNMENT | C_PARENTHESES)
		- right: [WHITESPACE] (WORD | PATH | ASSIGNMENT | O_PARENTESES)

	* ASSIGNMENT:
		- left: CMDBEGIN | [WHITESPACE] (PIPE | AND_IF | OR_IF | O_PARENTHESES | ASSIGNMENT)
		- right: ENDOFCMD | WHITESPACE WORD | [WHITESPACE] C_PARENTHESES
		
	* O_PARENTHESES:
		- left: CMDBEGIN | [WHITESPACE] (AND_IF | OR_IF | PIPE | O_PARENTHESES)
		- right: [WHITESPACE] (WORD | SINGLE_QUOTE | DOUBLE_QUOTE | PATH | ASSIGNMENT | O_PARENTHESES)

	* C_PARENTHESES:
		- left: [WHITESPACE] (WORD | SINGLE_QUOTE | DOUBLE_QUOTE | PATH | C_PARENTHESES)
		- right: [WHITESPACE] (AND_IF | OR_IF | PIPE | C_PARENTHESES | ENDOFCMD)

	* LESS, DLESS, GREAT, DGREAT:
		- right: [WHITESPACE] WORD | PATH | TILDE_EXP | 

	* DLESS (heredoc):
		? do not expand TILDE nor the wildcards
		- right: [WHITESPACE] WORD  

	* PARENTHESES MATCHING AND QUOTING:
		- the content inside each open and closed parentheses sould not be empthy
		- every open parentheses has to have a matching closing parentheses
		- every single/double quotes have to be matched
*/

t_node	*get_right(t_node *tokp)
{
	t_node	*right;

	right = tokp->next;
	if (right->token == WHITESPACE)
		right = right->next;
	return (right);
}

t_node	*get_left(t_node *tokp)
{
	t_node	*left;

	left = tokp->prev;
	if (left->token == WHITESPACE)
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
	// if (tok != PIPE && tok != OR_IF && tok != AND_IF)
	if (!(tok & (PIPE | OR_IF | AND_IF)))
		return (true);
	left = get_left(tokp);
	right = get_right(tokp);
	// if (left->token != WORD && left->token != PATH && left->token != ASSIGNMENT && left->token != C_PARENTHESESE)
	if (!(left->token & (STRING | C_PARENTHESESE)))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
		return (false);
	}
	// else if (right->token != WORD && right->token != PATH && right->token != ASSIGNMENT && right->token != O_PARENTHESESE)
	else if (!(right->token & (STRING | O_PARENTHESESE)))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
		return (false);
	}
	return (true);
}

// bool	check_assignment(t_node *tokp)
// {
// 	(void)tokp;
// 	// TODO: will be done in expansion phase
// 	// if not in the right spot it will not cause an error, instead it will be changed to WORD
// 	return (true);
// }

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
		// if (left->token != CMDBEGIN && left->token != AND_IF && left->token != OR_IF && left->token != PIPE && left->token != O_PARENTHESESE)
		if (!(left->token & (CMDBEGIN | AND_IF | OR_IF | PIPE | O_PARENTHESESE)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		// else if (right->token != WORD && right->token != PATH && right->token != ASSIGNMENT && right->token != O_PARENTHESESE)
		else if (!(right->token & (STRING | O_PARENTHESESE)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
			return (false);
		}
	}
	else if (tok == C_PARENTHESESE)
	{
		// if (left->token != WORD && left->token != PATH && left->token != ASSIGNMENT && left->token != C_PARENTHESESE)
		if (!(left->token & (STRING | C_PARENTHESESE)))
		{
			fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tokp->val);
			return (false);
		}
		// else if (right->token != ENDOFCMD && right->token != AND_IF && right->token != OR_IF && right->token != PIPE && right->token != C_PARENTHESESE)
		else if (!(right->token & (ENDOFCMD | AND_IF | OR_IF | PIPE | C_PARENTHESESE)))
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
	// if (tok != LESS && tok != DLESS && tok != GREAT && tok != DGREAT)
	if (!(tok & (LESS | DLESS | GREAT | DGREAT)))
		return (true);
	right = get_right(tokp);
	if (!(right->token & STRING))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", right->val);
		return (false);
	}
	return (true);
}

