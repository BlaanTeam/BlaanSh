/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:47 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 18:44:47 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(char *delim)
{
	int		hfd[2];
	char	*buff;
	
	if (!delim || ft_pipe(hfd) == -1)
		return (-1);
	while (true)
	{
		buff = readline("> ");
		if (!buff || ft_memcmp(delim, buff, ft_strlen(delim) + 1) == 0)
			break;
		gc_append(g_global.gc, buff, GC_TMP);
		write(hfd[WRITE_END], buff, ft_strlen(buff));
		write(hfd[WRITE_END], "\n", 1);
	}
	return (close(hfd[WRITE_END]), hfd[READ_END]);
}

int	fill_redir(t_redir *redir, t_token redir_type, char *file_delem)
{
	redir->redir_type = redir_type;
	redir->io_src = STDIN_FILENO;
	redir->oflag = O_RDONLY;
	redir->filename = file_delem;
	if (redir_type & (GREAT | DGREAT))
	{
		redir->io_src = STDOUT_FILENO;
		if (redir_type == GREAT)
			redir->oflag = O_CREAT | O_WRONLY | O_TRUNC;
		else
			redir->oflag = O_CREAT | O_WRONLY | O_APPEND;
	}
	else if (redir_type == DLESS)
		redir->io_dst = heredoc(file_delem);
	return (1);
}

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
