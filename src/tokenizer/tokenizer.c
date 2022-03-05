/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:42:41 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 19:52:41 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*whitespaces(t_list *tokens, char *line)
{
	int	len;

	len = 0;
	while (line[len] && line[len] != '\n' && ft_isspace(line[len]))
		len++;
	if (len)
		push_back(tokens, WSPACE, NULL);
	return (line + len);
}

char	*parentheses(t_list *tokens, char *line)
{
	char	c;
	t_token	token;

	c = *line;
	token = (c == '(') * OPAR + (c == ')') * CPAR;
	push_back(tokens, token, gc_filter(ft_chardup(c), GC_TMP));
	return (line + 1);
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
	token1 = (c == '|') * PIPE + (c == '&') * BG + (c == '>') * GREAT
		+ (c == '<') * LESS + (c == ';') * FG;
	token2 = (c == '|') * OR + (c == '&') * AND + (c == '>') * DGREAT
		+ (c == '<') * DLESS + (c == ';') * DSEMI;
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

char	*normal_state(t_list *tokens, char *line)
{
	int		len;
	char	*word;

	len = 0;
	while (!ft_strchr("\'\"()$<>&|;", line[len]) && !ft_isspace(line[len]))
		len++;
	if (len)
	{
		word = gc_filter(ft_strndup(line, len + 1), GC_TMP);
		if (word[0] == '~' && (!word[1] || word[1] == '/'))
			push_back(tokens, TILDE, word);
		else if (ft_strchr(word, '/') || !(ft_strchr(word, '*') || \
				ft_strchr(word, '?')))
			push_back(tokens, WORD, word);
		else
			push_back(tokens, WILDC, word);
	}
	return (line + len);
}

t_list	*tokenizer(char *cmdline)
{
	t_list	*tokens;

	tokens = new_list();
	push_back(tokens, CMDBEGIN, NULL);
	while (*cmdline && *cmdline != '#' && *cmdline != '\n')
	{
		if (ft_isspace(*cmdline))
			cmdline = whitespaces(tokens, cmdline);
		else if (*cmdline == '\'')
			cmdline = single_quote(tokens, cmdline + 1);
		else if (*cmdline == '\"')
			cmdline = double_quote(tokens, cmdline + 1);
		else if (*cmdline == '$')
			cmdline = dollar(tokens, cmdline);
		else if (*cmdline && ft_strchr("|&<>;", *cmdline))
			cmdline = lookahead_state(tokens, cmdline);
		else if (*cmdline == '(' || *cmdline == ')')
			cmdline = parentheses(tokens, cmdline);
		else
			cmdline = normal_state(tokens, cmdline);
	}
	push_back(tokens, ENDOFCMD, gc_filter(ft_strdup("newline"), GC_TMP));
	return (tokens);
}
