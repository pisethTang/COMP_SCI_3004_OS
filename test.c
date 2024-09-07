/********************************************************************************** 
Goal: Automating the testing procedure of our memsim.c file on all traces.
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>


/**************Prototype***********************/
void testAll();
void testTraceOnce(int traceID, int frames, const char* algorithm); 
void testApplicationTrace(int app_traceID);
/*************************************/

// Define the paths to traces
char *relativePathApp = "Traces/Application_Traces";
char *relativePathTraces = "Traces/t";

/* Utility function to compare two files */
int compareFiles(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        printf("Error opening files for comparison.\n");
        return 0;
    }

    char ch1 = fgetc(f1);
    char ch2 = fgetc(f2);

    while (ch1 != EOF && ch2 != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0; // Files are not the same
        }
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
    }

    int result = (ch1 == EOF && ch2 == EOF); // If both reached EOF, they are the same
    fclose(f1);
    fclose(f2);
    return result;
}

/* Function to run the memsim with given arguments and compare output */
void runAndCompare(const char *traceFile, int frames, const char *algorithm, const char *expectedOutput) {
    char command[512];
    char outputFile[] = "output_temp.txt";

    // Construct the command to run memsim with appropriate arguments
    snprintf(command, sizeof(command), "./memsim %s %d %s quiet > %s", traceFile, frames, algorithm, outputFile);

    // Run the command
    system(command);

    // Compare the output file with the expected output
    if (compareFiles(outputFile, expectedOutput)) {
        printf("Test passed for %s with %d frames using %s.\n", traceFile, frames, algorithm);
    } else {
        printf("Test failed for %s with %d frames using %s.\n", traceFile, frames, algorithm);
    }

    // Remove the temporary output file
    remove(outputFile);
}

/* Test all traces in t1, t2, and t3 */
void testAll() {
    printf("Running all trace tests...\n");

    // Run tests for t1
    testTraceOnce(1, 4, "clock");
    testTraceOnce(1, 4, "lru");
    testTraceOnce(1, 8, "clock");
    testTraceOnce(1, 8, "lru");

    // Run tests for t2
    testTraceOnce(2, 6, "clock");
    testTraceOnce(2, 6, "lru");

    // Run tests for t3
    testTraceOnce(3, 4, "clock");
    testTraceOnce(3, 4, "lru");
}

/* Test a single trace based on user input */
void testTraceOnce(int traceID, int frames, const char *algorithm) {
    char traceFile[128], expectedOutput[128];

    // Build paths to trace files and expected outputs based on input traceID
    snprintf(traceFile, sizeof(traceFile), "Traces/t%d/trace%d", traceID, traceID);
    snprintf(expectedOutput, sizeof(expectedOutput), "Traces/t%d/trace%d-%dframes-%s", traceID, traceID, frames, algorithm);

    // Run and compare the results
    runAndCompare(traceFile, frames, algorithm, expectedOutput);
}

/* Test application traces (e.g., bzip, gcc, etc.) */
void testApplicationTrace(int app_traceID) {
    char *traceFile, expectedOutput[128];

    switch (app_traceID) {
        case 0:
            traceFile = "Traces/Application_Traces/sixpack.trace";
            snprintf(expectedOutput, sizeof(expectedOutput), "Traces/Application_Traces/sixpack.trace");
            break;
        case 1:
            traceFile = "Traces/Application_Traces/bzip.trace.gz";
            snprintf(expectedOutput, sizeof(expectedOutput), "Traces/Application_Traces/bzip.trace.gz");
            break;
        case 2:
            traceFile = "Traces/Application_Traces/gcc.trace.gz";
            snprintf(expectedOutput, sizeof(expectedOutput), "Traces/Application_Traces/gcc.trace.gz");
            break;
        case 3:
            traceFile = "Traces/Application_Traces/swim.trace.gz";
            snprintf(expectedOutput, sizeof(expectedOutput), "Traces/Application_Traces/swim.trace.gz");
            break;
        default:
            printf("Invalid application trace ID.\n");
            return;
    }

    // Run and compare the results
    runAndCompare(traceFile, 0, "default", expectedOutput); // Assuming frames and algorithm not applicable to app traces
}




int main(int argc, char* argv[]){
    // main takes in the 3 arguments
    // arg[0]: the usual file or program name.
    // arg[1]: a choice - 0 for trace files, 1 for application traces.
    // arg[2]: see below:   
    /*
    if arg[1] == 1: we are testing trace files - t1, t2, or t3. 
    We have the options to either test all of them at once. Or just test one of them.
        if arg[2] == 0: test all of them at once.
        else if arg[2] == 1: test one of them. 
    else if arg[1] == 0: 
        if arg[2] == 0: test sixpack.trace
        else if arg[2] == 1: bzip.trace
        else if arg[2] == 2: gcc.trace 
        else if arg[2] == 3: swim.trace
    */

if (argc < 3) {
        printf("We need 3 arguments: [choice: 0=app traces, 1=custom traces] [subchoice]\n");
        exit(1);
    } else {
        int arg1 = atoi(argv[1]);
        int arg2 = atoi(argv[2]);

        if (arg1 == 1) {
            switch (arg2) {
                case 0: 
                    testAll(); // Test all custom traces
                    break;
                case 1: 
                    testTraceOnce(1, 4, "clock"); // Example of testing a specific trace (trace1, 4 frames, clock)
                    break;
                default:
                    printf("Invalid trace selection.\n");
                    break;
            }
        } else if (arg1 == 0) {
            // Application traces
            testApplicationTrace(arg2);
        } else {
            printf("Invalid argument. Must either be one or zero.\n");
            exit(1);
        }
    }




    return 0;
}




