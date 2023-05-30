#include "minishell.h"



t_pipe	*init_pipes(int32_t *fds, t_cmd *cmd)
{
	cmd->pipe = malloc(sizeof(t_cmd));
	if (!cmd->pipe)
		return (NULL);
	cmd->pipe->fd_in = fds[0];
	cmd->pipe->fd_out = fds[1];
	return (cmd->pipe);
}

void	pipe_cmd(t_cmd *cmd)
{
	int32_t	fds[2];

	if (pipe(fds) == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	init_pipes(fds, cmd);
}

static void	wait_childs(t_cmd *cmd)
{
	int32_t	i;
	int32_t	status;

	i = 0;
	while (cmd && cmd->pipe)
	{
		waitpid(cmd->pid, &status, 0);
		cmd = cmd->next;
		i++;
	}
	waitpid(cmd->pid, &status, 0);
}

void	fork_first_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();

	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = copy_env_from(proc);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		close(cmd->pipe->fd_in);
		close(cmd->pipe->fd_out);
		cmd->func(cmd);
		if (cmd->is_builtin)
			exit(EXIT_SUCCESS);
		else if (!cmd->is_builtin)
			exit(EXIT_FAILURE);
	}	
	cmd->pid = pid;
}

void	fork_last_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = copy_env_from(proc);
		dup2(cmd->prev->pipe->fd_in, STDIN_FILENO);
		close(cmd->prev->pipe->fd_in);
		close(cmd->prev->pipe->fd_out);
		cmd->func(cmd);
		if (cmd->is_builtin)
			exit(EXIT_SUCCESS);
		else if (!cmd->is_builtin)
			exit(EXIT_FAILURE);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	cmd->pid = pid;
}

void	fork_middle_child(t_cmd *cmd)
{
	pid_t		pid;
	t_process 	*proc;

	proc = get_process();
	pid = fork();
	if (pid == -1)
	{
		write_msg(STDERR_FILENO, strerror(errno));
		free_all_and_exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		get_process()->env_cpy = copy_env_from(proc);
		dup2(cmd->prev->pipe->fd_in, STDIN_FILENO);
		dup2(cmd->pipe->fd_out, STDOUT_FILENO);
		close(cmd->pipe->fd_out);
		close(cmd->prev->pipe->fd_in);
		close(cmd->prev->pipe->fd_out);
		cmd->func(cmd);
		if (cmd->is_builtin)
			exit(EXIT_SUCCESS);			
		else if (!cmd->is_builtin)
			exit(EXIT_FAILURE);
	}
	close(cmd->prev->pipe->fd_in);
	close(cmd->prev->pipe->fd_out);
	close(cmd->pipe->fd_out);
	cmd->pid = pid;
}

void	exec_pipes(t_cmd *cmd)
{
	int32_t	i;
	t_cmd	*start;

	i = 0;
	start = cmd;
	while (cmd)
	{		
		if (cmd->cmd_seq_type != CMD_PIPE)
		{
			fork_last_child(cmd);
			break;
		}
		else if (i == 0)
			fork_first_child(cmd);	
		else
			fork_middle_child(cmd);
		cmd = cmd->next;
		i++;
	}
	wait_childs(start);
}
