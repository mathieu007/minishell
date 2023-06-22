
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{	
	if (execve(cmd->full_path_name, cmd->args, get_env()) == -1)
		free_all_and_exit2(errno, "execve error");
	return (errno);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

/// @brief fork all piped command and execute then waitpid for all command to complete
/// @param token_group
/// @return we return the last pipe command
int32_t	exec_pipes_cmds(t_cmd *pipe_group)
{
	t_cmd	*start;
	t_cmd	*cmd;

	cmd = pipe_group->child;
	start = cmd;
	pipe_cmd(cmd);
	cmd = cmd->next;
	while (cmd)
	{
		pipe_cmd(cmd);
		cmd = cmd->next;
	}
	return (fork_pipes(start));
}

static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = cmd->func(cmd);
	return (proc->errnum);
}

static int32_t	fork_exec(t_cmd	*cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;
	t_cmd		*redir;

	ret = 0;
	proc = get_process();
	redir = cmd->next;
	build_token_environement(cmd->token);
	cmd = parse_at_execution(cmd);
	if (cmd->has_redirection)
		create_fd_redir(cmd, redir->child);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		get_process()->env_cpy = proc->env_cpy;
		file_redirection(cmd);		
		ret = exec(cmd);
		close_files_redirections(cmd);
		exit(ret);
	}
	close_files_redirections(cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	proc->errnum = ret;
	return (ret);
}

int32_t	execute_command(t_cmd *cmd, bool should_exec_in_child)
{
	int32_t		ret;
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	if (!cmd->args)
	{
		build_token_environement(cmd->token);
		cmd = parse_at_execution(cmd);
	}
	if (proc->errnum > 0)
		return (proc->errnum);
	if (ft_strncmp(cmd->name, "export", 6) == 0 && ft_strlen(cmd->name) == 6)
		should_exec_in_child = false;
	if (should_exec_in_child || (cmd->has_redirection && (cmd->out_redir->fd == -1 && cmd->in_redir->fd == -1)))
		ret = fork_exec(cmd);
	else if (cmd->is_builtin && !should_exec_in_child)
		ret = exec(cmd);
	else
		ret = fork_exec(cmd);
	return (ret);
}

int32_t	exec_commands(t_cmd *cmd, bool should_exec_in_child)
{
	int32_t		ret;
	t_process	*proc;

	proc = get_process();
	ret = 0;
	if (cmd->type == CMD_GROUP_OR)
		ret = 1;
	while (cmd && cmd->type != CMD_NONE)
	{
		if (cmd->type == CMD_GROUP_PIPE)
			ret = exec_pipes_cmds(cmd);
		else if (cmd->type == CMD_GROUP_SEQUENTIAL)
			ret = exec_sequential(cmd);
		else if (cmd->type == CMD_GROUP_AND && ret == 0)
			ret = exec_logical_and(cmd);
		else if (cmd->type == CMD_GROUP_OR && ret > 0)
			ret = exec_logical_or(cmd);
		else if (cmd->type == CMD_PARENTHESES)
			ret = exec_subshell(cmd);
		else if (cmd->type == CMD)
			ret = execute_command(cmd, should_exec_in_child);
		cmd = cmd->next;
	}
	return (proc->errnum);
}

t_cmd	*create_cmds_tree(t_token *token)
{
	t_cmd	*root_cmd;

	if (!token)
		return (NULL);
	root_cmd = new_cmd(NULL);
	create_cmds(token, root_cmd);
	return (root_cmd);
}

t_cmd	*parse_at_execution(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	return (cmd);
}

int32_t	exec_cmds(char *str)
{
	t_token		*token;
	t_cmd		*root_cmd;
	t_process	*proc;

	root_cmd = NULL;
	proc = get_process();
	proc->errnum = 0;
	token = tokenize(str);
	if (!has_error())
	{
		root_cmd = create_cmds_tree(token->child);
		proc->cmds = root_cmd;
		exec_commands(root_cmd->child, false);
	}	
	free_t_tokens(token);
	free_t_cmd(root_cmd);
	proc->tokens = NULL;
	proc->cmds = NULL;
	proc->last_cmd = NULL;
	proc->tokens = NULL;
	proc->last_errnum = proc->errnum;
	return (proc->errnum);
}
