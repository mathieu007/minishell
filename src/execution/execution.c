
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	char	**env;

	env = get_env();
	if (execve(cmd->full_path_name, cmd->args, env) == -1)
	{
		if (env)
			free(env);
		free_all_and_exit2(errno, "execve error");
	}
	return (errno);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

static int32_t	exec(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd);
	close_files_redirections(cmd);
	proc->errnum = cmd->func(cmd);
	return (proc->errnum);
}

static int32_t	fork_exec(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		status;

	proc = get_process();
	if (!cmd)
		return (proc->errnum);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
		return (exec(cmd));
	close_files_redirections(cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
}

int32_t	build_cmd(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = 0;
	if (!cmd->args)
	{
		build_token_environement(cmd->token);
		cmd = re_parse_at_execution(cmd);
		if (proc->errnum > 0)
			return (proc->errnum);
		if (cmd->has_redirection)
			create_fd_redir(cmd, cmd->next);
		return (proc->errnum);
	}
	return (proc->errnum);
}

int32_t	execute_command(t_cmd *cmd, bool should_exec_in_child)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum > 0)
		return (proc->errnum);
	if (cmd->is_builtin || !should_exec_in_child)
		proc->errnum = exec(cmd);
	else
		proc->errnum = fork_exec(cmd);
	return (proc->errnum);
}

int32_t	exec_commands(t_cmd *cmd, bool should_exec_in_child)
{
	t_process	*proc;

	proc = get_process();
	if (cmd->type == CMD_LOG_OR && proc->errnum == 0)
		proc->errnum = 1;
	if (cmd->type == CMD_LOG_AND && !cmd->prev)
		proc->errnum = 0;
	if (cmd->type == CMD_PIPE)
		proc->errnum = exec_pipes_cmds(cmd);
	else if (cmd->type == CMD_SEQUENTIAL)
		proc->errnum = exec_sequential(cmd);
	else if (cmd->type == CMD_LOG_AND && proc->errnum == 0)
		proc->errnum = exec_logical_and(cmd);
	else if (cmd->type == CMD_LOG_OR && proc->errnum > 0)
		proc->errnum = exec_logical_or(cmd);
	else if (cmd->type == CMD_PARENTHESES)
		proc->errnum = exec_subshell(cmd);
	else if (cmd->type == CMD)
		proc->errnum = execute_command(cmd, should_exec_in_child);
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

t_cmd	*re_parse_at_execution(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (!cmd)
		return (NULL);
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
		exec_commands(root_cmd->child, true);
	}
	free_t_cmd(root_cmd);
	free_t_tokens(proc->tokens);
	proc->tokens = NULL;
	proc->cmds = NULL;
	proc->last_cmd = NULL;
	proc->tokens = NULL;
	proc->last_errnum = proc->errnum;
	return (proc->errnum);
}
