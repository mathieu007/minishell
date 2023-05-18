#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char bash_command[MAX_COMMAND_LENGTH];
    char minishell_command[MAX_COMMAND_LENGTH];
    char *bash_output = NULL;
    char *minishell_output = NULL;
    size_t bash_output_len = 0;
    size_t minishell_output_len = 0;
    FILE *bash_pipe;
    FILE *minishell_pipe;

    // Construct the command to run in Bash
    snprintf(bash_command, MAX_COMMAND_LENGTH, "bash -c \"%s\" 2>&1", command);
    // Construct the command to run in Minishell
    snprintf(minishell_command, MAX_COMMAND_LENGTH, "../bin/minishell \"%s\" 2>&1", command);

    // Open a pipe to capture the output of the Bash command
    bash_pipe = popen(bash_command, "r");
    if (bash_pipe == NULL)
    {
        perror("popen bash_command");
        exit(EXIT_FAILURE);
    }

    // Open a pipe to capture the output of the Minishell command
    minishell_pipe = popen(minishell_command, "r");
    if (minishell_pipe == NULL)
    {
        perror("popen minishell_command");
        pclose(bash_pipe); // Close the Bash pipe before exiting
        exit(EXIT_FAILURE);
    }

    // Read the output of the Bash command
    char line[MAX_COMMAND_LENGTH];
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

    // Read the output of the Minishell command
    while (fgets(line, sizeof(line), minishell_pipe) != NULL)
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

    // Close the pipes
    int bash_status = pclose(bash_pipe);
    int minishell_status = pclose(minishell_pipe);


    // Compare the outputs
    int output_equal = (strcmp(bash_output, minishell_output) == 0);

    // Print the test result
    if (output_equal)
    {
        printf(GREEN "TEST PASSED!\n\n" RESET);
    printf("Bash exit status: %d\n", WEXITSTATUS(bash_status));
    printf("Minishell exit status: %d\n\n", WEXITSTATUS(minishell_status));
    }
    else
    {
        printf(RED "TEST FAILED!\n\n" RESET);
        printf("Bash output:\n%s\n", bash_output);
    printf("Bash exit status: %d\n\n", WEXITSTATUS(bash_status));
        printf("Minishell output:\n%s\n", minishell_output);
    printf("Minishell exit status: %d\n", WEXITSTATUS(minishell_status));
    }

    // Free allocated memory
    free(bash_output);
    free(minishell_output);
}


int main(void)
{
    FILE *file;
    char line[MAX_COMMAND_LENGTH];

    file = fopen("tester.txt", "r");
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
