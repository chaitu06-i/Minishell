/*******************************************************************************************************************************************************************
Name        : G. CHAITANYA GOWD
Title       : Mini Shell – Main Program
Description : Implements a basic Unix-like shell that accepts user commands,
              identifies built-in and external commands, and executes them.
              Supports features such as:
              - Custom shell prompt (PS1)
              - Built-in commands (cd, pwd, echo, jobs, fg, bg, exit)
              - Execution of external commands using fork() and execvp()
              - Pipe handling for multiple commands
              - Signal handling for SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z)
              - Background and foreground job control using a linked list

Prototype   : int main(void);

Inputs      : User input from standard input (command line)

Output      : Executes commands and displays output on standard output.
              Returns SUCCESS on normal execution.

*******************************************************************************************************************************************************************/

#include "header.h"            // Include header file

char prompt[25] = "minishell$: "; // Shell prompt
char input_string[50];           // Input buffer

int main()                       // Main function
{
    system("clear");             // Clear terminal
    scan_input(prompt, input_string); // Start shell
}


