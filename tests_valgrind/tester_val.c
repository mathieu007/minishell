#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

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

char	*ft_strjoinfree(char *s1, char const *s2)
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

void print_exit_status(int bash_status, int minishell_status)
{
    printf("Bash exit status: %d\n", bash_status);
    printf("Minishell exit status: %d\n", minishell_status);
}

int compare_files(const char *file1, const char *file2)
{
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");

    if (fp1 == NULL || fp2 == NULL)
    {
        perror("Error opening files for comparison");
        return -1;
    }

    int equal = 1;
    int ch1, ch2;

    while ((ch1 = fgetc(fp1)) != EOF && (ch2 = fgetc(fp2)) != EOF)
    {
        if (ch1 != ch2)
        {
            equal = 0;
            break;
        }
    }

    if (ch1 != EOF || ch2 != EOF)
        equal = 0;

    fclose(fp1);
    fclose(fp2);

    return equal;
}

void run_test(char **command2)
{
		char minishell_command[MAX_COMMAND_LENGTH];
		char *minishell_output = NULL;
		char line[MAX_COMMAND_LENGTH];
		size_t minishell_output_len = 0;
		FILE *minishell_fd;

		*command2 = replaceString(*command2, "'", "'\\''");
		snprintf(minishell_command, MAX_COMMAND_LENGTH, "valgrind --track-fds=yes --leak-check=full --track-origins=yes --show-reachable=yes ./minishell '%s'", *command2);
		free(*command2);
		command2++;
		while (*command2)
		{
			*command2 = replaceString(*command2, "'", "'\\''");
			snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s '%s'", &minishell_command[0], *command2);
			free(*command2);
			command2++;
		}
		snprintf(minishell_command, MAX_COMMAND_LENGTH, "%s 2>&1 | grep -e 'lost: ' -e 'still reachable: '", &minishell_command[0]);
		printf("minishell_command:%s", minishell_command);
		minishell_fd = popen(minishell_command, "r");
		if (minishell_fd == NULL)
		{
			perror("popen minishell_command");
			exit(EXIT_FAILURE);
		}	
		while (fgets(line, sizeof(line), minishell_fd) != NULL)
		{
			size_t line_len = strlen(line);
			minishell_output = realloc(minishell_output, minishell_output_len + line_len + 1);
			if (minishell_output == NULL)
			{
				perror("realloc minishell_output");
				exit(EXIT_FAILURE);
			}
			strcpy(minishell_output + minishell_output_len, line);
			minishell_output_len += line_len;
		}
		int minishell_status = pclose(minishell_fd);
		printf("\n%s\n status:%i\n", minishell_output, minishell_status);
		free(minishell_output);
}

static int	iswhitespace(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}


int	ft_atoi(const char *str)
{
	int	neg;
	int	n;

	neg = 1;
	n = 0;
	while (*str != '\0' && iswhitespace(*str) == 1)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			neg = -1;
		str++;
	}
	while (ft_isdigit(*str) == 1)
		n = (n * 10) + (*str++ - 48);
	return (n * neg);
}

int main(int32_t argc, char **argv)
{
	(void) argc;
	char **commands;
	int32_t cmd_count = ft_atoi(argv[1]);
	int32_t i;
	commands = malloc(cmd_count + 1);
	commands[cmd_count] = NULL;
    FILE *file;
    char line[MAX_COMMAND_LENGTH];
    file = fopen("./tests_valgrind.txt", "r");
    if (file == NULL)
    {
        perror("fopen");
        return (EXIT_FAILURE);
    }
	i = 0;
    while (fgets(line, sizeof(line), file) != NULL)
    {
		if (i == cmd_count)
		{
			printf("LEAKS TESTS--------------------------------------------------------\n");
			i = 0;
			while (i < cmd_count)
				printf("%s\n", commands[i++]);
			run_test(commands);
			i = 0;
		}
        line[strcspn(line, "\n")] = '\0'; 
		commands[i] = line;
		i++;
	}
	free(commands);
    fclose(file);
    return (EXIT_SUCCESS);
}
