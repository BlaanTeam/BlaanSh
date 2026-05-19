/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 19:51:31 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 19:52:22 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dollar(t_list *tokens, char *line)
{
	int	len;

	if (ft_isdigit(line[1]) || line[1] == '?' || line[1] == '$')
	{
		push_back(tokens, VAR, xstrndup(line, 2));
		return (line + 2);
	}
	len = 0;
	while (line[len + 1] && (ft_isalnum(line[len + 1]) || \
	line[len + 1] == '_'))
		len++;
	if (len)
		push_back(tokens, VAR, xstrndup(line, len + 1));
	else
		push_back(tokens, WORD, xstrndup(line, 1));
	return (line + len + 1);
}

char	*single_quote(t_list *tokens, char *line)
{
	int	len;

	push_back(tokens, SQUOTE, xchardup('\''));
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\'')
		len++;
	push_back(tokens, WORD, xstrndup(line, len));
	if (line[len] == '\'')
	{
		push_back(tokens, SQUOTE, xchardup('\''));
		len++;
	}
	return (line + len);
}

bool	emppthy_dquotes(t_list *tokens, char **line)
{
	push_back(tokens, DQUOTE, xchardup('\"'));
	if (**line == '\"')
	{
		push_back(tokens, WORD, xstrdup(""));
		push_back(tokens, DQUOTE, xchardup('\"'));
		(*line) += 1;
		return (1);
	}
	return (0);
}

char	*double_quote(t_list *tokens, char *line)
{
	int	len;

	if (emppthy_dquotes(tokens, &line))
		return (line);
	len = 0;
	while (line[len] && line[len] != '\n' && line[len] != '\"')
	{
		if (line[len] == '$')
		{
			if (len)
				push_back(tokens, WORD, xstrndup(line, len));
			line = dollar(tokens, line + len);
			len = 0;
		}
		else
			len++;
	}
	if (len)
		push_back(tokens, WORD, xstrndup(line, len));
	if (line[len] == '\"')
		push_back(tokens, DQUOTE, xchardup('\"'));
	return (line + len + (line[len] == '\"'));
}
