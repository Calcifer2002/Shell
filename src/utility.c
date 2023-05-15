// Malavika Shanker, 20102232 - Operating Systems Project 1 * ca216/myshell
// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.
// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.
// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.
// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.
// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.
// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study.



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include "myshell.h"

void setEnv()
{
    char shell[100];
    strcat(shell, getenv("PWD")); // get the current environment variable and set it as shell variable
    strcat(shell, "/myshell");    // add both the shell variable and "/myshell"
    setenv("SHELL", shell, 1);
    setenv("PARENT", shell, 1); // add this line to set PARENT environment variable to the same value as SHELL
}

void quit()
{
    exit(0);
}
void pauseWork()
{
    printf("Paused - Press Enter to resume!"); // https://s-nako.work/2020/07/tipshow-to-detect-enter-key-pressed-in-c-c/
    while (getchar() != '\n')
        ; // read characters until a newline is encountered
    printf("resumed\n");
}
void help()
{
    // cat the readme file in manual folder and pipe the output to more
    system("cat manual/readme | more");

    // https://www.geeksforgeeks.org/more-command-in-linux-with-examples/
}

    void exec(char **args,  char *prompt)
{
    int i, background = 0; // flags
    int status;            // to check on bg processes
    pid_t pid;             // id of processes to make it easy for me to track

    char *input = NULL;  // input file name for < redirection
    char *output = NULL; // output file name for > redirection
    char *append = NULL; // append file name for >> redirection

    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            input = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            output = args[i + 1]; // a set of ifs to check for each operator.

            args[i] = NULL;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            append = args[i + 1];
            args[i] = NULL;
        }
        else if (strcmp(args[i], "&") == 0)
        {
            background = 1;
            args[i] = NULL;
        }
    }

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Could not Fork\n");
        exit(1);
    }
    else if (pid == 0)
    {
        if (input != NULL)
        {
            int file = open(input, O_RDONLY);
            if (file == -1)
            {
                fprintf(stderr, "Could not open input file\n");
                exit(1);
            }
            dup2(file, STDIN_FILENO); // replace stdin with input file
            close(file);
        }

        if (output != NULL)
        {
            int flags = O_WRONLY | O_CREAT | O_TRUNC; // https://pubs.opengroup.org/onlinepubs/7908799/xsh/open.html
            // if (strcmp(args[i - 2], ">>") == 0) {
            //     printf("meow");
            //     flags = flags| O_APPEND;
            // } else {                              this should have worked but didnt.
            //     printf("meow");
            //     flags = flags| O_TRUNC;
            // }
            int file = open(output, flags, 0666); // 0666 permissions
            if (file == -1)
            {
                fprintf(stderr, "Could not open output file\n");
                exit(1);
            }
            dup2(file, STDOUT_FILENO); // replace stdout with output file
            close(file);
        }
        if (append != NULL)
        {
            int flags = O_WRONLY | O_CREAT | O_APPEND;
            ;
            // if (strcmp(args[i - 2], ">>") == 0) {
            //     printf("meow");                   This should have worked but it is not.
            //     flags = flags| O_APPEND;
            // } else {
            //     printf("meow");
            //     flags = flags| O_TRUNC;
            // }
            int file = open(append, flags, 0666);
            if (file == -1)
            {
                fprintf(stderr, "Could not open output file\n");
                exit(1);
            }
            dup2(file, STDOUT_FILENO); // replace stdout with output file
            close(file);
        }

        if (execvp(args[0], args) == -1)
        {
           
             fprintf(stderr, "Command not found\n");
             exit(1);
        }
    }
    else
    {
        if (!background)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            printf("Process running in background with PID %d\n", pid);
            waitpid(pid, &status, WNOHANG); // this doesnt return the terminal but i did my best.
        }
    }
        
    }



extern char **environ;
void environGet()
{
    int i = 0;
    while (environ[i])
    {
        printf("%s\n", environ[i++]); // prints in form of "variable=value"
    }
}
// https://man7.org/linux/man-pages/man7/environ.7.html
void echo(char *args[])
{

    int i = 1;
    while (args[i] != NULL)
    {
        printf("%s ", args[i++]); // printing
    }
    printf("\n");
}
void dir(char **args)
{
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir("."); // if no argument given it will display ls of current dir

    if (dirp == NULL) {
        printf("Could not open dir %s\n", args[1]); // doesn't exist
        return;
    }

    while ((entry = readdir(dirp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            // exclude . and ..
            continue;
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dirp); // closing
}
void clr(){
   
        printf("\033[2J\033[1;1H"); //https://en.wikipedia.org/wiki/ANSI_escape_code


}
void cd(char ** args, char *prompt){
     if(args[1] == 0){
    char curPath[1024];
    getcwd(curPath, sizeof(curPath)); //https://www.ibm.com/docs/en/i/7.3?topic=ssw_ibm_i_73/apis/getcwd.html
    printf("%s\n", curPath);
   }
   else if (chdir(args[1]) == -1)//https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-chdir-change-working-directory
   {
    int retVal = chdir(args[1]);
    if (retVal == -1) {
        printf("Error- '%s' is not a valid directory name\n", args[1]);
        return;
    }
    /* code */
   }
   
   else{
    chdir(args[1]);
    setenv("PWD", args[1],1); //1 for overwriting if already exists https://man7.org/linux/man-pages/man3/setenv.3.html#:~:text=The%20setenv()%20function%20adds,()%20returns%20a%20success%20status).
     strcpy(prompt, getcwd(NULL, 0)); // update prompt with current working directory
                    strcat(prompt, " ==> "); // append shell prompt
                    
   }

}
