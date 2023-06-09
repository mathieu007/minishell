#include "minishell.h"
#include <fcntl.h>


int	redirect_append(const char *output_file)
{
	int output_fd;

//0644 value sets the file permissions to read and write for the owner, and read-only for the group and others.
	output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd == -1)
	{
		return(perror("Failed to open output file"),1);
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		perror("Failed to duplicate file descriptor");
		close(output_fd);
		return (1);
	}
	close(output_fd);
	return (0);
}

int	redirect_overwrite(const char *output_file)
{
	int output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC , 0644);
	if (output_fd == -1)
		return(perror("Failed to open output file"),1);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		perror("Failed to duplicate file descriptor");
		close(output_fd);
		return (1);
	}
	close(output_fd);
	return (0);
}

	