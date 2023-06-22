#include "minishell.h"

inline bool	has_error(void)
{
	t_process	*proc;

	proc = get_process();
	if (proc->errnum > 0)
		return (true);
	return (false);
}

/// @param stderror
/// @param msg
/// @param msg2
void	write_err2(int32_t error, char *msg, char *msg2)
{
	char		*err_msg;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	write(2, err_msg, ft_strlen(err_msg));
	proc->errnum = error;
	if (proc->last_error)
		free(proc->last_error);
	proc->last_error = ft_strdup(err_msg);
	free(err_msg);
}

void	write_err3(int32_t error, char *msg, char *msg2, char *msg3)
{
	char		*err_msg;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	err_msg = ft_strjoinfree(err_msg, msg3);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	free(err_msg);
	write(2, err_msg, ft_strlen(err_msg));
	proc = get_process();
	if (proc->last_error)
		free(proc->last_error);
	proc->errnum = error;
	proc->last_error = ft_strdup(err_msg);
}

void	write_err(int32_t error, char *msg)
{
	t_process	*proc;

	proc = get_process();
	proc->errnum = error;
	if (proc->last_error)
		free(proc->last_error);
	proc->last_error = ft_strdup(msg);
	write(2, msg, ft_strlen(msg));
	proc->errnum = error;
	proc->last_error = msg;
}

void	write_msg(int32_t std_fileno, char *msg)
{
	if (msg)
		write(std_fileno, msg, ft_strlen(msg));
}
