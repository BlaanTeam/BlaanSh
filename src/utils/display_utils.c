/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 15:41:26 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 23:06:53 by omoussao         ###   ########.fr       */
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
	"GROUP"
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
	fprintf(stderr, "-----------------Display Tokens-----------------\n");
	while (top)
	{
		fprintf(stderr, "tok = %-40s ", token_names[(int)log2(top->token)]);
		if (top->token != GROUP)
			fprintf(stderr, "val = %s\n", top->val);
		else
		{
			fprintf(stderr, "vals =");
			t_node *cpy = top->val_grp->top;
			while (cpy) {
				fprintf(stderr, " <%s: %s> ", token_names[(int)log2(cpy->token)], cpy->val);
				cpy = cpy->next;
			}
			fprintf(stderr, "\n");
		}
		top = top->next;
	}
	fprintf(stderr, "-------------------Tokens End-------------------\n");
}

void	display_venv(t_venv *venv)
{
	fprintf(stderr, "-----------------The venv variables-----------------\n");
	while (venv)
	{
		fprintf(stderr, "%s=%s\n", venv->key, venv->value);
		venv = venv->next;
	}
	fprintf(stderr, "-----------------END venv variables-----------------\n");
}

void	display_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		fprintf(stderr, "%s\n", env[i]);
}

// tree visualizer
void display_tree_(t_cmdtree *tree, int ident_level)
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
			if (lst->token != GROUP)
				fprintf(stderr, " <%s: %s> ", token_names[(int)log2(lst->token)], lst->val);
			else
			{
				t_node *cpy = lst->val_grp->top;
				fprintf(stderr, " <GROUP: {");
				while (cpy) {
					fprintf(stderr, " <%s: %s> ", token_names[(int)log2(cpy->token)], cpy->val);
					cpy = cpy->next;
				}
				fprintf(stderr, "}>");
			}
			lst = lst->next;
		}
		fprintf(stderr, "\n");
	}
	else if (tree->node_type == NODE_SUBSH) {
		fprintf(stderr, "├── SUBSH\n");
		display_tree_(((t_subsh *)tree)->cmdtree, ident_level + 1);
	}
	else if (tree->node_type == NODE_REDIR) {
		t_redir *redir = (t_redir *)tree;
		fprintf(stderr, "REDIR %d to ", redir->io_src);
		if (redir->filenode->token & (WORD | VAR))
			fprintf(stderr, "<%s: %s>", token_names[(int)log2(redir->filenode->token)], redir->filenode->val);
		else {
			t_node *cpy = redir->filenode->val_grp->top;
			fprintf(stderr, " <GROUP: {");
			while (cpy) {
				fprintf(stderr, " <%s: %s> ", token_names[(int)log2(cpy->token)], cpy->val);
				cpy = cpy->next;
			}
			fprintf(stderr, "}>");
		}
		fprintf(stderr, "\n");
		display_tree_(redir->cmdtree, ident_level);
	}
	else if (tree->node_type == NODE_PIPE || tree->node_type == NODE_FG
		|| tree->node_type == NODE_BG || tree->node_type == NODE_AND
		|| tree->node_type == NODE_OR)
	{
		fprintf(stderr, "├── %s\n", type[tree->node_type]);
		display_tree_(((t_connector *)tree)->lcmdtree, ident_level + 1);
		display_tree_(((t_connector *)tree)->rcmdtree, ident_level + 1);
	}
}

void	display_tree(t_cmdtree *tree)
{
	fprintf(stderr, "-------------------Syntax Tree-------------------\n");
	display_tree_(tree, 0);
	fprintf(stderr, "-----------------End Syntax Tree-----------------\n");
}