#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../libft/include/libft.h"

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*mem;
// 	size_t	s1_l;
// 	size_t	s2_l;

// 	s1_l = strlen(s1);
// 	s2_l = strlen(s2);
// 	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
// 	if (!mem)
// 		return (NULL);
// 	memcpy(mem, s1, s1_l);
// 	memcpy(&mem[s1_l], s2, s2_l);
// 	mem[s1_l + s2_l] = '\0';
// 	return (mem);
// }

// char	*ft_strjoinfree(char *s1, char *s2)
// {
// 	char	*mem;
// 	size_t	s1_l;
// 	size_t	s2_l;

// 	s1_l = strlen(s1);
// 	s2_l = strlen(s2);
// 	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
// 	if (!mem)
// 		return (free(s1), NULL);
// 	memcpy(mem, s1, s1_l);
// 	memcpy(&mem[s1_l], s2, s2_l);
// 	mem[s1_l + s2_l] = '\0';
// 	return (free(s1), mem);
// }

// char	*ft_strjoinfree2(char *s1, char *s2)
// {
// 	char	*mem;
// 	size_t	s1_l;
// 	size_t	s2_l;

// 	s1_l = strlen(s1);
// 	s2_l = strlen(s2);
// 	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
// 	if (!mem)
// 	{
// 		if (s1)
// 			free(s1);
// 		if (s2)
// 			free(s2);
// 		return (NULL);
// 	}
// 	memcpy(mem, s1, s1_l);
// 	memcpy(&mem[s1_l], s2, s2_l);
// 	mem[s1_l + s2_l] = '\0';
// 	if (s1)
// 		free(s1);
// 	if (s2)
// 		free(s2);
// 	return (mem);
// }

void copyFile(const char* sourcePath, const char* destinationPath) {
    FILE* sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        perror("Failed to open source file");
        return;
    }
    
    FILE* destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL) {
        perror("Failed to create destination file");
        fclose(sourceFile);
        return;
    }
    
    char buffer[4096];
    size_t bytesRead;
    
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }
    
    fclose(sourceFile);
    fclose(destinationFile);
}

