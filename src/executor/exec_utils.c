/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asabani <asabani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 21:06:04 by asabani           #+#    #+#             */
/*   Updated: 2022/02/28 00:24:29 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 	* The buitlin commands :
	*	- echo		-> Write arguments to the standard output.
	*	- cd 		-> Change the shell working directory.
	*	- pwd 		-> Print the name of the current working directory.
	*	- export 	-> Set export attribute for shell variables.
	*	- unset 	-> Unset values and attributes of shell variables.
	*	- env 		-> Set environment and execute command, or print environment.
	*	- exit		-> Exit the shell.
**/

static bool	is_builtin(char *cmd, char **argv)
{
	if (!ft_strncmp(cmd, "echo", sizeof("echo") + 1))
		return (echo(argv), true);
	else if (!ft_strncmp(cmd, "cd", sizeof("cd") + 1))
		return (cd(argv, &g_global.venv), true);
	else if (!ft_strncmp(cmd, "pwd", sizeof("pwd") + 1))
		return (pwd(argv, &g_global.venv), true);
	else if (!ft_strncmp(cmd, "export", sizeof("export") + 1))
		return (export(argv, &g_global.venv), true);
	else if (!ft_strncmp(cmd, "unset", sizeof("unset") + 1))
		return (unset(argv, &g_global.venv), true);
	else if (!ft_strncmp(cmd, "env", sizeof("env") + 1))
		return (ft_env(argv, &g_global.venv), true);
	else if (!ft_strncmp(cmd, "exit", sizeof("exit") + 1))
		return (ft_exit(argv), true);
	return (false);
}

void	exec_cmd(char *cmd, char **argv)
{
	int	pid;

	if (is_builtin(cmd, argv + 1))
		return ;
	pid = fork();
	if (pid == -1)
		(cmd_error("fork", strerror(errno), NULL), exit(1));
	else if (pid == 0)
	{
		term_restore();
		ft_execvp(cmd, argv);
		cmd_error(cmd, strerror(errno), NULL);
		gc_clean(&g_global.gc, GC_TMP);
		if (errno == ENOENT)
			exit(127);
		if (errno == EACCES)
			exit(126);
		exit(1);
	}
}
