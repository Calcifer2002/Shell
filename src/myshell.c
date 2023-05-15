// Malavika Shanker, 20102232 - Operating Systems Project 1 * ca216/myshell
// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.
// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.
// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.
// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.
// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.
// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study.



#include "myshell.h"

#define MAX_BUFFER 1024    // max line buffer
#define MAX_ARGS 64        // max # args
#define SEPARATORS " \t\n" // token separators

int main(int argc, char **argv)
{
    char buf[MAX_BUFFER]; // line buffer
    char *args[MAX_ARGS]; // pointers to arg strings
    char **arg;
    // working pointer thru args
    char *prompt = malloc(sizeof(char) * (1000 + 4)); // allocate memory for prompt
    strcpy(prompt, getcwd(NULL, 0));                  // get current working directory and copy to prompt
    strcat(prompt, " ==> ");// shell prompt
    setEnv();
    
  
    bool batchMode;
    FILE *feed;

    if (argc == 2)
    {
        batchMode = true;
        feed = fopen(argv[1], "r");
        if (!feed)
        {
            fprintf(stderr, "Error opening input file\n");
            exit(1);
        }
    }
    else
    {
        batchMode = false;
        feed = stdin;
    }
  //keep reading input until "quit" command or eof of redirected input
    while (!feof(feed))
    {
        /* get command line from input */
        if (!batchMode)
            fputs(prompt, stdout);

        // write prompt
        // shellEnv();
        if (fgets(buf, MAX_BUFFER, feed))
        { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf, SEPARATORS); // tokenise input

            while ((*arg++ = strtok(NULL, SEPARATORS)))
                ;

            // last entry will be NULL
            if (args[0])
            { // if there's anything there
             // check for internal/external command
                if (!strcmp(args[0], "cd")) // cd command
                {  //i used to have this under a function but when i put it into utils an error popped
                   //for cd and clr so i had to define in main for them but others are within functions.
                   cd(args, prompt);
                }
                else if (!strcmp(args[0], "clr"))   // clr command
                { 

                    clr();
                } // https://en.wikipedia.org/wiki/ANSI_escape_code ;   
                else if (!strcmp(args[0], "dir")) // dir command
                { 
                    dir(args);
                } 
                else if (!strcmp(args[0], "environ")) // environ command
                {
                    environGet();
                }
                else if (!strcmp(args[0], "echo"))//echo command
                {
                    echo(args);
                }
                else if (!strcmp(args[0], "help"))//help command
                { 
                    help();
                }
                else if (!strcmp(args[0], "pause"))//pause command
                { 
                    pauseWork();
                }
                else if (!strcmp(args[0], "quit"))//quit command
                {
                    quit();
                }
                else
                {
                    exec(args, prompt); //if not internal pass to external function

                    wait(NULL); 
                }
            }
        }
    }
    if (batchMode) //closing file
    {
        if (fclose(feed) != 0)
        {
            fprintf(stderr, "Error closing input file\n");
            exit(1);
        }
    }

    return 0;
}