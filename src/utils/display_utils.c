/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:41:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/28 15:51:33 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Temporary file
//* Functions in this file aim to display/visulize different
//* data structures used in the project (for debugging purposes)
#include "minishell.h"
#include <math.h>

const char *token_names[] = {
	"CMDBEGIN",
	"ENDOFCMD",
	"WSPACE",
	"WORD",
	"PIPE",
	"AND",
	"OR",
	"OPAR",
	"CPAR",
	"BG",
	"FG",
	"DSEMI",
	"VAR",
	"TILDE",
	"WILDC",
	"SQUOTE",
	"DQUOTE",
	"LESS",
	"DLESS",
	"GREAT",
	"DGREAT",
};

// todo: display_astree

void	display_tokens(t_node *top)
{
	printf("-----------------Display Tokens-----------------\n");
	while (top)
	{
		printf("tok = %-40s val = %s\n", token_names[(int)log2(top->token)], top->val);
		top = top->next;
	}
	printf("-------------------Tokens End-------------------\n");
}

void	display_venv(t_venv *venv)
{
	printf("-----------------The venv variables-----------------\n");
	while (venv)
	{
		printf("%s=%s\n", venv->key, venv->value);
		venv = venv->next;
	}
}

void	display_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
}
