/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mroy <mroy@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 07:02:30 by math              #+#    #+#             */
/*   Updated: 2023/05/16 13:39:54 by mroy             ###   ########.fr       */
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
			free_2d_Array((void **)cmd->args);
		if (cmd->options)
			free_2d_Array((void **)cmd->options);
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
		if (current->str)
			free(current->str);
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
	t_token_group *next;

	while (token_group)
	{
		next = token_group->next;
		free_ptr(&(token_group->str));	
		if (token_group->env_cpy)
			free_t_env_cpy(token_group->env_cpy);
		if (token_group->first_token)
			free_t_token(token_group->first_token);
		free(token_group);
		token_group = next;
	}
}