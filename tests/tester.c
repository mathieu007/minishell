#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = strlen(s1);
	s2_l = strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
		return (NULL);
	memcpy(mem, s1, s1_l);
	memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	return (mem);
}

char	*ft_strjoinfree(char *s1, char *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = strlen(s1);
	s2_l = strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
		return (free(s1), NULL);
	memcpy(mem, s1, s1_l);
	memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	return (free(s1), mem);
}

char	*ft_strjoinfree2(char *s1, char *s2)
{
	char	*mem;
	size_t	s1_l;
	size_t	s2_l;

	s1_l = strlen(s1);
	s2_l = strlen(s2);
	mem = (char *)malloc(sizeof(char) * (s1_l + s2_l + 1));
	if (!mem)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	memcpy(mem, s1, s1_l);
	memcpy(&mem[s1_l], s2, s2_l);
	mem[s1_l + s2_l] = '\0';
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	return (mem);
}

extern char** environ;
int		return_code;



char *get_output(char *full_path, char *filename, char *arg1, int32_t fileno, int32_t discard) {
	int std_pipe[2];
	char *output = NULL;
	if (pipe(std_pipe) == -1) {
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("Failed to fork");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		if (dup2(std_pipe[1], fileno) == -1) {
			perror("Failed to redirect stderr");
			exit(EXIT_FAILURE);
		}
		int dev_null_fd = open("/dev/null", O_WRONLY);
        if (dev_null_fd == -1) {
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
		execlp(full_path,filename, arg1, NULL);
		perror("Failed to execute command");
		exit(EXIT_FAILURE);
	} else {
		close(std_pipe[1]);
		int child_status;
		waitpid(child_pid, &child_status, 0);
		char buffer[256];
		ssize_t read_size;
		int len;
		len = 0;
		while ((read_size = read(std_pipe[0], buffer, sizeof(buffer))) > 0)
		{
			output = realloc(output, len + read_size + 1);
			if (output == NULL)
			{
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			strcpy(output + len, buffer);
			len += read_size;
		}
		close(std_pipe[0]);
		if (WIFEXITED(child_status))
			return_code = WEXITSTATUS(child_status);
		else if (WIFSIGNALED(child_status))
			return_code = WTERMSIG(child_status);
	}
	return output;
}
char *get_output2(char *full_path, char *filename, char *arg1, char *arg2, int32_t fileno, int32_t discard) {
	int std_pipe[2];
	char *output = NULL;
	if (pipe(std_pipe) == -1) {
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	pid_t child_pid = fork();
	if (child_pid == -1) {
		perror("Failed to fork");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		if (dup2(std_pipe[1], fileno) == -1) {
			perror("Failed to redirect stderr");
			exit(EXIT_FAILURE);
		}
		int dev_null_fd = open("/dev/null", O_WRONLY);
        if (dev_null_fd == -1) {
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
	} else {
		close(std_pipe[1]);
		int child_status;
		waitpid(child_pid, &child_status, 0);
		char buffer[256];
		ssize_t read_size;
		int len;
		len = 0;
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
	return output;
}

char	*get_std_err(char *executable)
{
	char	*output;
	int		stderr_pipe[2];
	char	buffer[256];
	ssize_t	read_size;
	int32_t	len;
	int		exit_status;


	output = NULL;
	if (pipe(stderr_pipe) == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	if (dup2(stderr_pipe[1], STDERR_FILENO) == -1)
	{
		perror("Failed to redirect stderr");
		exit(EXIT_FAILURE);
	}
	close(stderr_pipe[1]);
	exit_status = system(executable);
	len = 0;
	while ((read_size = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0)
	{
		output = realloc(output, len + read_size + 1);
		if (output == NULL)
		{
			perror("realloc failed");
			exit(EXIT_FAILURE);
		}
		strcpy(output + len, buffer);
		len += read_size;
	}
	close(stderr_pipe[0]);
	if (exit_status == -1)
	{
		perror("Failed to execute command");
	}
	else
	{
		if (WIFEXITED(exit_status))
			return_code = WEXITSTATUS(exit_status);
	}
	return (output);
}

char	*get_std_out(char *executable)
{
	char	*output;
	int		std_pipe[2];
	char	buffer[256];
	ssize_t	read_size;
	int32_t	len;
	int		exit_status;

	output = NULL;
	if (pipe(std_pipe) == -1)
	{
		perror("Failed to create pipe");
		exit(EXIT_FAILURE);
	}
	if (dup2(std_pipe[1], STDOUT_FILENO) == -1)
	{
		perror("Failed to redirect stderr");
		exit(EXIT_FAILURE);
	}
	close(std_pipe[1]);
	exit_status = system(executable);
	if (exit_status == -1)
	{
		perror("Failed to execute command");
	}
	else
	{
		if (WIFEXITED(exit_status))
			return_code = WEXITSTATUS(exit_status);
	}
	len = 0;
	while ((read_size = read(std_pipe[0], buffer, sizeof(buffer))) > 0)
	{
		output = realloc(output, len + read_size + 1);
		if (output == NULL)
		{
			perror("realloc failed");
			exit(EXIT_FAILURE);
		}
		strcpy(output + len, buffer);
		len += read_size;
	}
	close(std_pipe[0]);
	return (output);
}
char* replaceString(const char* input, const char* search, const char* replace) {
    // Calculate the length of the resulting string
    size_t inputLen = strlen(input);
    size_t searchLen = strlen(search);
    size_t replaceLen = strlen(replace);
    size_t resultLen = inputLen;
    size_t count = 0;

    // Count the number of occurrences of the search string
    const char* p = input;
    while ((p = strstr(p, search)) != NULL) {
        count++;
        p += searchLen;
    }

    // Calculate the length of the resulting string after replacement
    resultLen += (replaceLen - searchLen) * count;

    // Allocate memory for the resulting string
    char* result = (char*)malloc((resultLen + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Copy the input string, replacing occurrences of the search string with the replace string
    const char* inputPtr = input;
    char* resultPtr = result;
    while (count > 0) {
        const char* match = strstr(inputPtr, search);
        if (match == NULL) {
            // No more occurrences, copy the remaining part of the input string
            strcpy(resultPtr, inputPtr);
            break;
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

    return result;
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

void	run_test(char *command2)
{
	// char		bash_cmd[MAX_COMMAND_LENGTH];
	// char		minishell_command[MAX_COMMAND_LENGTH];
	char		*bash_output;
	char		*bash_err_output;
	char		*minishell_output;
	char		*minishell_err_output;
	// char		*command;
	int			bash_status;
	int			minishell_status;
	int			output_equal;
	int			output_err_equal;
	
	char cwd[1024]; // Buffer to store the current directory path

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
		exit(1);
    }

	bash_output = NULL;
	minishell_output = NULL;
	output_err_equal = 0;
	char *full_name = ft_strjoin(cwd, "/minishell");
	
	// snprintf(bash_cmd, MAX_COMMAND_LENGTH, "%s", command);
	bash_err_output = get_output2("bash", "bash", "-c", command2, STDERR_FILENO, STDOUT_FILENO);
	
	// snprintf(bash_cmd, MAX_COMMAND_LENGTH, "%s", command2);
	bash_output = get_output2("bash", "bash", "-c", command2, STDOUT_FILENO, STDERR_FILENO);
	bash_status = return_code;
	
	// snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s", command2);
	minishell_err_output = get_output2(full_name, "./minishell", command2, NULL, STDERR_FILENO, STDOUT_FILENO);

	// snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s", command2);
	minishell_output = get_output2(full_name, "./minishell", command2, NULL, STDOUT_FILENO, STDERR_FILENO);
	minishell_status = return_code;
	output_equal = 0;
	
	bash_err_output = clean_output(bash_err_output);

	if (minishell_output && bash_output)
		output_equal = (int)(strcmp(bash_output, minishell_output) == 0);
	else if (!minishell_output && !bash_output)
		output_equal = 1;
	else if (!minishell_output || !bash_output)
		output_equal = 0;
	if (minishell_err_output && bash_err_output)
		output_err_equal = (int)(strcmp(bash_err_output, minishell_err_output) == 0);
	else if (!minishell_err_output && !bash_err_output)
		output_err_equal = 1;
	else if (!minishell_err_output || !bash_err_output)
		output_err_equal = 0;
	if ((output_equal && output_err_equal))
	{
		printf(GREEN "TEST PASSED!\n\n" RESET);
		printf("Bash output:\n[%s]\n", bash_output);
		printf("Bash err output:\n[%s]\n", bash_err_output);
		printf("Bash exit status: %d\n\n", bash_status);
		printf("Minishell output:\n[%s]\n", minishell_output);
		printf("Minishell err output:\n[%s]\n", minishell_err_output);
		printf("Minishell exit status: %d\n\n", minishell_status);
	}
	else
	{
		printf(RED "TEST FAILED!\n\n" RESET);
		printf("Bash output:\n[%s]\n", bash_output);
		printf("Bash err output:\n[%s]\n", bash_err_output);
		printf("Bash exit status: %d\n\n", bash_status);
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

	file = fopen("./tests.txt", "r");
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
