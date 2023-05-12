/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmartin <bmartin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/12 15:33:48 by bmartin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_t_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	if (cmd == NULL)
		return ;
	current = cmd;
	while (current != NULL)
	{
		next = current->next;
		if (cmd->name)
			free(cmd->name);
		if (cmd->full_path_name)
			free(cmd->full_path_name);
		if (cmd->args)
			free_2d_Array(cmd->args);
		if (cmd->options)
			free_2d_Array(cmd->options);
		if (cmd->pipe)
			free(cmd->pipe);
		if (cmd->redirect)
			free_t_redirect(cmd->redirect);
		free(current);
		current = next;
	}
}

void	free_t_token(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current != NULL)
	{
		next = current->next;
		if (current->start)
			free(current->start);
		free(current);
		current = next;
	}
}

void	free_t_env_cpy(t_env_cpy *env_cpy)
{
	t_env_cpy	*current;
	t_env_cpy	*next;

	current = env_cpy;
	while (current != NULL)
	{
		next = current->next;
		if (current->variable)
			free(current->variable);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

void	free_t_token_group(t_token_group *token_group)
{
	t_token_group *current;
	t_token_group *next;

	current = token_group;
	while (current != NULL)
	{
		next = current->next;
		if (current->start)
			free(current->start);
		if (current->end)
			free(current->end);
		if (current->parsed_str)
			free(current->parsed_str);
		if (current->env_cpy)
			free_t_env_cpy(current->env_cpy);
		if (current->first)
			free_t_token(current->first);
		if (current->last)
			free_t_token(current->last);
		free(current);
		current = next;
	}
}