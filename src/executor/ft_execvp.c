/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 17:32:29 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 00:25:37 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

static char	*find_path(char *cmd, char *path)
{
	char	*token;
	char	*cmd_path;

	if (!cmd)
		return (NULL);
	else if (!path || ft_strchr(cmd, '/'))
		return (cmd);
	cmd = gc_filter(ft_strjoin("/", cmd), GC_TMP);
	token = ft_strtok(path, ":");
	while (token)
	{
		gc_filter(token, GC_TMP);
		cmd_path = gc_filter(ft_strjoin(token, cmd), GC_TMP);
		if (!access(cmd_path, X_OK))
			return (cmd_path);
		token = ft_strtok(NULL, ":");
	}
	return (NULL);
}

int	ft_execvp(char *file, char **argv)
{
	char		**env;
	struct stat	stat_;

	argv[0] = find_path(file, getvenv("PATH"));
	if (!argv[0])
		(cmd_error(file, "command not found", NULL), \
		gc_clean(&g_global.gc, GC_TMP), exit(127));
	stat(argv[0], &stat_);
	if (S_ISDIR(stat_.st_mode))
		(cmd_error(file, strerror(EISDIR), NULL), \
		gc_clean(&g_global.gc, GC_TMP), exit(126));
	env = venv_export_array(g_global.venv);
	execve(argv[0], argv, env);
	return (-1);
}
