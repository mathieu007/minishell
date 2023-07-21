/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/17 09:44:14 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int32_t	check_redirection_syntax_errors(char *str, t_token_type type, 
	int32_t i)
{
	int32_t		len;
	t_process	*proc;

	proc = get_process();
	if (type == TK_LESSLESS)
		proc->has_here_doc = true;
	len = get_token_len(&str[i], type, false);
	i = check_newline_syntax_error(str, len, i);
	if (i != -1)
		i = check_syntax_error_after_near(str, i, "<>|&;()#");
	return (i);
}

static bool	has_syntax_errors(int32_t i, int32_t tk_len, t_token *parent)
{
	if (tk_len != 0 && check_syntax_error_before_near(parent->str, i,
			"|&;(<>") == -1)
		return (true);
	if (check_syntax_error_after_near(parent->str, i + tk_len, "|&;)<>") == -1)
		return (true);
	return (false);
}

bool	is_continuation(char *str)
{
	int32_t	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	if (!str[i])
		return (true);
	return (false);
}

int32_t	check_sequence_continuation(int32_t i, t_token_type	type, 
	t_token *parent)
{
	int32_t			len;
	char			*str;
	t_redirect		*redir;

	len = get_token_len(&parent->str[i], type, false);
	str = &parent->str[i + len];
	if (is_continuation(str))
	{
		redir = exec_continuation(parent);
		unlink_files_redirections(redir);
		free_t_redirect(redir);
		return (check_sequence_continuation(i, type, parent));
	}
	return (i + len);
}

int32_t	check_sequence_syntax_errors(int32_t i, t_token_type type,
		t_token *parent)
{
	int32_t	len;

	len = get_token_len(&parent->str[i], type, false);
	if (type == TK_SEMICOLON && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_OR && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_AND && has_syntax_errors(i++, len, parent))
		return (-1);
	else if (type == TK_PIPE && has_syntax_errors(i++, len, parent))
		return (-1);
	else
		check_sequence_continuation(i, type, parent);
	if (len == 2)
		return (i + 1);
	return (i);
}
