/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 13:35:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/04 21:16:10 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*expand_var(t_list *tokens, t_node *node)
{
	char	*value;

	value = getvenv(node->val);
	if (ft_strncmp(node->val, "?", 2) == 0)
		value = gc_filter(ft_itoa(get_status()), GC_TMP);
	if (!value)
	{
		del_node(tokens, node);
		if (node->prev->token == WSPACE && node->next->token == WSPACE)
			node = del_node(tokens, node->next);
	}
	else
	{
		node->token = WORD;
		node->val = value;
	}
	return (node->next);
}

t_node	*expand_tilde(t_node	*node)
{
	char	*home;

	home = getvenv("HOME");
	if (home)
		node->val = gc_filter(ft_strjoin(home, node->val + 1), GC_TMP);
	node->token = WORD;
	return (node->next);
}

bool match(char *name, char *pattern)
{
	int		n = ft_strlen(name);
	int		m = ft_strlen(pattern);
	bool	T[n + 1][m + 1];

	T[0][0] = true;
	for (int i = 1; i < n + 1; i++) {
		T[i][0] = false;
	}
	for (int j = 1; j < m + 1; j++) {
		T[0][j] = false;
		if (pattern[j - 1] == '*') T[0][j] = T[0][j - 1];
	}
	for (int i = 1; i < n + 1; i++) {
		for (int j = 1; j < m + 1; j++) {
			if (name[i - 1] == pattern[j - 1] || pattern[j - 1] == '?') {
				T[i][j] = T[i - 1][j - 1];
			}
			else if (pattern[j - 1] == '*') {
				T[i][j] = T[i - 1][j] || T[i][j - 1];
			}
			else {
				T[i][j] = false;
			}
		}
	}
	return T[n][m];
}

t_node	*expand_wildcards(t_list *tokens, t_node *node)
{
	DIR				*dirp;
	struct dirent	*item;
	char			*pattern;
	int				matches_found;

	pattern = node->val;
	matches_found = 0;
	dirp = opendir(".");
	item = readdir(dirp);
	while (item)
	{
		if ((pattern[0] == '.' || item->d_name[0] != '.') && match(item->d_name, pattern))
		{
			if (matches_found == 0)
			{
				node = del_node(tokens, node)->next;
				if (node->token == WSPACE)
					node = del_node(tokens, node)->next;
			}
			insert_node(tokens, new_node(WORD, item->d_name), node->prev);
			matches_found++;
		}
		item = readdir(dirp);
	}
	closedir(dirp);
	if (matches_found)
		return (node);
	else
	{
		node->token = WORD;
		return (node->next);
	}
}

// exec expand:
// navigate tokens
// if current is GROUP
// {
// 	make empthy <str>
// 	navigate GROUP
// 		if is VAR expand it
// 		concat with <str>
// }
void	concat_group(t_node *node)
{
	t_node	*top;

	top = node->val_grp->top;
	if (top->token != WORD)
		return ;
	node->val = top->val;
	top = top->next;
	while (top)
	{
		if (!(top->token & (WORD | WILDC | TILDE)))
			return ;
		node->val = gc_filter(ft_strjoin(node->val, top->val), GC_TMP);
		top = top->next;
	}
	node->token = WORD;
}

t_list	*concat_words(t_list *tokens)
{
	t_node	*top;

	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token == GROUP)
			concat_group(top);
		top = top->next;
	}
	return (tokens);
}

t_list	*delete_quotes(t_list *tokens)
{
	t_node	*top;
	t_node	*next;

	next = NULL;
	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		next = top->next;
		if (top->token & (DQUOTE | SQUOTE))
			next = del_node(tokens, top)->next;
		top = next;
	}
	return (tokens);
}

t_list	*make_groups(t_list *tokens)
{
	t_node	*top;

	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token & STRING)
		{
			while (top->next->token & STRING)
			{
				if (top->token != GROUP)
				{
					top->val_grp = new_list();
					push_back(top->val_grp, top->token, top->val);
					top->token = GROUP;
					continue ;
				}
				push_back(top->val_grp, top->next->token, top->next->val);
				del_node(tokens, top->next);
			}
		}
		if (top->token == WSPACE)
			del_node(tokens, top);
		top = top->next;
	}
	return (tokens);
}

t_node	*handle_redirects(t_node *node)
{
	t_node	*right;

	right = get_right(node);
	if (right->token == ENDOFCMD)
		return (node->next);
	if (node->token & DLESS)
	{
		if (right->token == GROUP)
		{
			right->val = right->val_grp->top->val;
			del_front(right->val_grp);
			while (right->val_grp->len)
			{
				right->val = gc_filter(
					ft_strjoin(right->val, right->val_grp->top->val), GC_TMP);
				del_front(right->val_grp);
			}
		}
		right->token = WORD;
	}
	else if (right->token & WILDC)
		right->token = WORD;
	return (right);
}

/*
tests:
<< $PATH
<< *$PATH"--HELLO--"
"M"*"~"/Desktop
ls -la HELLO$PATH
*/

t_list	*expander(t_list *tokens)
{
	t_node	*top;
	t_node	*next;

	tokens = delete_quotes(tokens);
	tokens = make_groups(tokens);
	tokens = concat_words(tokens);
	next = NULL;
	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		next = top->next;
		if (top->token & REDIRECT)
			next = handle_redirects(top);
		else if (top->token & TILDE)
			next = expand_tilde(top);
		else if (top->token & WILDC)
			next = expand_wildcards(tokens, top);
		else
			next = top->next;
		top = next;
	}
	return (tokens);
}
