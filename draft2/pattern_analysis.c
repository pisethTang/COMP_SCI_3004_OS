#include "pattern_analysis.h"
#include "list.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>





extern char *search_pattern;
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;




void *pattern_search(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        // Search for the pattern in the list and log occurrences
        // For now, just simulate pattern analysis every 5 seconds
        printf("Searching for pattern: %s\n", search_pattern);
        sleep(5);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void start_analysis_threads() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, pattern_search, NULL);
    pthread_detach(thread_id);
}
