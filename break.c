#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    int pageNo;   // page number
    int modified; // dirty bit 
} page;

enum repl { Random, Fifo, Lru, Clock }; // Replacement algorithms
/*
By default: 
Random: 0
Fifo: 1
Lru: 2
Clock: 3
*/



const int pageOffset = 12;    
int numFrames; // Number of frames, given by the user.
int *frameTable;                 // Table to store page numbers in frames
int *modifiedBits;               // Array to store modified bits for each page
int* referencedBit; 

/******************************************************** */
// Global variables for the page and frame tables.
page* pageTable; // Array of page tables
int* frameTable; // Array of frame tables
/******************************************************** */


/************************************************************************************* */
int createMMU(int frames) {
    numFrames = frames;
    frameTable = (int *)malloc(numFrames * sizeof(int));
    modifiedBits = (int *)malloc(numFrames * sizeof(int));


    for (int i = 0; i < numFrames; i++) {
        frameTable[i] = -1; // -1 indicates an empty frame
        modifiedBits[i] = 0;
    }

    return 0;
}

/************************************************************************************* */


/************************************************************************************* */
// Checks if a page is in memory and returns the frame number or -1 if not found.
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i] == page_number) {
            // pageTable[i].lastUsed = currentTime++; // Update the last used time for LRU
            return i; // Frame number found
        }
    }
    return -1; // Page not found in memory
}
/************************************************************************************* */



/************************************************************************************* */
// Allocates a frame for a page if there's an empty frame available.
int allocateFrame(int page_number) {
	// Checks if there is an empty frame available

	

    return -1; // No empty frame available
}
/************************************************************************************* */





// Selects a victim for eviction/discard according to the replacement algorithm.
page selectVictim(int page_number, enum repl mode) {
    page victim;
    victim.pageNo = 0; // 
    victim.modified = 0; // 

    // Implement replacement algorithm logic here based on `mode`.
	if (mode == Random){
		
	}
	else if (mode == Lru){
		

		return victim;
	}
	else if (mode == Fifo){
		
	}
	else{
		// Clock Replacement algorithm (Cancelled - We don't need to implement this policy.)

	}


    // Default case (if mode is not Lru)
    victim.pageNo = -1;
    victim.modified = 0;
    return victim; // Return the selected victim page
}

int main(int argc, char *argv[]) 
    // Main function code remains the same...
{
	char *traceName; // name of the file 
	int	page_number; 
    // frame_no, done;
	int	do_line;

	// disk_writes and reads are operations that do not convern with the physical memory, only with the disk.
	// int	no_events, disk_writes, disk_reads;
	int debugMode;
 	enum repl replace;
    unsigned address;
    char  rw;
	FILE *trace;

	/*
	The simulator accepts 4 arguments, although the whole program, of course, an extra argument being the program's name.
	arg[0]: memsim.c
	arg[1]: The name of the memory trace file to use.
	arg[2]: The number of page frames in the simulated memory.
	arg[3]: The page replacement algorithm to use: rand/lru/rand
	arg[4]: the mode to run: quiet/debug.
	*/  
    char* programName = argv[0];

    traceName = argv[1];
    trace = fopen(traceName, "r"); // opening the the trace to read 
    numFrames = atoi(argv[2]); // Number of frames
    replace = atoi(argv[3]); // Replacement policy
    
    // Checks which mode we want our program to be in. 
    if (strcmp(argv[4], "quiet\0") == 0) debugMode = 0;
    else if (strcmp(argv[4], "debug\0") == 0) debugMode = 1;
    else {
        printf("Replacement policy must either be quiet or debug mode\n");
        exit(-1);
    }

    do_line = fscanf(trace, "%x %c", &address, &rw);

    // while(do_line == 2){
    page_number = address >> pageOffset;
    // }
    printf("---------------Printing out each given argument:------------\n");\
    printf("%s\n", programName);
    printf("-------------------------------\n");

    printf("-------------------------------\n");
    printf("%d\n", do_line);
    printf("%d\n", page_number);
    printf("Trace Name: %s\n", traceName);
    printf("Debug mode: %d\n", debugMode);
    printf("Number of frames: %d\n", numFrames);
    printf("Replacement Algorithm: %d\n", replace);
}		