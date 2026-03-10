#include "header.h"              // Include header file

char cmd[20];                   // Command buffer

char *builtins[] = 
{             // Built-in commands list
    "echo","printf","read","cd","pwd","pushd","popd","dirs","let","eval","set","unset","export","declare","typeset","readonly",
    "getopts","source","exit","exec","shopt","caller","true","type","hash","bind","help","jobs","fg","bg", NULL               // End of list
};

//char *ext_cmd[152];            // External commands (unused)
extern Slist *head;              // Job list head
int status;                      // Process status

char *get_command(char *input_string) // Extract command
{
	int i = 0, j = 0;             // Index variables
    while (input_string[i] != '\0' && input_string[i] != ' ') // Until space
	{
        cmd[j++] = input_string[i++]; // Copy character
	}

    cmd[j] = '\0';               // End string
    return cmd;                  // Return command
}

int check_command_type(char *cmmd) // Check command type
{
	int i = 0;                   // Index
	while(builtins[i] != NULL)   // Check built-ins
	{
		if(strcmp(builtins[i], cmmd) == 0) // Match found
		{
			return BUILTIN;       // Return built-in
		}
		i++;                      // Next command
	}
	int j = 0;                   // Index
	while(ext_cmd[j] != NULL)    // Check externals
	{
		if(strcmp(ext_cmd[j], cmmd) == 0) // Match found
		{
			return EXTERNAL;      // Return external
		}
		j++;                      // Next command
	}
	return NO_COMMAND;           // Command not found
}

void extract_external_commands(char **external_commands) // Load external commands
{
	FILE *fptr = fopen("ext_cmd.txt", "r"); // Open file
	if(fptr == NULL)            // File open check
	{
		return;                 // Exit function
	}
	char temp[50];              // Temp buffer
	int i = 0;                  // Index
	while(i < 152 && fscanf(fptr, "%50s", temp) == 1) // Read commands
	{
		external_commands[i] = malloc(strlen(temp) + 1); // Allocate memory
		strcpy(external_commands[i], temp);              // Copy command
		i++;                        // Next index
	}
	external_commands[i] = NULL; // End array
}

void execute_internal_commands(char *input_string) // Run built-in commands
{
    Slist Sig;                  // Job structure
	if(strcmp(input_string, "exit") == 0) // Exit command
	{
		exit(0);                // Exit shell
	}
	char buff[100];             // Path buffer
	if(strcmp(input_string, "pwd") == 0) // PWD command
	{
		getcwd(buff, 100);      // Get path
		printf("%s\n", buff);   // Print path
	}
	if(strncmp(input_string, "cd", 2) == 0) // CD command
	{
		chdir(input_string + 3); // Change directory
		getcwd(buff, 100);       // Update path
	}
	else if(strcmp(input_string,"echo $$")==0) // Echo PID
	{
		printf("%d\n",getpid()); // Print PID
	}
	else if(strcmp(input_string,"echo $?")==0) // Echo status
	{
		if(WIFEXITED(status))   // Normal exit
		{
			printf("%d\n",WEXITSTATUS(status)); // Print status
		}
		else 
		{
			printf("%d\n",WEXITSTATUS(status)); // Print status
		}	
	}
	else if(strcmp(input_string,"echo $SHELL")==0) // Echo shell
	{
		printf("%s\n",getenv("SHELL")); // Print shell path
	}
    else if(strcmp(input_string, "jobs") == 0) // Jobs command
    {
        print_list(head);        // Print jobs
    }
    else if(strcmp(input_string, "fg") == 0) // Foreground command
    {
        if (head != NULL)        // Job exists
        {
            kill(Sig.pid, SIGCONT); // Continue job
            waitpid(Sig.pid, &status, WUNTRACED); // Wait job
            sl_delete_first(&head); // Remove job
        }
    }
    else if(strcmp(input_string, "bg") == 0) // Background command
    {
        signal(SIGCHLD, signal_handler); // Handle child exit
        kill(Sig.pid, SIGCONT);          // Continue job
        if (sl_delete_first(&head))      // Delete job
        {
            // Failure ignored
        }
    }
}

void execute_external_commands(char *input_string) // Execute external commands
{
    char *argv[20];              // Argument list
    int argc = 0;                // Argument count

    char *token = strtok(input_string, " "); // Tokenize input
    while (token != NULL)        // Loop tokens
    {
        argv[argc++] = token;   // Store argument
        token = strtok(NULL, " "); // Next token
    }
    argv[argc] = NULL;           // End arguments

    int pipe_count = 0;          // Pipe count
    for (int i = 0; i < argc; i++) // Loop args
    {
        if (strcmp(argv[i], "|") == 0) // Pipe found
            pipe_count++;        // Increase count
    }

    if (pipe_count == 0)         // No pipe
    {
        execvp(argv[0], argv);  // Execute command
        perror("execvp");       // Error message
        exit(1);                // Exit on failure
    }

    int cmd[10];                 // Command index list
    int cmd_count = 0;           // Command count

    cmd[cmd_count++] = 0;        // First command
    for (int i = 0; i < argc; i++) // Parse pipes
    {
        if (strcmp(argv[i], "|") == 0) // Pipe found
        {
            argv[i] = NULL;      // Split command
            cmd[cmd_count++] = i + 1; // Next command
        }
    }

    int prev_fd = -1;            // Previous pipe
    int pipefd[2];               // Pipe descriptors

    for (int i = 0; i < cmd_count; i++) // Loop commands
    {
        if (i < cmd_count - 1)
            pipe(pipefd);        // Create pipe

        pid_t pid = fork();      // Create process

        if (pid == 0)            // Child process
        {
            if (prev_fd != -1)   // Previous pipe exists
            {
                dup2(prev_fd, STDIN_FILENO); // Read pipe
                close(prev_fd);  // Close fd
            }

            if (i < cmd_count - 1) // Next pipe exists
            {
                dup2(pipefd[1], STDOUT_FILENO); // Write pipe
                close(pipefd[0]); // Close read end
                close(pipefd[1]); // Close write end
            }

            execvp(argv[cmd[i]], &argv[cmd[i]]); // Execute command
            perror("execvp");    // Error message
            exit(1);             // Exit child
        }
        else                     // Parent process
        {
            if (prev_fd != -1)
                close(prev_fd);  // Close old fd

            if (i < cmd_count - 1)
            {
                close(pipefd[1]); // Close write end
                prev_fd = pipefd[0]; // Save read end
            }
        }
    }

    for (int i = 0; i < cmd_count; i++) // Wait children
        wait(NULL);               // Wait process
}