int truncate_file(const char* filename, off_t length)
{
	int fd = open(filename, O_WRONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    int result = truncate(filename, length);
    if (result == -1) {
        perror("Failed to truncate file");
		close(fd);
        return 1;
    }
	close(fd);
    return 0;
}

extern char	**environ;
int			return_code;

char	*get_output(char *full_path, char *filename, char *arg1, char *arg2,
		int32_t fileno, int32_t discard)
{
	int		std_pipe[2];
	char	*output;
	pid_t	child_pid;
	int		dev_null_fd;
	int		child_status;
	char	buffer[256];
	ssize_t	read_size;
	int		len;

	output = NULL;
	if (pipe(std_pipe) == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Failed to fork");
		exit(EXIT_FAILURE);
	}
	else if (child_pid == 0)
	{
		if (dup2(std_pipe[1], fileno) == -1)
		{
			perror("Failed to redirect stderr");
			exit(EXIT_FAILURE);
		}
		dev_null_fd = open("/dev/null", O_WRONLY);
		if (dev_null_fd == -1)
		{
			perror("Failed to open /dev/null");
			exit(EXIT_FAILURE);
		}
		if (dup2(dev_null_fd, discard) == -1)
		{
			perror("Failed to redirect");
			exit(EXIT_FAILURE);
		}
		close(dev_null_fd);
		close(std_pipe[1]);
		execlp(full_path, filename, arg1, arg2, NULL);
		perror("Failed to execute command");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(std_pipe[1]);
		waitpid(child_pid, &child_status, 0);
		len = 0;
		fflush(stdout);
		read_size = read(std_pipe[0], buffer, sizeof(buffer));
		while (read_size > 0)
		{
			buffer[read_size] = '\0';
			output = realloc(output, len + read_size + 1);
			if (output == NULL)
			{
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			strcpy(output + len, buffer);
			len += read_size;
			read_size = read(std_pipe[0], buffer, sizeof(buffer));
		}
		close(std_pipe[0]);
		if (WIFEXITED(child_status))
			return_code = WEXITSTATUS(child_status);
		else if (WIFSIGNALED(child_status))
			return_code = WTERMSIG(child_status);
	}
	return (output);
}

char	*replaceString(const char *input, const char *search,
		const char *replace)
{
	size_t		inputLen;
	size_t		searchLen;
	size_t		replaceLen;
	size_t		resultLen;
	size_t		count;
	const char	*p;
	char		*result;
	const char	*inputPtr;
	char		*resultPtr;
	const char	*match;

	// Calculate the length of the resulting string
	inputLen = strlen(input);
	searchLen = strlen(search);
	replaceLen = strlen(replace);
	resultLen = inputLen;
	count = 0;
	// Count the number of occurrences of the search string
	p = input;
	while ((p = strstr(p, search)) != NULL)
	{
		count++;
		p += searchLen;
	}
	// Calculate the length of the resulting string after replacement
	resultLen += (replaceLen - searchLen) * count;
	// Allocate memory for the resulting string
	result = (char *)malloc((resultLen + 1) * sizeof(char));
	if (result == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return (NULL);
	}

	inputPtr = input;
	resultPtr = result;
	while (count > 0)
	{
		match = strstr(inputPtr, search);
		if (match == NULL)
		{
			// No more occurrences, copy the remaining part of the input string
			strcpy(resultPtr, inputPtr);
			break ;
		}
		// Copy the part before the match
		strncpy(resultPtr, inputPtr, match - inputPtr);
		resultPtr += match - inputPtr;
		// Copy the replacement string
		strcpy(resultPtr, replace);
		resultPtr += replaceLen;
		// Move the input pointer to the end of the matched search string
		inputPtr = match + searchLen;
		count--;
	}
	// Copy the remaining part of the input string
	strcpy(resultPtr, inputPtr);
	return (result);
}

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define VIOLET "\x1b[35m"
#define RESET "\x1b[0m"

#define MAX_COMMAND_LENGTH 1000000

void	print_exit_status(int bash_status, int minishell_status)
{
	printf("Bash exit status: %d\n", bash_status);
	printf("Minishell exit status: %d\n", minishell_status);
}

int	compare_files(const char *file1, const char *file2)
{
	FILE	*fp1;
	FILE	*fp2;
	int		equal;

	fp1 = fopen(file1, "r");
	fp2 = fopen(file2, "r");
	if (fp1 == NULL || fp2 == NULL)
	{
		perror("Error opening files for comparison");
		return (-1);
	}
	equal = 1;
	int ch1, ch2;
	ch1 = -1;
	ch2 = -1;
	while ((ch1 = fgetc(fp1)) != EOF && (ch2 = fgetc(fp2)) != EOF)
	{
		if (ch1 != ch2)
		{
			equal = 0;
			break ;
		}
	}
	if (ch1 != EOF || ch2 != EOF)
		equal = 0;
	fclose(fp1);
	fclose(fp2);
	return (equal);
}

char	*clean_output(char *output)
{
	char	*temp;

	temp = output;
	if (output)
	{
		output = replaceString(output, "bash: ", "");
		free(temp);
		temp = output;
		output = replaceString(output, "line 1: ", "");
		free(temp);
		temp = output;
		output = replaceString(output, "line 0: ", "");
		free(temp);
	}
	return (output);
}

void truncate_files(char *path)
{
	int		i;
	char	*filename1;
	char	*filename2;
	char	*num;

	i = 1;
	while (i < 10)
	{
		num = ft_itoa(i);

		filename1 = ft_strjoin(path, "/outfile_minishell");
		filename1 = ft_strjoinfree(filename1, num);
		
		filename2 = ft_strjoin(path, "/outfile_bash");
		filename2 = ft_strjoinfree(filename2, num);
		truncate_file(filename1, 0);
		truncate_file(filename2, 0);
		free(filename1);
		free(filename2);
		i++;
	}
}

int	check_file_content_equality(char *path)
{
	int		i;
	char	*filename1;
	char	*filename2;
	char	*num;
	int		files_content_equals;
	int		cur_files_content_equals;
	
	i = 1;
	files_content_equals = 1;
	cur_files_content_equals = 1;
	while (i < 10)
	{
		num = ft_itoa(i);
		filename1 = ft_strjoin(path, "/outfile_minishell");
		filename1 = ft_strjoinfree(filename1, num);
		
		filename2 = ft_strjoin(path, "/outfile_bash");
		filename2 = ft_strjoinfree(filename2, num);
		free(num);
		if (files_content_equals == 1)
		{
			cur_files_content_equals = compare_files(filename1, filename2);
			if (cur_files_content_equals == 0)
				printf("file mismatch %s : %s\n", filename1, filename2);
			files_content_equals = cur_files_content_equals;
		}
		// truncate_file(filename1, new_size);
		// truncate_file(filename2, new_size);
		free(filename1);
		free(filename2);
		i++;
	}
	return (files_content_equals);
}

void	run_test(char *command)
{
	char	*bash_output;
	char	*bash_err_output;
	char	*minishell_output;
	char	*minishell_err_output;
	int		bash_status;
	int		minishell_status;
	int		output_equal;
	int		output_err_equal;
	int		files_equals;
	char	*full_name;

	// char		bash_cmd[MAX_COMMAND_LENGTH];
	// char		minishell_command[MAX_COMMAND_LENGTH];
	// char		*command;
	char cwd[1024]; // Buffer to store the current directory path
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd() error");
		exit(1);
	}
	files_equals = 1;
	truncate_files(cwd);
	bash_output = NULL;
	minishell_output = NULL;
	output_err_equal = 0;
	full_name = ft_strjoin(cwd, "/minishell");
	// snprintf(bash_cmd, MAX_COMMAND_LENGTH, "%s", command);
	bash_err_output = get_output("bash", "bash", "-c", command, STDERR_FILENO,
			STDOUT_FILENO);
	// snprintf(bash_cmd, MAX_COMMAND_LENGTH, "%s", command2);
	bash_output = get_output("bash", "bash", "-c", command, STDOUT_FILENO,
			STDERR_FILENO);
	bash_status = return_code;
	// snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s", command2);
	minishell_err_output = get_output(full_name, "./minishell", command, NULL,
			STDERR_FILENO, STDOUT_FILENO);
	// snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s", command2);
	minishell_output = get_output(full_name, "./minishell", command, NULL,
			STDOUT_FILENO, STDERR_FILENO);
	minishell_status = return_code;
	output_equal = 0;
	bash_err_output = clean_output(bash_err_output);
	files_equals = check_file_content_equality(cwd);
	if (minishell_output && bash_output)
		output_equal = (int)(strcmp(bash_output, minishell_output) == 0);
	else if (!minishell_output && !bash_output)
		output_equal = 1;
	else if (!minishell_output || !bash_output)
		output_equal = 0;
	if (minishell_err_output && bash_err_output)
		output_err_equal = (int)(strcmp(bash_err_output,
					minishell_err_output) == 0);
	else if (!minishell_err_output && !bash_err_output)
		output_err_equal = 1;
	else if (!minishell_err_output || !bash_err_output)
		output_err_equal = 0;
	if ((output_equal && output_err_equal && files_equals))
	{
		printf(GREEN "TEST PASSED!\n\n" RESET);
		printf("Bash output:\n[%s]\n", bash_output);
		printf("Bash err output:\n[%s]\n", bash_err_output);
		printf("Bash exit status: %d\n\n", bash_status);
		printf("Minishell output:\n[%s]\n", minishell_output);
		if (files_equals)
			printf("Minishell out files are equals\n");
		else
			printf("Minishell out files differ\n");
		printf("Minishell err output:\n[%s]\n", minishell_err_output);
		printf("Minishell exit status: %d\n\n", minishell_status);
	}
	else
	{
		printf(RED "TEST FAILED!\n\n" RESET);
		printf("Bash output:\n[%s]\n", bash_output);
		printf("Bash err output:\n[%s]\n", bash_err_output);
		printf("Bash exit status: %d\n\n", bash_status);
		if (files_equals)
			printf("Minishell out files are equals\n");
		else
			printf("Minishell out files differ\n");
		printf("Minishell output:\n[%s]\n", minishell_output);
		printf("Minishell err output:\n[%s]\n", minishell_err_output);
		printf("Minishell exit status: %d\n\n", minishell_status);
	}
	free(bash_output);
	free(minishell_output);
	free(bash_err_output);
	free(minishell_err_output);
	free(full_name);
}

int	main(void)
{
	FILE	*file;
	char	line[MAX_COMMAND_LENGTH];

	file = fopen("./tests_redir.txt", "r");
	if (file == NULL)
	{
		perror("fopen");
		return (EXIT_FAILURE);
	}
	while (fgets(line, sizeof(line), file) != NULL)
	{
		// Remove trailing newline character
		line[strcspn(line, "\n")] = '\0';
		printf("--------------------------------------------------------\n");
		printf(VIOLET "TEST  %s\n" RESET, line);
		run_test(line);
	}
	fclose(file);
	return (EXIT_SUCCESS);
}
