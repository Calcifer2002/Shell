// Malavika Shanker, 20102232 - Operating Systems Project 1 * ca216/myshell
// I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.
// I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.
// I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references. Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other source whatsoever are acknowledged and the sources cited are identified in the assignment references.
// I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of others save and to the extent that such work has been cited and acknowledged within the text of my work.
// I have used the DCU library referencing guidelines (available at https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate referencing system recommended in the assignment guidelines and/or programme documentation.
// By signing this form or by submitting material online I confirm that this assignment, or any part of it, has not been previously submitted by me or any other person for assessment on this or any other course of study.



#define MAX_BUFFER 1024    // max line buffer
#define MAX_ARGS 64        // max # args
#define SEPARATORS " \t\n"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>
 // token separators

void setEnv();

void dir(char **args);
void environGet();
void cd(char **args, char *prompt);
void clr();
void echo(char **args);
void help();
void pauseWork();
void quit();
void exec(char **args,  char *prompt);
void shellEnv();