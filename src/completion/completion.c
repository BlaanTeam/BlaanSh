/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Readline tab-completion hooks.                                           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

/*
 * Completion contract from GNU readline:
 *   - rl_attempted_completion_function gets (text, start, end) and
 *     returns a NULL-terminated array of strings. Both array and
 *     strings must be malloc'd; readline frees them.
 *   - rl_completion_matches(text, gen) repeatedly calls `gen(text, state)`
 *     with state=0 first, then state=1,2,... until gen returns NULL.
 *     Each non-NULL return must be a malloc'd string.
 *
 * Since readline owns the strings, we use plain strdup/malloc here —
 * NOT the shell's arena.
 */

static const char	*g_builtins[] = {
	"cd", "echo", "env", "exit", "export", "pwd", "unset", NULL
};

/* Returns 1 if `name` is in `path` and executable. */
static int	is_executable_in(const char *dir, const char *name)
{
	char		path[PATH_MAX];
	struct stat	st;

	if (snprintf(path, sizeof(path), "%s/%s", dir, name) >= (int)sizeof(path))
		return (0);
	if (stat(path, &st) != 0 || !S_ISREG(st.st_mode))
		return (0);
	return (access(path, X_OK) == 0);
}

/* Builtin/PATH/cwd-executable completion for the first word. */
static char	*command_generator(const char *text, int state)
{
	static int				phase;
	static int				bi;
	static char				*path_dup;
	static char				*saveptr;
	static char				*dir;
	static DIR				*dirp;
	static size_t			tlen;
	struct dirent			*ent;

	if (state == 0)
	{
		phase = 0;
		bi = 0;
		tlen = strlen(text);
		if (path_dup)
			free(path_dup);
		path_dup = NULL;
		if (dirp)
			closedir(dirp);
		dirp = NULL;
		dir = NULL;
		saveptr = NULL;
	}
	while (phase == 0)
	{
		const char	*b = g_builtins[bi];
		if (!b)
		{
			phase = 1;
			path_dup = getvenv("PATH");
			path_dup = (path_dup ? strdup(path_dup) : NULL);
			saveptr = path_dup;
			break ;
		}
		bi++;
		if (strncmp(b, text, tlen) == 0)
			return (strdup(b));
	}
	while (phase == 1)
	{
		if (!dirp)
		{
			if (!saveptr || !*saveptr)
			{
				phase = 2;
				break ;
			}
			dir = saveptr;
			while (*saveptr && *saveptr != ':')
				saveptr++;
			if (*saveptr == ':')
			{
				*saveptr = '\0';
				saveptr++;
			}
			dirp = opendir(dir);
			if (!dirp)
				continue ;
		}
		ent = readdir(dirp);
		if (!ent)
		{
			closedir(dirp);
			dirp = NULL;
			continue ;
		}
		if (strncmp(ent->d_name, text, tlen) != 0)
			continue ;
		if (!is_executable_in(dir, ent->d_name))
			continue ;
		return (strdup(ent->d_name));
	}
	if (path_dup)
	{
		free(path_dup);
		path_dup = NULL;
	}
	return (NULL);
}

/* Variable name completion: text starts with '$'. */
static char	*var_generator(const char *text, int state)
{
	static t_venv	*node;
	static size_t	tlen;
	const char		*needle;

	if (state == 0)
	{
		node = g_global.venv;
		tlen = strlen(text);
	}
	needle = text + 1;
	while (node)
	{
		t_venv	*cur = node;
		node = node->next;
		if (strncmp(cur->key, needle, tlen - 1) == 0)
		{
			size_t	klen = strlen(cur->key);
			char	*out = malloc(klen + 2);
			if (!out)
				return (NULL);
			out[0] = '$';
			memcpy(out + 1, cur->key, klen);
			out[klen + 1] = '\0';
			return (out);
		}
	}
	return (NULL);
}

/* True if every non-whitespace char to the left of `start` is empty
 * or a shell separator that begins a new command word. */
static int	is_command_position(int start)
{
	const char	*line = rl_line_buffer;
	int			i;

	i = start - 1;
	while (i >= 0 && (line[i] == ' ' || line[i] == '\t'))
		i--;
	if (i < 0)
		return (1);
	return (line[i] == '|' || line[i] == ';' || line[i] == '&'
		|| line[i] == '(');
}

char	**shell_completion(const char *text, int start, int end)
{
	(void)end;
	rl_attempted_completion_over = 1;
	if (text[0] == '$')
		return (rl_completion_matches(text, var_generator));
	if (is_command_position(start))
		return (rl_completion_matches(text, command_generator));
	return (rl_completion_matches(text, rl_filename_completion_function));
}

void	completion_init(void)
{
	rl_attempted_completion_function = shell_completion;
	/* Default break chars include $, which would split "$VAR" into the
	 * '$' and "VAR" — preventing us from seeing the '$' in the text.
	 * Strip '$' from the break set so $-completion sees the full token. */
	rl_completer_word_break_characters = " \t\n\"'`<>=;|&(){}";
}
