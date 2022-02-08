/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:42:41 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/08 17:33:03 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

/*
grammer rules :
	cmdline:  (pipeline [io_modifier]) | comment | varible_assignment | WHITESPACE
	pipeline : pipeline [PIPE cmd_and_args] | cmd_and_args | EMPTY
	cmd_and_args : WHITESPACE (cmd_name | cmd_path) cmd_arg*
	cmd_name : WORD
	cmd_path : PATH
	cmd_arg: WORD
	comment : HASH TEXT
*/

/**
 * TODO: - add allocated memory to tmp_dustbin
 * TODO: - PARSER		check syntax errors
 * TODO: - EXPANDER 	variables, tilde, wildcard
 * *	 - for the wildcard expansion: if there are no matches, the pattern will be considered as a WORD
 */

char	*var_expand_state(t_list *tokens, char *line)
{
	int	len;
		
	if (ft_isdigit(*line) || *line == '?' || *line == '$')
	{
		push_back(tokens, VAR_EXPANSION, ft_charstr(*line));
		return (line + 1);
	}
	len = 0;
	while (line[len] && (ft_isalnum(line[len]) || line[len] == '_'))
		len++;
	if (len)
		push_back(tokens, VAR_EXPANSION, ft_strndup(line, len + 1));
	else
		push_back(tokens, WORD, ft_charstr('$'));
	return (line + len);
}

char	*sq_parse_mode(t_list *cmdline, char *line)
{
	int	len;

	push_back(cmdline, SINGLE_QUOTE, ft_charstr('\''));
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\'')
		len++;
	push_back(cmdline, WORD, ft_strndup(line, len + 1));
	if (line[len] == '\'')
	{
		push_back(cmdline, SINGLE_QUOTE, ft_charstr('\''));
		len++;
	}
	return (line + len);
}

char	*dq_parse_mode(t_list *cmdline, char *line)
{
	int		len;

	push_back(cmdline, DOUBLE_QUOTE, ft_charstr('\"'));
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\"')
	{
		if (line[len] == '$')
		{
			if (len)
				push_back(cmdline, WORD, ft_strndup(line, len + 1));
			line = parse_expansion(cmdline, line + len + 1);
			len = 0;
		}
		else
			len++;
	}
	if (len)
		push_back(cmdline, WORD, ft_strndup(line, len + 1));
	if (line[len] == '\"')
	{
		push_back(cmdline, DOUBLE_QUOTE, ft_charstr('\"'));
		len++;
	}
	return (line + len);
}

char	*pipeline_parse_mode(t_list *cmdline, char *line)
{
	(void)cmdline;
	while (*line && *line != '\n')
		line++;
	return (line);
}

t_list	*tokenize(char *line)
{
	t_list	*cmdline;
	char	c;

	cmdline = new_list();
	while (*line && *line != '\n')
	{
		c = *line;
		if (c == '\'')
			line = sq_parse_mode(cmdline, line + 1);
		else if (c == '\"')
			line = dq_parse_mode(cmdline, line + 1);
		else
			line = pipeline_parse_mode(cmdline, line);
	}
	return (cmdline);
}