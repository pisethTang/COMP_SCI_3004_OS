/**********************************************************************
    Program: miniShell              Version: 1.3
    ---------------------------------------------------------------
    Skeleton code for Linux/Unix/Minix Command Line Interpreter
    ---------------------------------------------------------------
    File: miniShell.c 
    Compiler/System: gcc/Linux

  **********************************************************/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define NV 20  /*Max number of command tokens*/
#define NL 100 /*Input buffer size*/
char line[NL]; /*Command input buffer*/

void prompt(void){
    fprintf(stdout, "\nmsh> ");
    fflush(stdout);
}


int main(int argk, char* argv[], char* envp[]){
    /*argk - number of arguments */
    /*argv - argument vector for command line*/
    /*envp - environment pointer */
    int frkRtnVal;      /*Value returned by fork sys call*/
    int wpid;           /*Value returned by wait*/
    char *v[NV];        /*Array of pointers to command line tokens*/
    char *sep = "\t\n"; /*Command line token separators*/ 
    int i;              /*Parse index*/
    int bg;             /*Background flag*/



    /*Prompt for and process one command line at a time*/
    while(1){ /*Loop forever*/
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        if (feof(stdin)){ /*Non-zero on EOF*/
            fprintf(stderr, "Error pid %d feof %d ferror %d\n", getpid(), feof(stdin), ferror(stdin));
            exit(0);
        }

        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000') continue; /*prompting*/

        v[0] = strtok(line, sep);

        for (i = 1; i < NV; i++){
            v[i] = strtok(NULL, sep);

            if (v[i] == NULL) break;
        }

        /* Check for background processes */
        if (strcmp(v[i-1], "&") == 0){
            bg = 1;
            v[i-1] = NULL; /* Remove '&' from arguments*/
        }
        else{
            bg = 0;
        }

        /* Handle built-in commands like cd*/
        if (strcmp(v[0], "cd") == 0){
            if (chdir(v[1]) != 0){
                perror("cd failed");
            }
            continue; /* Skip the rest of the loop*/
        }

        /*Assert is number of tokens + 1*/

        /*Fork a child process to exec the command in v[0]*/

        switch(frkRtnVal = fork()){
            case -1: 
                perror("fork failed");
                break; /*Fork returns error to parent process*/
            case 0: 
                execvp(v[0], v); /*code executed only by the child process*/
                perror("exec failed");
                exit(1);

            default: /* code executed only by the parent process*/
                if (!bg) {
                    wpid = wait(0);
                    printf("%s done\n", v[0]);
                } else {
                    printf("[%d] %s running in background\n", frkRtnVal, v[0]);
                }
                break;
            // {       
            //     wpid = wait(0);
            //     printf("%s done \n", v[0]);
            //     break;
            // }
        } /*END switch*/
    } /*END while*/

    return 0;
}
