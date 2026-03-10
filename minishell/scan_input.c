#include "header.h"                 // Include header file

char *ext_cmd[152];                 // External commands array
int pid = 0;                        // Process id
extern char prompt[25];             // Shell prompt
extern int status;                  // Process status
char *command;                     // Current command
Slist *head = NULL;                 // Job list head

void scan_input(char *prompt, char *input_string) // Input handling function
{
    extract_external_commands(ext_cmd); // Load external commands
    signal(SIGINT, signal_handler);     // Handle Ctrl+C
    signal(SIGTSTP, signal_handler);    // Handle Ctrl+Z
    while(1)                            // Infinite shell loop
    {
        char str[20];                  // Temp string
        printf(ANSI_COLOR_MAGENTA"\n%s "ANSI_COLOR_RESET,prompt); // Print prompt
        scanf("%[^\n]", input_string); // Read input
        getchar();                     // Clear newline
        if(strncmp(input_string, "PS1=", 4) == 0) // Check prompt change
        {
            strncpy(str, input_string + 4, sizeof(str) - 1); // Copy prompt
            str[sizeof(str) - 1] = '\0'; // Null terminate
            if(strchr(str, ' ') != NULL) // Check space
            {
                int len = strlen(str); // Get length
                if(str[len - 1] == ' ' && str[0] != ' ') // Valid prompt
                {
                    strcpy(prompt, str); // Update prompt
                }
                else
                {
                    printf("%s command not found\n", str); // Invalid prompt
                }
            }
            else
            {
                strcpy(prompt, str); // Update prompt
            }
        }
        else
        {
            command = get_command(input_string); // Extract command
            int type = check_command_type(command); // Check command type
            
            //printf("%d ", type); // Debug print
            if(type == BUILTIN) // Built-in command
            {
                execute_internal_commands(input_string); // Execute built-in
            }
            else
            {
                pid = fork(); // Create child
                if(pid > 0)
                {
                    waitpid(pid, &status, WUNTRACED); // Wait for child
                    pid = 0; // Reset pid
                }
                else if(pid == 0)
                {
                    signal(SIGINT, SIG_DFL); // Default Ctrl+C
                    signal(SIGTSTP, SIG_DFL); // Default Ctrl+Z
                    execute_external_commands(input_string); // Execute command
                    exit(0); // Exit child
                }
            }
        }
    }
}

void signal_handler(int signum) // Signal handler
{
    //Slist *head = NULL;       // Commented code
    if(signum == SIGINT)       // Ctrl+C signal
    {
        if(pid == 0)           // No child running
        {        
            printf(ANSI_COLOR_MAGENTA"\n%s"ANSI_COLOR_RESET,prompt); // Print prompt
            fflush(stdout);    // Flush output
        }
    }
    else if(signum == SIGTSTP) // Ctrl+Z signal
    {
        if(pid == 0)           // No child
        {
            printf(ANSI_COLOR_MAGENTA"\n%s"ANSI_COLOR_RESET,prompt); // Print prompt
            fflush(stdout);    // Flush output
        }
        else
        {
            if(insert_at_first(&head, pid, command) == -1) // Store job
            {
                //printf("Info : Insertion Failure\n"); // Debug
            }
            if(signum == SIGCHLD) // Child exit signal
            {
                waitpid(-1, &status, WNOHANG); // Reap child
            }
        }
    }
}

int insert_at_first(Slist **head, data_t data, char comnd[30]) // Insert node
{
	Slist *new =(Slist *) malloc(sizeof(Slist)); // Allocate node
    if(new == NULL)        // Check allocation
    {
        return FAILURE;   // Return failure
    }
    new -> pid = data;    // Store pid
    strcpy(new -> comd, comnd); // Store command
    new -> link = NULL;   // Set link
    if(*head == NULL)     // Empty list
    {
        *head = new;      // Set head
        return SUCCESS;   // Return success
    }
    else
    {
        new -> link = *head; // Point to old head
        *head = new;         // Update head
    }
    return SUCCESS;        // Return success
}

int sl_delete_first(Slist **head) // Delete first node
{
    if(*head == NULL)     // Empty list
    {
        return FAILURE;   // Return failure
    }
    Slist *temp = *head; // Store head
    *head = temp -> link; // Move head
    free(temp);          // Free node
    return SUCCESS;      // Return success
}

void print_list(Slist *head) // Print list
{
	if (head == NULL)    // Empty list
	{
		//printf("INFO : List is empty\n"); // Debug
	}
    else
    {
	    while (head)	    // Traverse list
	    {
		    printf("%s %d \n", head -> comd, head -> pid); // Print data
		    head = head -> link; // Move next
	    }
    }
}
