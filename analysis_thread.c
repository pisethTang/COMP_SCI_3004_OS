#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "assignment3.c" // include shared structures

#define ANALYSIS_INTERVAL 5

typedef struct {
    char *pattern;
    SharedList *shared_list;
} AnalysisArgs;

void *analyze_pattern(void *args) {
    AnalysisArgs *analysis_args = (AnalysisArgs *)args;
    char *pattern = analysis_args->pattern;
    SharedList *list = analysis_args->shared_list;

    while (1) {
        sleep(ANALYSIS_INTERVAL);
        Node *current = list->head;
        int count = 0;

        while (current) {
            if (strstr(current->line, pattern) != NULL) {
                count++;
            }
            current = current->next;
        }

        printf("Pattern \"%s\" found %d times in the received data.\n", pattern, count);
    }
    return NULL;
}
