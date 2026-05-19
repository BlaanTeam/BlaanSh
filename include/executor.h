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

// execution utils — every dispatcher takes the t_cmdtree it dispatches on,
// so the discriminant (tree->kind) is always at hand and there are no blind
// casts into the union.
int		ft_execvp(char *file, char **argv);
void	exec_cmd(char *cmd, char **argv);
void	executor(t_cmdtree *tree);
pid_t	run_pipe(t_cmdtree *tree, int fds[2], int side);
void	run_pipeline(t_cmdtree *tree);
void	run_cmdlist(t_cmdtree *tree);
void	run_logical_connector(t_cmdtree *tree);
void	run_subshell(t_cmdtree *tree);
void	run_bg_connector(t_cmdtree *tree);
void	run_fg_connector(t_cmdtree *tree);
int		run_redirection(t_cmdtree *tree, int exec);

// expander utils used during execution
char	*expand_group(t_list *group);
char	**list_export_array(t_list *list);

#endif
