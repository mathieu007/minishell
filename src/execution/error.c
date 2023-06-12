#include "minishell.h"

/// @param stderror
/// @param msg 
/// @param msg2 
void	write_err2(int32_t stderror, char *msg, char *msg2)
{	
	char		*err_msg;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	write(2, err_msg, ft_strlen(err_msg));
	proc->errnum = stderror;
	free(err_msg);
}

void	write_err3(int32_t stderror, char *msg, char *msg2, char *msg3)
{	
	char		*err_msg;
	char		*err_msg2;
	t_process	*proc;

	proc = get_process();
	err_msg = ft_strjoin(msg, msg2);
	if (!err_msg)
		free_all_and_exit2(errno, "malloc error");
	err_msg2 = ft_strjoin(err_msg, msg3);
	if (!err_msg2)
		free_all_and_exit2(errno, "malloc error");
	free(err_msg);
	write(2, err_msg2, ft_strlen(err_msg2));
	proc->errnum = stderror;
	free(err_msg2);
}

void	write_err(int32_t stderror, char *msg)
{	
	t_process	*proc;

	proc = get_process();
	write(2, msg, ft_strlen(msg));
	proc->errnum = stderror;
}

void	write_msg(int32_t stderror, char *msg)
{
	if (msg)
		write(stderror, msg, ft_strlen(msg));
}