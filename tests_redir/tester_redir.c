#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "../libft/include/libft.h"

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

int truncate_file(const char* filename, off_t length) {
    int result = truncate(filename, length);
    if (result == -1) {
        perror("Failed to truncate file");
        return -1;
    }
    return 0;
}

int compare_files(const char* filename1, const char* filename2) {
    FILE* file1 = fopen(filename1, "r");
    FILE* file2 = fopen(filename2, "r");

    if (file1 == NULL || file2 == NULL) {
        perror("Failed to open files");
		if (!file1)
			printf("file: %s\n", filename1);
		if (!file2)
			printf("file: %s\n", filename2);	
        return -1;
    }

    int equal = 1;
    int ch1 = 0;
	int ch2 = 0;

    while ((ch1 = fgetc(file1)) && (ch2 = fgetc(file2))) {
		if (ch1 == EOF || ch2 == EOF)
			break;
        if (ch1 != ch2) {
			
            equal = 0;
            break;
        }
    }

    if (ch1 != ch2) {
        equal = 0;
    }

    fclose(file1);
    fclose(file2);

    return equal;
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

void run_test(const char *command2)
{
		int i = 0;
        char bash_cmd[MAX_COMMAND_LENGTH];
		char minishell_command[MAX_COMMAND_LENGTH];
		char *bash_output = NULL;
		char *temp_bashoutput = NULL;
		char *minishell_output = NULL;
		char line[MAX_COMMAND_LENGTH];
		size_t bash_output_len = 0;
		size_t minishell_output_len = 0;
		FILE *bash_pipe;
		FILE *minishell_fd;
		const char *command = replaceString(command2, "'", "'\\''");
		char	*command_minishell = replaceString(command, "outfile", "outfile_minishell");
		char	*command_bash = replaceString(command, "outfile", "outfile_bash");

		snprintf(bash_cmd, MAX_COMMAND_LENGTH, "bash -c '%s' 2>&1", command_bash);
		snprintf(minishell_command, MAX_COMMAND_LENGTH, "./minishell '%s' 2>&1", command_minishell);

		bash_pipe = popen(bash_cmd, "r");
		if (bash_pipe == NULL)
		{
			perror("popen bash_command");
			exit(EXIT_FAILURE);
		}
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
			pclose(bash_pipe);
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
		i = 1;
		char *filename1;
		char *filename2;
		char *num;
		int files_content_equals = 1;
		int cur_files_content_equals = 1;
		off_t new_size = 0;

		while (i < 10)
		{	
			num = ft_itoa(i);
			filename1 = ft_strjoin("outfile_minishell", num);
			filename2 = ft_strjoin("outfile_bash", num);
			free(num);
			if (files_content_equals == 1)
			{
				cur_files_content_equals = compare_files(filename1, filename2);
				if (cur_files_content_equals == 0)
					printf("file mismatch %s : %s\n", filename1, filename2);
				files_content_equals = cur_files_content_equals;
			}
			truncate_file(filename1, new_size);
			truncate_file(filename2, new_size);
			free(filename1);
			free(filename2);
			i++;
		}
		int output_equal = 0;
		temp_bashoutput = bash_output;
		if (bash_output)
		{
			bash_output = replaceString(bash_output, "bash: ", "");
			bash_output = replaceString(bash_output, "line 1: ", "");
			free(temp_bashoutput);
		}			
		if (minishell_output && bash_output)
			output_equal = (strcmp(bash_output, minishell_output) == 0);
			
		// Print the test result
		if (files_content_equals && (output_equal || (!minishell_output && !bash_output)) && WEXITSTATUS(bash_status) == WEXITSTATUS(minishell_status))
		{
			printf(GREEN "TEST PASSED!\n\n" RESET);
			printf("Bash output:\n[%s]\n", bash_output);
			printf("Bash exit status: %d\n", WEXITSTATUS(bash_status));
			printf("Minishell output:\n[%s]\n", minishell_output);
			printf("Minishell exit status: %d\n\n", WEXITSTATUS(minishell_status));
		}
		else
		{
			printf(RED "TEST FAILED!\n\n" RESET);
			printf("Bash output:\n[%s]\n", bash_output);
			printf("Bash exit status: %d\n\n", WEXITSTATUS(bash_status));
			printf("Minishell output:\n[%s]\n", minishell_output);
			if (files_content_equals != 1)
				printf("Minishell out file differ\n");
			printf("Minishell exit status: %d\n\n", WEXITSTATUS(minishell_status));
		}
		free(bash_output);
		free(minishell_output);
}

int main(void)
{
    FILE *file;
    char line[MAX_COMMAND_LENGTH];

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
