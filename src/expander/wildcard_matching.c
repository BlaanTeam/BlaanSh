/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_matching.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/05 17:01:02 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/05 17:06:32 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	**dp_init(char *pattern, int n, int m)
{
	bool	**table;
	int		i;

	table = gc_filter(malloc((n + 1) * sizeof(bool *)), GC_TMP);
	i = 0;
	while (i < n + 1)
		table[i++] = gc_filter(malloc((m + 1) * sizeof(bool)), GC_TMP);
	table[0][0] = true;
	i = 1;
	while (i < n + 1)
		table[i++][0] = false;
	i = 1;
	while (i < m + 1)
	{
		table[0][i] = false;
		if (pattern[i - 1] == '*')
			table[0][i] = table[0][i - 1];
		i++;
	}
	return (table);
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
