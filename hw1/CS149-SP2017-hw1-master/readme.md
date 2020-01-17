

# CS149 Spring 2017 HW1
## Question 1
 Including the initial parent process, how many processes are created by the program? And what is the valueof i before each process terminates? Please draw the parent-child process hierarchy to justify your answer.
 ```c
#include<stdio.h>
#include<unistd.h>
int main() 
{
    int i = 0;
    if (fork() == 0) 
    {
        ++i;
        if (fork() != 0)
        {
            i = 3;
        }
        else
        {
            ++i;
        }
        fork();
    } else 
    {
        i = 5;
    if (fork() == 0)
        ++i;
    }
    return 0;
}//main
```
## Question 2
Design a C program to serve as a shell interface that accepts user commands andthen executes each command in a separate process. HW1 can be completed on Linux or Linux VM.
A shell interface gives the user a prompt, after which the next command is entered. The format of the prompt is
```sh
FirstName-L3SID>
```
where L3SID is the last three digits of your student ID. Assuming your first name is demo and L3SID is 123, the examplebelow illustrates the prompt (demo-123>) and the user’s next command: cat prog.c (which displays the fileprog.c on the terminal using the UNIX cat command).
```sh
demo-123> cat prog.c
```
The shell process first prints the prompt, reads what the user enters on the command line (in the above case, cat prog.c), and then creates a separate child process that performs the command.
Unless otherwise specified, the shell(parent) process waits for the child to exit before printing the next prompt and continuing. However, UNIX shells typically also allow the child process to run in the background, or concurrently. 
To accomplish this, we add an ampersand (&) at the end of the command. Thus, if we rewrite the above command as
```sh
demo-123> cat prog.c &
```
the shell (parent) and child processes will run concurrently. 
In both cases the separate child process is created using the fork() system call, and the user’s command is executed using one of the system calls in the exec() family (asdescribed in Section 3.3.1).
For any command running in the background, the shell (parent) process does not wait for the completion of the child process; the shell prints the prompt and reads the next command immediately.
A C program that provides the general operations of a command-line shell is as follows.
```c
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#define MAXLINE 80 /* The maximum length command */
int main(void)
{
char *args[MAXLINE/2 + 1]; /* command line with max 40 arguments */
int should_run = 1; /* flag to determine when to exit program */
printf("CS149 Shell from FirstName LastName\n"); /* replace w/ name */
while (should_run) { CS149 HW1
SPRING 2017
2
printf("FirstName-L3SID>"); /* prompt- replace FirstName and L3SID */
fflush(stdout);
/* After reading user input, the steps are:
*(1) fork a child process using fork()
*(2) the child process will invoke execvp()
*(3) if command included &, parent will NOT invoke wait()
*/
}
return 0;
}
```
The main() function presents the prompt and outlines the steps to be taken after input from the user has been read.
The main() function continually loops as long as should_run equals 1; when the user enters exit at the prompt,your shell will set should_run to 0 and terminate.
You should modify the main() function so that a child process is forked and executes the command specified by theuser. 
This will require parsing what the user has entered into separate tokens and storing the tokens in an array of character strings args. 
For example, if the user enters the command `ps –af` at the prompt, the values stored in theargs array are:
```c
args[0] = "ps"
args[1] = "-af"
args[2] = NULL
```
This args array will be passed to the execvp() function, which has the following prototype:
```c
execvp(char *command, char *params[]);
```
Here, command represents the command to be performed and params stores the parameters to this command. ForQ2, the execvp() function should be invoked as execvp(args[0], args). 
Be sure to check whether the userincluded an & at the tail end of command line to determine whether or not the shell (parent) process is to wait for thechild to exit. 
The character &, if it exists at the tail end of a command line, is only meaningful to the shell and is not anargument of the command.
When fork() or execvp() failed, the shell should print out error messages, output the prompt and accept the nextcommand from user.
The shell in Q2 does not need to support any additional functions such as pipe, and I/O redirection.
To read a line from the terminal (i.e., stdin), you may utilize readline(3), fgets(3) on Linux, or any other APIs. To extract tokens from a string, you may use strtok(3), strtok_r(3), or any other APIs, or write your own parsing routine.
Please make sure for each invocation the program always prints out “CS149 Shell from …” only once.Screenshots must include “CS149 Shell from …” from the program. Please follow the format for the promptwhich includes FirstName and L3SID.
Compile your program with “gcc –o shell shell.c”. You can execute the program with “./shell”.
Once you are done with the simple shell, execute the following steps in a “terminal”:
### Step a:
- Bring up your shell: `./shell`
- Within the shell, invalid command (print error msg): `xyz`
- Within the shell, get current date: `date`
- Within the shell, list current directory:`ls`
- Within the shell, sleep 5 minutes in the background: `sleep 5m &`
- Within the shell, show processes:`ps –af` 
- Within the shell, exit the shell:`exit`
- Show processes:`ps –af`
- Take screenshot of the entire sequence (which includes “CS149 Shell from …” from your program)
### Step b:
In step a, the output from two executions of “ps –af” should include the same process “sleep 5m” withidentical PID but with different parent PID (PPID). Explain the reason.
## Submission
Submit the following files as individual files (do not zip them together):
- CS149_HW1_YourName_L3SID (.pdf, .doc, or .docx), which includes
- Q1: answers and justification
- Q2: embeds screenshots from step a, and explanation in step b. - Screenshots that are not readable, orscreenshot without “CS149 - Shell from …” from the program, will receive 0 point for the entirehomework.
- shell_YourName_L3SID.c Please ident your source code and include comments . 
- Also please follow theexact format for the prompt as specified. Any submission without the proper FirstName and L3SID in theprompt will receive 0 point for the entire homework.

The ISA and/or instructor leave feedback to your homework as comments and/or in Crocodoc of your submission. To access Crocodoc, click “view feedback” button. For details, see the following URL:
http://guides.instructure.com/m/4212/l/106690-how-do-i-use-the-submission-details-page-for-an-assignment
 see your updates!




