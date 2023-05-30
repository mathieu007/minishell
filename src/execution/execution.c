
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	printf("%s\n", cmd->full_path_name);
	if (execve(cmd->full_path_name, cmd->args, get_env_path()) == -1)
		perror("Could not execve");
	return (1);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

static t_cmd	*parse(t_token_group *token_group)
{
	char	*str;
	t_cmd	*cmd;

	tokenize(token_group);
	str = parse_env(token_group);
	reset_token_group(token_group);
	token_group->str = str;
	tokenize(token_group);
	cmd = parse_cmd(token_group);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

/// @brief fork all piped command and execute then waitpid for all command to complete
/// @param token_group 
/// @return we return the last pipe command
t_token_group	*pipes_exec_cmds(t_token_group *token_group)
{
	t_cmd	*cmd;
	t_cmd	*start;

	cmd = parse(token_group);
	start = cmd;
	pipe_cmd(cmd);
	while (token_group->cmd_seq_type == CMD_PIPE)
	{
		token_group = token_group->next;
		if (token_group)
		{
			cmd = parse(token_group);
			pipe_cmd(cmd);
		}
	}
	return (exec_pipes(start), token_group->next);
}

// exec the function right away because it is a sequential cmd.
// No need to fork.
// Because t_process is stored inside static variable no need 
// to initilized a new one, each forked process will received it's
// own t_process struct
t_token_group	*exec_sequential(t_token_group *token_group)
{
	char	*str;
	t_cmd	*cmd;

	tokenize(token_group);
	str = parse_env(token_group);
	reset_token_group(token_group);
	token_group->str = str;
	tokenize(token_group);
	cmd = parse_cmd(token_group);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	cmd->func(cmd);
	return (token_group->next);
}

int32_t	exec_cmds(char *str)
{
	t_token_group	*token_group;

	token_group = tokenize_groups(str);
	while (token_group)
	{
		if (token_group->cmd_seq_type == CMD_PIPE)
			token_group = pipes_exec_cmds(token_group);
		else if (token_group->cmd_seq_type == CMD_NONE
			|| token_group->cmd_seq_type == CMD_SEQUENTIAL
			|| token_group->cmd_seq_type == CMD_LOG_AND
			|| token_group->cmd_seq_type == CMD_LOG_OR)
				token_group = exec_sequential(token_group);
		else
			return (free_all_and_exit(EXIT_FAILURE),
				printf("CMD_SEQUENCE_TYPE_UNKNOWN\n"));
	}
	//print_cmd(get_process()->cmds);
	return (1);
}
