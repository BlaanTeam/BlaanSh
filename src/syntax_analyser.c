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
