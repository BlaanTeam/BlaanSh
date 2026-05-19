/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 17:32:29 by asabani           #+#    #+#             */
/*   Updated: 2022/03/02 00:38:31 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

/*
 * ft_strtok hands out malloc'd substrings, so we must free each one.
 * We walk the whole PATH (rather than break early) so the malloc'd
 * tokens that come after a match still get freed instead of leaking.
 */
static char	*find_path(char *cmd, char *path)
{
	char	*token;
	char	*cmd_path;
	char	*slash_cmd;
	char	*found;

	if (!cmd)
		return (NULL);
	if (!path || ft_strchr(cmd, '/'))
		return (cmd);
	slash_cmd = xstrjoin("/", cmd);
	found = NULL;
	token = ft_strtok(path, ":");
	while (token)
	{
		cmd_path = xstrjoin(token, slash_cmd);
		free(token);
		if (!found && !access(cmd_path, X_OK))
			found = cmd_path;
		token = ft_strtok(NULL, ":");
	}
	return (found);
}

int	ft_execvp(char *file, char **argv)
{
	char		**env;
	struct stat	stat_;

	argv[0] = find_path(file, getvenv("PATH"));
	if (!argv[0])
	{
		shell_error(file, "command not found", NULL, 1);
		child_exit(127);
	}
	stat(argv[0], &stat_);
	if (S_ISDIR(stat_.st_mode))
	{
		shell_error(file, strerror(EISDIR), NULL, 1);
		child_exit(126);
	}
	env = venv_export_array(g_global.venv);
	execve(argv[0], argv, env);
	return (-1);
}
