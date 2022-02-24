#include "minishell.h"

t_lexer	*lexer(char *cmdline)
{
	t_lexer	*tokens;

	if (!cmdline || !*cmdline)
		return (NULL);
	if (!ft_isspace(*cmdline))
		add_history(cmdline);
	tokens = tokenizer(cmdline);
	if (!validate_syntax(tokens))
		return (NULL);
	return (expander(tokens));
}