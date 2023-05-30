#include "minishell.h"

void	write_err2(int32_t stderror, char *msg, char *msg2)
{	
	char	*err_msg;

	err_msg = ft_strjoin(msg, msg2);
	write(2, err_msg, ft_strlen(err_msg));
	free(msg);
	free(msg2);
	free(err_msg);
	free_all_and_exit(stderror);
}