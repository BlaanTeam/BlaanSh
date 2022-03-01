/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 17:04:34 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/01 19:38:50 by omoussao         ###   ########.fr       */
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
	return (expander(tokens));
}
