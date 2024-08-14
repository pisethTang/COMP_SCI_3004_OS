#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define NV 20   /* max number of command tokens */
#define NL 100  /* input buffer size */

char line[NL]; /* command input buffer */

typedef struct Job {
    int pid;
    int id;
    char command[NL];
    struct Job *next;
} Job;

Job *jobs = NULL;
int job_count = 0;

/*
shell prompt
*/
void prompt(void) {
    fprintf(stdout, "\nmsh> ");
    fflush(stdout);
}

void add_job(int pid, char *command) {
    Job *new_job = (Job *)malloc(sizeof(Job));
    new_job->pid = pid;
    new_job->id = ++job_count;
    strcpy(new_job->command, command);
    new_job->next = jobs;
    jobs = new_job;
    printf("[%d] %d\n", new_job->id, pid);
}

void check_jobs() {
    Job *current = jobs;
    Job *prev = NULL;

    while (current != NULL) {
        int status;
        int result = waitpid(current->pid, &status, WNOHANG);

        if (result == 0) {
            prev = current;
            current = current->next;
        } else if (result == -1) {
            perror("waitpid");
            if (prev == NULL) {
                jobs = current->next;
            } else {
                prev->next = current->next;
            }
            Job *temp = current;
            current = current->next;
            free(temp);
        } else {
            printf("[%d]+ Done %s\n", current->id, current->command);
            if (prev == NULL) {
                jobs = current->next;
            } else {
                prev->next = current->next;
            }
            Job *temp = current;
            current = current->next;
            free(temp);
        }
    }
}

int main(int argk, char *argv[], char *envp[]) {
    int frkRtnVal;   /* value returned by fork sys call */
    char *v[NV];     /* array of pointers to command line tokens */
    char *sep = " \t\n"; /* command line token separators */
    int i;           /* parse index */

    /* prompt for and process one command line at a time */
    while (1) { /* do Forever */
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        if (feof(stdin)) { /* non-zero on EOF */
            fprintf(stderr, "EOF pid %d feof %d ferror %d\n", getpid(),
                    feof(stdin), ferror(stdin));
            exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000')
            continue; /* to prompt */

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++) {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL)
                break;
        }

        int background = 0;
        if (i > 1 && strcmp(v[i - 1], "&") == 0) {
            background = 1;
            v[i - 1] = NULL;
        }

        /* Handle the built-in "cd" command */
        if (strcmp(v[0], "cd") == 0) {
            if (v[1] == NULL || chdir(v[1]) != 0) {
                perror("cd");
            }
            continue;
        }

        /* fork a child process to exec the command in v[0] */
        switch (frkRtnVal = fork()) {
        case -1: /* fork returns error to parent process */
            perror("fork");
            break;

        case 0: /* code executed only by child process */
            execvp(v[0], v);
            perror("execvp"); // If execvp fails
            _exit(EXIT_FAILURE);

        default: /* code executed only by parent process */
            if (background) {
                add_job(frkRtnVal, v[0]);
            } else {
                if (waitpid(frkRtnVal, NULL, 0) == -1) {
                    perror("waitpid");
                }
                printf("%s done\n", v[0]);
            }
            break;
        }

        check_jobs(); // Check if any background jobs have completed
    }
}
