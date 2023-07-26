/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/26 10:41:25 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**insert_files(char **split, char **files)
{
	if (!files)
		return (split);
	if (!ft_strisempty(*split))
		split++;
	while (*files)
	{
		*split = ft_strdup(*files);
		split++;
		files++;
	}
	return (split);
}

char	**process_token(t_token *token, char **split)
{
	char	**files;

	if ((!ft_strcontains(token->str, "*") && !is_token_delimiter(token->type)))
		*split = ft_strjoinfree(*split, token->str);
	else if (is_token_delimiter(token->type))
		*split = ft_strjoinfree(*split, token->str);
	if (token->next->type == TK_SPACE || token->next->type == TK_END)
	{
		if (ft_strcontains(token->str, "*") && !is_token_delimiter(token->type))
		{
			files = get_cwd_files_array(token->str, " ");
			if (files)
				split = insert_files(split, files);
			else
				*split = ft_strjoinfree(*split, token->str);
			free_2d_char_array(files);
		}
		else
			split++;
	}
	return (split);
}

void	set_args(t_token *token, char **split)
{
	char	**prev_split;

	token = token->child;
	while (token)
	{
		if (((!token->str || ft_strisempty(token->str))
				&& is_token_delimiter(token->type) && token->next)
			|| (token->str && token->str[0] && token->next))
		{
			while (token && token->next)
			{
				prev_split = split;
				split = process_token(token, split);
				if (split != prev_split)
					break ;
				token = token->next;
			}
		}
		token = token->next;
	}
}

char	**parse_args(t_token *token)
{
	int32_t	args_len;
	char	**split;

	args_len = get_args_len(token);
	if (args_len == 0)
		return (NULL);
	split = ft_calloc((args_len + 1), sizeof(char *));
	if (!split)
		free_all_and_exit2(errno, "malloc error");
	split[args_len] = NULL;
	set_args(token, split);
	return (split);
}
