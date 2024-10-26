#include <pthread.h>
// #include "assignment3.c"
// #include "analysis_thread.c"

#define PATTERN "happy"

int main() {
    pthread_t analysis_thread;
    AnalysisArgs args = {PATTERN, &shared_list};

    // Start analysis thread
    pthread_create(&analysis_thread, NULL, analyze_pattern, &args);

    // Start the server
    // main(); // Call the server's main function

    pthread_join(analysis_thread, NULL);
    return 0;
}
