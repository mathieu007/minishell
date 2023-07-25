/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/21 13:16:06 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_args(t_token *token, char **split)
{
	int32_t	i;

	i = 0;
	token = token->child;
	while (token)
	{
		if (token->str && token->str[0] && token->next)
		{
			while (token && token->next)
			{
				if (!ft_strcontains(token->str, "*"))
					split[i] = ft_strjoinfree(split[i], token->str);
				if (token->next->type == TK_SPACE
					|| token->next->type == TK_END)
				{
					if (ft_strcontains(token->str, "*"))
						i = insert_files_as_args(split, i,
								get_cwd_files_array(token->str, " "));
					else
						i++;
					break ;
				}
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
	int32_t	i;

	i = 0;
	args_len = get_args_len(token);
	if (args_len == 0)
		return (NULL);
	split = malloc((args_len + 1) * sizeof(char *));
	if (!split)
		free_all_and_exit2(errno, "malloc error");
	while (i <= args_len)
	{
		split[i] = NULL;
		i++;
	}
	split[args_len] = NULL;
	set_args(token, split);
	return (split);
}
