#include "minishell.h"

void	*free_t_tokens(t_token *token)
{
	t_token	*current;
	t_token	*next;

	current = token;
	while (current != NULL)
	{
		printf("free address current:%lu\n", (uintptr_t)current);
		next = current->next;
		printf("free address str:%lu\n", (uintptr_t)current->str);
		current->str = free_ptr(current->str);
		current->str = NULL;
		current->token_str = free_ptr(current->token_str);
		printf("free address child:%lu\n", (uintptr_t)current->child);
		current->child = free_t_tokens(current->child);
		current->next = NULL;
		free(current);
		current = next;
	}
	return (NULL);
}

void	*free_t_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd;
	while (current != NULL)
	{
		next = current->next;
		current->child = free_t_cmd(current->child);
		current->name = free_ptr(current->name);
		current->full_path_name = free_ptr(current->full_path_name);
		current->args = free_2d_char_array(current->args);
		current->options = free_2d_array((void **)current->options);
		current->pipe = free_ptr(current->pipe);
		current->out_redir = free_t_redirect(current->out_redir);
		current->in_redir = free_t_redirect(current->in_redir);
		current->next = NULL;
		free(current);
		current = next;
	}
	return (NULL);
}

void	*free_t_process(t_process *proc)
{
	if (proc)
	{
		proc->cwd = free_ptr(proc->cwd);
		proc->env_cpy = free_t_env_cpy(proc->env_cpy);
		proc->tokens = free_t_tokens(proc->tokens);
		proc->cmds = free_t_cmd(proc->cmds);
		proc->last_error = free_ptr(proc->last_error);
	}
	return (NULL);
}