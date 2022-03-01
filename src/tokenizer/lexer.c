/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:04:34 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/01 23:00:53 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*lexer(char *cmdline)
{
	t_lexer	*tokens;

	if (!cmdline || !*cmdline)
		return (NULL);
	if (!ft_isspace(*cmdline))
		add_history(cmdline);
	tokens = tokenizer(cmdline);
	if (!validate_syntax(tokens))
	{
		set_status(2);
		return (NULL);
	}
	tokens = expander(tokens);
	del_front(tokens);
	return (tokens);
}
