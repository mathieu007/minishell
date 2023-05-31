#include "minishell.h"

void	write_err2(int32_t stderror, char *msg, char *msg2)
{	
	char	*err_msg;

	err_msg = ft_strjoin(msg, msg2);
	write(2, err_msg, ft_strlen(err_msg));
	free(err_msg);
	free_all_and_exit(stderror);
}

void	write_err3(int32_t stderror, char *msg, char *msg2, char *msg3)
{	
	char	*err_msg;
	char	*err_msg2;

	err_msg = ft_strjoin(msg, msg2);
	err_msg2 = ft_strjoin(err_msg, msg3);
	free(err_msg);
	write(2, err_msg2, ft_strlen(err_msg2));
	free(err_msg2);
	free_all_and_exit(stderror);
}

void	write_err(int32_t stderror, char *msg)
{	
	write(2, msg, ft_strlen(msg));
	free_all_and_exit(stderror);
}

void	write_msg(int32_t stderror, char *msg)
{
	if (msg)
		write(stderror, msg, ft_strlen(msg));
}
