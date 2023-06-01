#include "minishell.h"

// exec the function right away because it is a sequential cmd.
// No need to fork.
// Because t_process is stored inside static variable no need 
// to initilized a new one, each forked process will received it's
// own t_process struct
static int32_t	parse_and_exec(t_token_sequence *token_seq)
{
	char		*str;
	t_cmd		*cmd;
	int32_t		ret;
	t_process	*proc;

	proc = get_process();
	tokenize(token_seq);
	str = parse_env(token_seq);
	reset_token_group(token_seq);
	token_seq->str = str;
	tokenize(token_seq);
	cmd = parse_cmd(token_seq);
	if (cmd->is_builtin)
		add_built_in_func(cmd);
	else
		add_execve_func(cmd);
	if (proc->errnum > 0)
		return (proc->errnum);
	ret = cmd->func(cmd);
	return (ret);
}

/// @brief logical operator such as && || stop the execution of the program
/// if an error occur.
/// @param cmd 
int32_t	fork_logical(t_token_sequence *token_seq)
{
	pid_t		pid;
	t_process	*proc;
	int32_t		status;
	int32_t		pipefd[2];
    
	pipe(pipefd);
	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{		
		get_process()->env_cpy = proc->env_cpy;
		get_process()->parent = proc;
		if (token_seq->cmd_seq_type == CMD_LOG_AND)
		{
			proc->errnum = parse_and_exec(token_seq);
			proc->stop_process = true;
		}
		else if (token_seq->cmd_seq_type == CMD_LOG_OR)
		{
			proc->errnum = parse_and_exec(token_seq);
			proc->stop_process = false;
		}
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
		exit(proc->errnum);
	}
	waitpid(pid, &status, 0);
	return (proc->errnum);
}
