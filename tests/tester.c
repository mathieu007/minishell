#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>


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

void run_test(const char *command)
{

        char bash_cmd[MAX_COMMAND_LENGTH];
		char minishell_command[MAX_COMMAND_LENGTH];
		char *bash_output = NULL;
		char *minishell_output = NULL;
		char *bash_output2 = NULL;
		char *minishell_output2 = NULL;
		char line[MAX_COMMAND_LENGTH];
		size_t bash_output_len = 0;
		size_t minishell_output_len = 0;
		FILE *bash_pipe;
		FILE *minishell_fd;
		// char *command_cpy;

		// command_cpy = strdup(command);
		// Construct the command to run in Bash
		command = replaceString(command, "'", "'\\''");
		snprintf(bash_cmd, MAX_COMMAND_LENGTH, "bash -c '%s' 2>&1", command);
		// Construct the command to run in Minishell
		snprintf(minishell_command, MAX_COMMAND_LENGTH, "./minishell '%s' 2>&1", command);

		// Open a pipe to capture the output of the Bash command
		bash_pipe = popen(bash_cmd, "r");	
		if (bash_pipe == NULL)
		{
			perror("popen bash_command");
			exit(EXIT_FAILURE);
		}
		// Read the output of the Bash command   
		while (fgets(line, sizeof(line), bash_pipe) != NULL)
		{
			bash_output = realloc(bash_output, bash_output_len + strlen(line) + 1);
			if (bash_output == NULL)
			{
				perror("realloc bash_output");
				exit(EXIT_FAILURE);
			}
			strcpy(bash_output + bash_output_len, line);
			bash_output_len += strlen(line);
		}
		int bash_status = pclose(bash_pipe);

		minishell_fd = popen(minishell_command, "r");
		if (minishell_fd == NULL)
		{
			perror("popen minishell_command");
			exit(EXIT_FAILURE);
		}	
		usleep(500);
		// Read the output of the Minishell command
		while (fgets(line, sizeof(line), minishell_fd) != NULL)
		{
			minishell_output = realloc(minishell_output, minishell_output_len + strlen(line) + 1);
			if (minishell_output == NULL)
			{
				perror("realloc minishell_output");
				exit(EXIT_FAILURE);
			}
			strcpy(minishell_output + minishell_output_len, line);
			minishell_output_len += strlen(line);
		}
		int minishell_status = pclose(minishell_fd);
		// Close the pipes
			

		// Compare the outputs
		int output_equal = 0;
		if (minishell_output)
			minishell_output2 = minishell_output;
		if (bash_output)	
			bash_output2 = bash_output;
		if (minishell_output && bash_output)
			output_equal = (strcmp(bash_output, minishell_output) == 0);

		// Print the test result
		if ((output_equal || (!minishell_output && !bash_output)) && WEXITSTATUS(bash_status) == WEXITSTATUS(minishell_status))
		{
			printf(GREEN "TEST PASSED!\n\n" RESET);
			printf("Bash exit status: %d\n", WEXITSTATUS(bash_status));
			printf("Minishell exit status: %d\n\n", WEXITSTATUS(minishell_status));
		}
		else
		{
			printf(RED "TEST FAILED!\n\n" RESET);
			printf("Bash output:\n[%s]\n", bash_output2);
			printf("Bash exit status: %d\n\n", WEXITSTATUS(bash_status));
			printf("Minishell output:\n[%s]\n", minishell_output2);
			printf("Minishell exit status: %d\n\n", WEXITSTATUS(minishell_status));
		}
		// Free allocated memory
		free(bash_output);
		free(minishell_output);
		// free(bash_output2);
		// free(minishell_output2);
    
}


int main(void)
{
    FILE *file;
    char line[MAX_COMMAND_LENGTH];

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
