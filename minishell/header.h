#ifndef MAIN_H                  // Header guard start
#define MAIN_H                  // Header guard define

#define SUCCESS 0               // Success value
#define FAILURE -1              // Failure value

#include <stdio.h>              // Input output functions
#include <string.h>             // String functions
#include <stdlib.h>             // Memory functions
#include <stdio_ext.h>          // Extended stdio
#include <sys/types.h>          // System types
#include <sys/stat.h>           // File status
#include <fcntl.h>              // File control
#include <unistd.h>             // System calls
#include <signal.h>             // Signal handling
#include <sys/wait.h>           // Wait functions

extern char *ext_cmd[152];      // External commands list

typedef int data_t;             // Integer type alias

typedef struct node             // Linked list node
{
	data_t pid;                  // Process id
    char comd[50];               // Command name
	struct node *link;           // Next node
} Slist;                        // Structure name

#define BUILTIN 1               // Built-in command
#define EXTERNAL 2              // External command
#define NO_COMMAND 3            // Invalid command

#define ANSI_COLOR_RED "\x1b[31m"      // Red color
#define ANSI_COLOR_GREEN "\x1b[32m"    // Green color
#define ANSI_COLOR_YELLOW "\x1b[33m"   // Yellow color
#define ANSI_COLOR_BLUE "\x1b[34m"     // Blue color
#define ANSI_COLOR_MAGENTA "\x1b[35m"  // Magenta color
#define ANSI_COLOR_CYAN "\x1b[36m"     // Cyan color
#define ANSI_COLOR_RESET "\x1b[0m"     // Reset color

void scan_input(char *prompt, char *input_string);   // Read input
char *get_command(char *input_string);               // Get command

void copy_change(char *prompt, char *input_string);  // Change prompt

int check_command_type(char *command);               // Check command type
void echo(char *input_string, int status);           // Echo command
void execute_internal_commands(char *input_string);  // Built-in execution
void signal_handler(int sig_num);                     // Signal handler
void extract_external_commands(char **external_commands); // Load commands
void execute_external_commands(char *input_string);  // External execution

int insert_at_first(Slist **head, data_t pid, char comd[]); // Insert node
int sl_delete_first(Slist **);                              // Delete node
void print_list(Slist *head);                               // Print list

#endif                           // Header guard end
