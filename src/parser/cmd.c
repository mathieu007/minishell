/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/07/25 09:46:50 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_cmd(t_cmd *cmd)
{
	cmd->name = free_ptr(cmd->name);
	if (cmd->type == CMD_PARENTHESES)
		return (cmd->name = ft_strdup(cmd->token->str), cmd);
	cmd->args = free_2d_char_array(cmd->args);
	cmd->args = parse_args(cmd->token);
	if (cmd->args == NULL || ((ft_strisempty(cmd->args[0])
				|| ft_striswhitespace(cmd->args[0]))
			&& !is_token_redirection(cmd->token->next->type)))
		return (NULL);
	cmd->name = ft_strdup(cmd->args[0]);
	if (!cmd->name)
		free_all_and_exit2(errno, "malloc error");
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->full_path_name = free_ptr(cmd->full_path_name);
	if (!cmd->is_builtin && !ft_strisempty(cmd->name))
	{
		free_ptr(cmd->full_path_name);
		cmd->full_path_name = get_full_path(cmd->name);
	}
	cmd->options = get_options(cmd->token);
	return (cmd);
}

int32_t	count_arr(char **arr)
{
	int32_t	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**resize_array(char **arr, int32_t add_count)
{
	int32_t	count;
	char	**new;
	int32_t	i;

	count = count_arr(arr);
	new = malloc(sizeof(char *) * (add_count + count + 1));
	if (!new)
		free_all_and_exit2(errno, "malloc error");
	new[add_count + count] = NULL;
	i = 0;
	while (i < count)
	{
		new[i] = ft_strdup(arr[i]);
		i++;
	}
	return (new);
}

char	**copy_args(char **dest, int32_t i, char **src)
{
	if (!src)
		return (NULL);
	while (*src)
	{
		dest[i] = ft_strdup(*src);
		if (!dest[i])
			free_all_and_exit2(errno, "malloc error");
		src++;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}
