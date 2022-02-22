/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 13:35:44 by omoussao          #+#    #+#             */
/*   Updated: 2022/02/22 17:33:45 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*expand_var(t_list *tokens, t_node *node)
{
	char	*value;
	
	value = getenv(node->val);
	if (ft_strncmp(node->val, "?", 2) == 0)
		value = gc_filter(ft_itoa(WEXITSTATUS(g_global.status)), GC_TMP); // TODO : handle status code 
	if (!value)
	{
		del_node(tokens, node);
		if (node->prev->token == WHITESPACE && node->next->token == WHITESPACE)
			node = del_node(tokens, node->next);
	}
	else
	{
		node->val = value;
		if (ft_strchr(value, '/'))
			node->token = PATH;
		else
			node->token = WORD;
	}
	return (node->next);
}

t_node	*expand_tilde(t_node	*node)
{
	char	*home;

	home = getenv("HOME");
	if (home)
		node->val = gc_filter(ft_strjoin(home, node->val + 1), GC_TMP);
	if (ft_strchr(node->val, '/'))
		node->token = PATH;
	else
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
				if (node->token == WHITESPACE)
					node = del_node(tokens, node)->next;
			}
			insert_node(tokens, new_node(WORD, item->d_name), node->prev);
			insert_node(tokens, new_node(WHITESPACE, NULL), node->prev);
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

t_list	*concat_chained_words(t_list *tokens)
{
	t_node	*top;

	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token & (WORD | PATH))
		{
			while (top->next->token & (WORD | PATH))
			{
				top->val = gc_filter(ft_strjoin(top->val, top->next->val), GC_TMP);
				del_node(tokens, top->next);
			}
		}
		top = top->next;
	}
	return (tokens);
}

t_list	*expand(t_list *tokens)
{
	t_node	*top;
	t_node	*next;

	next = NULL;
	top = tokens->top;
	while (top && top->token != ENDOFCMD)
	{
		if (top->token & (DOUBLE_QUOTE | SINGLE_QUOTE))
			next = del_node(tokens, top)->next;
		else if (top->token == VAR_EXPANSION)
			next = expand_var(tokens, top);
		else if (top->token == TILDE_EXPANSION)
			next = expand_tilde(top);
		else if (top->token == WILDCARD_EXPANSION)
			next = expand_wildcards(tokens, top);
		else
			next = top->next;
		top = next;
	}
	return (concat_chained_words(tokens));
}