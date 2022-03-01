/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:47 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/01 16:47:56 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// typedef struct s_redir
// {
// 	int			node_type;
// 	t_token		redir_type;
// 	int			io_src;
// 	int			io_dst;
// 	int			oflag;
// 	char		*filename;
// 	t_cmdtree	*cmdtree;
// }				t_redir;

// void	fill_redir_node(t_redir *redir, t_token redir_type, char *file_delem)
// {
// 	redir->redir_type = redir_type;
// 	redir->io_src = STDIN_FILENO;
// 	redir->oflag = O_CREAT | O_RDONLY;
// 	if (redir_type & (GREAT | DGREAT))
// 	{
// 		redir->io_src = STDOUT_FILENO;
// 		redir->oflag &= ~O_RDONLY;
// 		if (ret)
// 	}
// }

// <redir>  ::=  {("<" | "<<" | ">" | ">>") <filename>}
t_cmdtree	*parse_redir(t_cmdtree *cmdtree, t_node **tokp)
{
	t_cmdtree	*ret;
	t_token		redir_type;

	if (current(*tokp) == ENDOFCMD)
		return (cmdtree);
	ret = cmdtree;
	while (current(*tokp) & REDIRECT)
	{
		redir_type = scan(tokp);
		if (current(*tokp) != WORD)
			return (NULL);
		ret = new_redir(ret);
		if (!fill_redir((t_redir *)ret, redir_type, (*tokp)->val))
			return (NULL);
		scan(tokp);
	}
	return (ret);
}