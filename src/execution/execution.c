
#include "minishell.h"

int32_t	execve_cmd(t_cmd *cmd)
{
	char	**env;

	env = get_env();
	if (execve(cmd->full_path_name, cmd->args, env) == -1)
	{
		free_2d_char_array(env);
		free_all_and_exit2(errno, "execve error");
		
	}
	return (errno);
}

int32_t	add_execve_func(t_cmd *cmd)
{
	cmd->func = &execve_cmd;
	return (1);
}

static int32_t	exec_from_subshell_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, false);
	proc->errnum = cmd->func(cmd);
	fflush(stdout);
	close_files_redirections(cmd);
	return (proc->errnum);
}

static int32_t	exec_from_main_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, false);
	proc->errnum = cmd->func(cmd);
	fflush(stdout);
	close_files_redirections(cmd);
	return (proc->errnum);
}

static int32_t	exec_from_child_process(t_cmd *cmd)
{
	t_process	*proc;

	proc = get_process();
	file_redirection(cmd, true);
	close_files_redirections(cmd);
	proc->errnum = cmd->func(cmd);
	fflush(stdout);
	free_all_and_exit(proc->errnum);
	return (proc->errnum);
}

static int32_t	fork_exec(t_cmd *cmd)
{
	pid_t		pid;
	t_process	*proc;

	if (signal(SIGQUIT, sigquit_handler) == SIG_ERR) 
        free_all_and_exit2(1,"Signal SIGQUIT error" );
	proc = get_process();
	if (!cmd)
		return (proc->errnum);
	pid = ft_fork();
	if (pid == 0)
		return (exec_from_child_process(cmd));
	proc->errnum = ft_waitpid(pid);
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
		if (!cmd)
			return (-1);
		if (proc->errnum > 0)
			return (proc->errnum);
		if (cmd->has_redirection)
			create_fd_redir(cmd, cmd->next);
		return (proc->errnum);
	}
	return (proc->errnum);
}

int32_t	execute_command(t_cmd *cmd, bool is_in_child_process)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = build_cmd(cmd);
	if (proc->errnum == -1)
		return (0);
	if (proc->errnum > 0 || ft_strisempty(cmd->name))
		return (proc->errnum);
	if (cmd->is_builtin && is_in_child_process)
		proc->errnum = exec_from_child_process(cmd);
	else if (cmd->is_builtin && proc->is_subshell)
		proc->errnum = exec_from_subshell_process(cmd);
	else if (cmd->is_builtin && !is_in_child_process)
		proc->errnum = exec_from_main_process(cmd);
	else if (is_in_child_process)
		proc->errnum = exec_from_child_process(cmd);
	else
		proc->errnum = fork_exec(cmd);
	unlink_files_redirections(cmd->in_redir);
	return (proc->errnum);
}

int32_t	exec_commands(t_cmd *cmd, bool is_in_child_process)
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
		proc->errnum = execute_command(cmd, is_in_child_process);
	return (proc->errnum);
}

t_cmd	*create_cmds_tree(t_token *root_token)
{
	t_cmd		*root_cmd;
	t_process	*proc;
	t_token		*token;

	if (has_error() || !root_token)
		return (NULL);
	token = root_token->child;
	if (!token)
		return (NULL);
	proc = get_process();
	root_cmd = new_cmd(NULL);
	create_cmds(token, root_cmd);
	proc->cmds = root_cmd;
	return (root_cmd);
}

t_cmd	*re_parse_at_execution(t_cmd *cmd)
{
	cmd = parse_cmd(cmd);
	if (!cmd)
		return (NULL);
	if (cmd->next && is_token_redirection(cmd->next->token->type))
		cmd->has_redirection = true;
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

	token = NULL;
	root_cmd = NULL;
	proc = get_process();
	proc->errnum = 0;
	if (!ft_strisempty(str) && !ft_striswhitespace(str))
		token = tokenize(str);
	root_cmd = create_cmds_tree(token);
	if (root_cmd)
		exec_commands(root_cmd->child, false);
	free_t_cmd(proc->cmds);
	free_t_tokens(proc->tokens);
	proc->tokens = NULL;
	proc->cmds = NULL;
	proc->last_cmd = NULL;
	proc->tokens = NULL;
	proc->last_errnum = proc->errnum;
	proc->syntax_error = false;
	return (proc->errnum);
}
