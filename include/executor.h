/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 00:41:48 by asabani           #+#    #+#             */
/*   Updated: 2022/03/05 17:57:09 by asabani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# define READ_END	0
# define WRITE_END	1
# define LEFT_SIDE	1
# define RIGHT_SIDE	2
# define FILE_PERM	0644

// execution utils
int		ft_execvp(char *file, char **argv);
void	exec_cmd(char *cmd, char **argv);
void	executor(t_cmdtree *tree);
pid_t	run_pipe(t_connector *connector, int fds[2], int side);
void	run_pipeline(t_connector *connector);
void	run_cmdlist(t_cmdlist *cmdlist);
void	run_logical_connector(t_connector *connector, int node_type);
void	run_subshell(t_subsh *subshell);
void	run_bg_connector(t_connector *connector);
void	run_fg_connector(t_connector *connecter);
int		run_redirection(t_redir	*redir, int exec);

// expander utils used during execution
char	*expand_group(t_list *group);
char	**list_export_array(t_list *list);

#endif
