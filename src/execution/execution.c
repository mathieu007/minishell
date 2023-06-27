
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
	(void) cmd;
	t_process	*proc;

	proc = get_process();
	// if (cmd->has_redirection)
	// 	create_fd_redir(cmd, redir->child);
	// if (has_error())
	// 	return (close_files_redirections(cmd), pipe->next);
	proc->errnum = cmd->func(cmd);
	return (proc->errnum);
}

static int32_t	fork_exec(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		ret;
	int32_t		status;
	t_cmd		*redir;

	ret = 0;
	proc = get_process();
	redir = cmd->next;
	if (!cmd)
		return (proc->errnum);
	pid = fork();
	if (pid == -1)
		free_all_and_exit2(errno, "fork error");
	else if (pid == 0)
	{
		file_redirection(cmd);
		close_files_redirections(cmd);
		ret = exec(cmd);
		return (ret);
	}
	close_files_redirections(cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		proc->errnum = WEXITSTATUS(status);
	return (proc->errnum);
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
		if (cmd->has_redirection)
			create_fd_redir(cmd, cmd->next->child);
		if (!cmd)
			return (proc->errnum);
	}
	if (proc->errnum > 0)
		return (proc->errnum);
	if (ft_strncmp(cmd->name, "export", 6) == 0 && ft_strlen(cmd->name) == 6)
		should_exec_in_child = false;
	if (cmd->is_builtin || !should_exec_in_child)
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
