// #include "minishell.h"

// static t_cmd	*parse_logical_cmd(t_token *token)
// {
// 	char		*str;
// 	t_cmd		*cmd;
	
// 	str = parse_env(token);
// 	reset_token_sequence(token);
// 	token->str = str;
// 	tokenize(token);
// 	cmd = parse_cmd(token);
// 	if (cmd->is_builtin)
// 		add_built_in_func(cmd);
// 	else
// 		add_execve_func(cmd);
// 	return (cmd);
// }

// // exec the function right away because it is a sequential cmd.
// // No need to fork.
// // Because t_process is stored inside static variable no need 
// // to initilized a new one, each forked process will received it's
// // own t_process struct
// static int32_t	exec(t_cmd *cmd)
// {
// 	t_process	*proc;

// 	proc = get_process();
// 	if (proc->errnum > 0)
// 		return (proc->errnum);
// 	proc->errnum = cmd->func(cmd);
// 	return (proc->errnum);
// }

// static void	fork_exec_and(t_cmd	*cmd)
// {
// 	pid_t		pid;
// 	t_process	*proc;
// 	int32_t		ret;
// 	int32_t		status;

// 	pid = fork();
// 	proc = get_process();
// 	ret = proc->errnum;
// 	if (pid == -1)
// 	{
// 		write_msg(STDERR_FILENO, strerror(errno));
// 		free_all_and_exit(EXIT_FAILURE);
// 	}
// 	else if (pid == 0)
// 	{
// 		get_process()->env_cpy = proc->env_cpy;
// 		ret = exec(cmd);
// 		exit(ret);
// 	}
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		ret = WEXITSTATUS(status);
// 	if (ret != 0)
// 		proc->errnum = ret;
// }

// /// @brief logical operator such as && || stop the execution of the program
// /// if an error occur.
// /// @param cmd 
// t_token	*exec_logical_or(t_token *token)
// {
// 	t_process	*proc;
// 	t_cmd		*cmd;

// 	proc = get_process();
// 	proc->errnum = 0;
// 	// cmd = parse_logical_cmd(token);
// 	if (!cmd)
// 		return (token);
// 	if (cmd->is_builtin && proc->errnum == 0)
// 		proc->errnum = exec(cmd);
// 	else if (proc->errnum == 0)
// 		fork_exec(cmd);
// 	proc->stop_exec = false;
// 	if (proc->errnum == 0 && cmd->cmd_seq_type == CMD_LOG_OR)
// 		token = token->next;
// 	return (token);
// }

// t_token	*exec_logical_and(t_token *token)
// {
// 	t_process	*proc;
// 	t_cmd		*cmd;

// 	proc = get_process();
// 	proc->errnum = 0;
// 	cmd = parse_logical_cmd(token);
// 	if (!cmd)
// 		return (token);
// 	if (cmd->is_builtin && proc->errnum == 0)
// 		proc->errnum = exec(cmd);
// 	else if (proc->errnum == 0)
// 		fork_exec(cmd);
// 	proc->stop_exec = false;
// 	if (proc->errnum > 0 && cmd->cmd_seq_type == CMD_LOG_AND)
// 		proc->stop_exec = true;
// 	return (token);
// }
