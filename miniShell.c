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
    fprintf(stdout, "\n msh> ");
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

        /*Assert is number of tokens + 1*/
        /*Fork a child process to exec the command in v[0]*/

        switch(frkRtnVal == fork()){
            case -1: break; /*Fork returns error to parent process*/
            case 0: execvp(v[0], v); /*code executed only by the child process*/
            default: /* code executed only by the parent process*/
            {       
                wpid = wait(0);
                printf("%s done \n", v[0]);
                break;
            }
        } /*END switch*/
    } /*END while*/

    return 0;
}

/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <signal.h>
// #define NV 20 /* max number of command tokens */
// #define NL 100 /* input buffer size */
// char line[NL]; /* command input buffer */
// /*
// shell prompt
// */
// prompt(void)
// {
// fprintf(stdout, "\n msh> ");
// fflush(stdout);
// }
// main(int argk, char *argv[], char *envp[])
// /* argk - number of arguments */
// /* argv - argument vector from command line */
// /* envp - environment pointer */
// {
// int frkRtnVal; /* value returned by fork sys call */
// int wpid; /* value returned by wait */
// char *v[NV]; /* array of pointers to command line tokens */
// char *sep = " \t\n";/* command line token separators */
// int i; /* parse index */
// /* prompt for and process one command line at a time */
// while (1) { /* do Forever */
// prompt();
// fgets(line, NL, stdin);
// fflush(stdin);
// if (feof(stdin)) { /* non-zero on EOF */
// fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
// feof(stdin), ferror(stdin));
// exit(0);
// }
// if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
// continue; /* to prompt */
// v[0] = strtok(line, sep);
// for (i = 1; i < NV; i++) {
// v[i] = strtok(NULL, sep);
// if (v[i] == NULL)
// break;
// }
// /* assert i is number of tokens + 1 */
// /* fork a child process to exec the command in v[0] */
// switch (frkRtnVal = fork()) {
// case -1: /* fork returns error to parent process */
// {
// break;
// }
// case 0: /* code executed only by child process */
// {
// execvp(v[0], v);
// }
// default: /* code executed only by parent process */
// {
// wpid = wait(0);
// printf("%s done \n", v[0]);
// break;
// }
// } /* switch */
// } /* while */
// } /* main */







// */