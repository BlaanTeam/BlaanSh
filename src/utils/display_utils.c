/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:41:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/02 21:06:03 by omoussao         ###   ########.fr       */
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

const char *type[] = {
	"CMDLST",
	"SUBSH",
	"PIPE",
	"AND",
	"OR",
	"FG",
	"BG",
	"REDIR"
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

// tree visualizer
void display_tree(t_cmdtree *tree, int ident_level)
{
	if (tree == NULL)
		return ;
	for (int i = 0; i < ident_level; i++) {
		fprintf(stderr, "├   ");
	}
	if (tree->node_type == NODE_CMDLST)
	{
		fprintf(stderr, "├── CMDLIST:");
		t_node	*lst = ((t_cmdlist *)tree)->cmdvec->top;
		while (lst) {
			fprintf(stderr, " %s", lst->val);
			lst = lst->next;
		}
		fprintf(stderr, "\n");
	}
	else if (tree->node_type == NODE_SUBSH) {
		fprintf(stderr, "├── SUBSH\n");
		display_tree(((t_subsh *)tree)->cmdtree, ident_level + 1);
	}
	else if (tree->node_type == NODE_REDIR) {
		t_redir *redir = (t_redir *)tree;
		fprintf(stderr, "REDIR %d to `%s`\n", redir->io_src, redir->filename);
		display_tree(redir->cmdtree, ident_level + 1);
	}
	else if (tree->node_type == NODE_PIPE || tree->node_type == NODE_FG
		|| tree->node_type == NODE_BG || tree->node_type == NODE_AND
		|| tree->node_type == NODE_OR)
	{
		fprintf(stderr, "├── %s\n", type[tree->node_type]);
		display_tree(((t_connector *)tree)->lcmdtree, ident_level + 1);
		display_tree(((t_connector *)tree)->rcmdtree, ident_level + 1);
	}
}
