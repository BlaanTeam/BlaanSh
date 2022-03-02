/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:56:47 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/02 15:45:45 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(char *delim)
{
	int		hfd[2];
	int		nbytes;
	char	buff[BUFSIZ + 1];

	if (pipe(hfd) == -1)
		return (-1);
	write(1, "heredoc> ", 9);
	nbytes = read(0, buff, BUFSIZ);
	while (nbytes > 0)
	{
		buff[nbytes] = 0;
		if ((nbytes - 1) && strncmp(buff, delim, nbytes - 1) == 0)
		{
			close(hfd[WRITE_END]);
			return (hfd[READ_END]);
		}
		write(hfd[WRITE_END], buff, nbytes);
		write(1, "heredoc> ", 9);
		nbytes = read(0, buff, BUFSIZ);
	}
	close(hfd[WRITE_END]);
	return (hfd[READ_END]);
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
	{
		redir->io_dst = heredoc(file_delem);
		if (redir->io_dst == -1)
			return (0);
	}
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
