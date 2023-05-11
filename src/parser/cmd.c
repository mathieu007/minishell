/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/11 08:42:26 by mroy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static	char *get_cmd_name(t_token_group *group)
// {
// 	char	**split;
// 	int32_t	i;
// 	char	*name;
	
// 	split = ft_split(group->first->start, ' ');
// 	if (!split)
// 		return (NULL);
// 	if (!*split)
// 		return (free(split), NULL);
// 	i = 1;
// 	while (split[i])
// 		free(split[i]);
// 	name = *split;
// 	return (free(split), name);
// }

// static	void	*free_slpit(char **split)
// {
// 	if (split != NULL)
// 	{
// 		split++;
// 		while (*split)
// 			free(*split);
// 		free(split);
// 	}
// 	return (NULL);
// }

bool	is_builtins(char *str)
{
	int32_t	i;
	char	**builtins;
	int32_t	len;

	builtins = get_builtins_cmd();
	// str_len = ft_strlen(str) - 1;
	i = 0;
	while (builtins[i])
	{
		len = ft_strlen(builtins[i]);
		if (ft_strn_right_cmp(str, builtins[i], len))
			return (true);
		i++;
	}
	return (false);
}

inline bool	is_end_of_seq(t_token *token)
{
	t_token_type	type;

	type = token->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE
		|| token->next == NULL)
		return (true);
	return (false);
}



int32_t	options_count(t_token_group *group)
{
	int32_t	count;
	t_token	*token;

	count = 0;
	token = group->first;
	while (token)
	{
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
			count++;
		token = token->next;
	}
	return (count);
}

int32_t	get_option_len(char *option)
{
	int32_t	len;

	len = 0;	
	while (*option != ' ' || *option != '\t')
	{
		len++;
		option++;
	}
	return (len);
}

char	**get_options(t_token_group *group)
{
	int32_t	count;
	int32_t	i;
	char	**options;
	t_token *token;	

	count = options_count(group);
	if (count == 0)
		return (NULL);
	options = malloc(sizeof(char *) * count);
	if (options == NULL)	
		return (NULL);
	token = group->first;
	i = 0;
	while (token)
	{
		if (token->type == TK_DASH || token->type == TK_DASHDASH)
		{			
			options[i] = ft_strdupn(token->start, get_option_len(token->start));
			i++;
		}			
		token = token->next;
	}
	return (options);
}

int32_t	get_args_split_len(char *str)
{
	int32_t	i;
	int32_t	len;
	
	i = 0;
	len = 0;
	while (str[i])
	{
		if (is_escaped_double_quote(str,i)
			|| is_escaped_single_quote(str, i))
			i++;
		else if (is_opening_double_quote(str, i)
			|| is_opening_single_quote(str, i))
		{
			len++;
			i++;
		}
		if (str[i++] == ' ')
			len++;
		while (str[i] == ' ')
			i++;
	}
	return (len);
}

void	add_args(char *str, char **split)
{
	int32_t	i;
	int32_t	start;
	int32_t	split_i;
	
	i = 0;
	start = 0;
	split_i = 0;
	while (str[i])
	{
		if (is_escaped_double_quote(str,i)
			|| is_escaped_single_quote(str, i))
			i++;
		else if (is_opening_double_quote(str, i)
			|| is_opening_single_quote(str, i))
		{			
			start = i;
			i++;
			while (!is_closing_double_quote(str, i)
				&& !is_closing_single_quote(str, i))
			{
				split[split_i] =  ft_substr(str, start, i - start);
				split_i++;
			}			
		}
		if (str[i++] == ' ')
		{			
			while (str[i] == ' ')
				i++;
			start = i;
			while (str[i] && str[i] != ' ')
				i++;
			split[split_i] =  ft_substr(str, start, i - start);
			split_i++;
		}			
	}
}


char	**split_args(char *str)
{	
	// char	*start;
	int32_t	args_len;
	char	**split;

	// start = str;
	args_len = get_args_split_len(str);
	split = malloc(args_len * sizeof(char *));
	if (!split)
		return (NULL);
	add_args(str, split);
	return (split);
}

t_cmd_seq	get_sequence_type(t_token_group *group)
{
	t_token_type	type;
	
	type = group->last->type;
	if (type == TK_SEMICOLON || type == TK_AND || type == TK_OR
		|| type == TK_AMPERSAND || type == TK_GREATGREAT
		|| type == TK_LAST_PIPE_EXIT || type == TK_PIPE || type == TK_END)
		return ((t_cmd_seq)type);
	return (CMD_SEQUENTIAL);
}

t_cmd	*parse_cmd(t_token_group *group)
{
	t_cmd		*cmd;
	char		**args;

	cmd = add_cmd();
	args = split_args(group->start);
	cmd->name = args[0];
	if (cmd->name == NULL)
		return (NULL);	
	cmd->full_path_name = get_full_path(cmd->name);
	cmd->is_builtin = is_builtins(cmd->name);
	cmd->options = get_options(group);
	cmd->cmd_seq_type = get_sequence_type(group);
	cmd->args = &args[0];
	return (cmd);
}

t_cmd	*parse_cmds(t_token_group *group)
{
	if (!group)
		return (NULL);
	while (group)
	{
		parse_cmd(group);
		group = group->next;
	}
	return (get_first_cmd());
}
