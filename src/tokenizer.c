/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:42:41 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/09 17:24:03 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		push_back(tokens, VAR_EXPANSION, gc_filter(ft_chardup(*line), GC_TMP));
		return (line + 1);
	}
	len = 0;
	while (line[len] && (ft_isalnum(line[len]) || line[len] == '_'))
		len++;
	if (len)
		push_back(tokens, VAR_EXPANSION, gc_filter(ft_strndup(line, len + 1), GC_TMP));
	else
		push_back(tokens, WORD, gc_filter(ft_chardup('$'), GC_TMP));
	return (line + len);
}

char	*squote_state(t_list *tokens, char *line)
{
	int	len;

	push_back(tokens, SINGLE_QUOTE, gc_filter(ft_chardup('\''), GC_TMP));
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\'')
		len++;
	push_back(tokens, WORD, gc_filter(ft_strndup(line, len + 1), GC_TMP));
	if (line[len] == '\'')
	{
		push_back(tokens, SINGLE_QUOTE, gc_filter(ft_chardup('\''), GC_TMP));
		len++;
	}
	return (line + len);
}

char	*dquote_state(t_list *tokens, char *line)
{
	int		len;

	push_back(tokens, DOUBLE_QUOTE, gc_filter(ft_chardup('\"'), GC_TMP));
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\"')
	{
		if (line[len] == '$')
		{
			if (len)
				push_back(tokens, WORD, gc_filter(ft_strndup(line, len + 1), GC_TMP));
			line = var_expand_state(tokens, line + len + 1);
			len = 0;
		}
		else
			len++;
	}
	if (len)
		push_back(tokens, WORD, gc_filter(ft_strndup(line, len + 1), GC_TMP));
	if (line[len] == '\"')
	{
		push_back(tokens, DOUBLE_QUOTE, gc_filter(ft_chardup('\"'), GC_TMP));
		len++;
	}
	return (line + len);
}

char	*whitespaces(t_list *tokens, char *line)
{
	int	len;

	len = 0;
	while (line[len] && line[len] != '\n' && ft_isspace(line[len]))
		len++;
	if (len)
		push_back(tokens, WHITESPACE, NULL);
	return (line + len);
}

char	*lookahead_state(t_list *tokens, char *line)
{
	char	c;
	int		len;
	t_token	token1;
	t_token	token2;
	char	val2[3];

	c = *line;
	val2[0] = c;
	val2[1] = c;
	token1 = (c == '|') * PIPE + (c == '&') * AMPERSAND + (c == '>') * GREAT
		+ (c == '<') * LESS + (c == ';') * SEMICL;
	token2 = (c == '|') * OR_IF + (c == '&') * AND_IF + (c == '>') * DGREAT
		+ (c == '<') * DLESS + (c == ';') * DSEMICL;
	len = 1;
	while (line[len] == c)
		len++;
	line += len;
	while (len > 1)
	{
		push_back(tokens, token2, gc_filter(ft_strndup(val2, 3), GC_TMP));
		len -= 2;
	}
	if (len)
		push_back(tokens, token1, gc_filter(ft_chardup(c), GC_TMP));
	return (line);
}

char	*character_state(t_list *tokens, char *line)
{
	char	c;
	t_token	token;

	c = *line;
	token = (c == '=') * EQUAL + (c == '(') * O_PARENTHESESE
		+ (c == ')') * C_PARENTHESESE;
	push_back(tokens, token, gc_filter(ft_chardup(c), GC_TMP));
	return (line + 1);
}

// tilde will work only if it's passed like ~ or ~/<some characters that possibly 0>
// wildcard expansion will not work if the token is represented as a TILDE_EXPANSION or as a PATH
char	*general_state(t_list *tokens, char *line)
{
	int		len;
	char	*word;

	len = 0;
	while (!ft_strchr("\'\"()$=<>&|;", line[len]) && !ft_isspace(line[len]))
		len++;
	if (len)
	{
		word = gc_filter(ft_strndup(line, len + 1), GC_TMP);
		if (word[0] == '~' && (!word[1] || word[1] == '/'))
			push_back(tokens, TILDE_EXPANSION, word);
		else if (ft_strchr(word, '/'))
			push_back(tokens, PATH, word);
		else if (ft_strchr(word, '*'))
			push_back(tokens, WILDCARD_EXPANSION, word);
		else
			push_back(tokens, WORD, word);
	}
	return (line + len);
}

t_list	*tokenize(char *line)
{
	t_list	*tokens;

	tokens = new_list();
	while (*line && *line != '#' && *line != '\n')
	{
		if (ft_isspace(*line))
			line = whitespaces(tokens, line);
		else if (*line == '\'')
			line = squote_state(tokens, line + 1);
		else if (*line == '\"')
			line = dquote_state(tokens, line + 1);
		else if (*line == '$')
			line = var_expand_state(tokens, line + 1);
		else if (*line && ft_strchr("|&<>;", *line))
			line = lookahead_state(tokens, line);
		else if (*line == '=' || *line == '(' || *line == ')')
			line = character_state(tokens, line);
		else
			line = general_state(tokens, line);
	}
	push_back(tokens, TK_NEWLINE, NULL);
	return (tokens);
}