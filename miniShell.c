#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#define NV 20 /* max number of command tokens */
#define NL 100 /* input buffer size */
char line[NL]; /* command input buffer */

int main(int argk, char *argv[], char *envp[]) {
    int frkRtnVal; /* value returned by fork sys call */
    int wpid; /* value returned by wait */
    int status; /* status for wait */
    char *v[NV]; /* array of pointers to command line tokens */
    char *sep = " \t\n"; /* command line token separators */
    int i; /* parse index */
    int bg_processes = 0; /* count of background processes */

    while (1) { /* do Forever */
        // Prompt
        printf("\nmsh> ");
        fflush(stdout);

        // Read command
        if (fgets(line, NL, stdin) == NULL) {
            if (feof(stdin)) {
                exit(0);
            }
            perror("fgets");
            continue;
        }

        // Tokenize input
        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++) {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL) break;
        }

        // Handle empty command
        if (v[0] == NULL) continue;

        // Handle `cd` command
        if (strcmp(v[0], "cd") == 0) {
            if (v[1] == NULL || chdir(v[1]) != 0) {
                perror("cd");
            }
            continue;
        }

        // Check for background process request
        int background = 0;
        if (v[i-1] != NULL && strcmp(v[i-1], "&") == 0) {
            background = 1;
            v[i-1] = NULL; // Remove the "&" from the arguments
        }

        // Fork a child process
        frkRtnVal = fork();
        if (frkRtnVal < 0) {
            perror("fork");
            continue;
        } else if (frkRtnVal == 0) { // Child process
            execvp(v[0], v);
            // If execvp fails
            perror("execvp");
            exit(EXIT_FAILURE);
        } else { // Parent process
            if (background) {
                bg_processes++;
                printf("[%d] %d\n", bg_processes, frkRtnVal);
            } else {
                wpid = waitpid(frkRtnVal, &status, 0);
                if (wpid == -1) {
                    perror("waitpid");
                } else {
                    if (WIFEXITED(status)) {
                        printf("%s done\n", v[0]);
                    }
                }
            }

            // Check if any background processes have finished
            while ((wpid = waitpid(-1, &status, WNOHANG)) > 0) {
                printf("[%d]+ Done %s\n", bg_processes, v[0]);
                bg_processes--;
            }
        }
    }
    return 0;
}
