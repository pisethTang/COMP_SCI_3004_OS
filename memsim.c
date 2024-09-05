#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*********************** Page Table Construction *********************************** */
typedef struct {
    int pageNo;         // Page number
    int modified;       // Dirty bit
    int lastAccessTime; // LRU: Timestamp for least recently used tracking
    int referenceBit;   // Clock: Reference bit for second chance algorithm
} page;

enum repl { Random, Lru, Clock }; // Page replacement algorithms
/************************************************************** */

/************************ TO-DO *********************************** */
int createMMU(int frames);
int checkInMemory(int page_number);
int allocateFrame(int page_number);
page selectVictim(enum repl mode);
/*********************************************************** */

/********************** Satellite Data ************************************* */
const int pageoffset = 12; // Page size fixed at 4 KB
int numFrames;
page *frameTable;
int currentTime = 0; // Global time for LRU
int clockHand = 0;   // Clock hand for the Clock algorithm
/********************************************************** */

/******************************************************** */
// Creates the page table structure to record memory allocation
/******************************************************** */
int createMMU(int frames) {
    numFrames = frames;
    frameTable = (page *)malloc(numFrames * sizeof(page));

    for (int i = 0; i < numFrames; i++) {
        frameTable[i].pageNo = -1; // Indicates an empty frame
        frameTable[i].modified = 0;
        frameTable[i].lastAccessTime = 0; // Initialize for LRU
        frameTable[i].referenceBit = 0;   // Initialize for Clock
    }

    return 0;
}

/******************************************************** */
// Checks for residency: returns frame number or -1 if not found
/******************************************************** */
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i].pageNo == page_number) {
            // Update for LRU: Mark as most recently used
            frameTable[i].lastAccessTime = currentTime++;
            // For Clock: Mark as referenced
            frameTable[i].referenceBit = 1;
            return i; // Page is in memory
        }
    }
    return -1; // Page not found
}

/******************************************************** */
// Allocates a page to the next free frame and records where it was placed
/******************************************************** */
int allocateFrame(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i].pageNo == -1) { // Find an empty frame
            frameTable[i].pageNo = page_number;
            frameTable[i].modified = 0;   // Initially not modified
            frameTable[i].lastAccessTime = currentTime++; // For LRU
            frameTable[i].referenceBit = 1; // For Clock: Set reference bit
            return i;
        }
    }
    return -1; // No empty frames available
}

/******************************************************** */
// Selects a victim for eviction/discard based on the replacement algorithm
/******************************************************** */
page selectVictim(enum repl mode) {
    page victim;
    int victimIndex = -1;

    if (mode == Random) {
        // Random replacement algorithm: Select a random frame
        victimIndex = rand() % numFrames;
    } 
    else if (mode == Lru) {
        // LRU replacement algorithm: Evict the least recently used page
        victimIndex = 0;
        for (int i = 0; i < numFrames; i++) {
            if (frameTable[i].lastAccessTime < frameTable[victimIndex].lastAccessTime) {
                victimIndex = i;
            }
        }
    } 
    else if (mode == Clock) {
        // Clock replacement algorithm
        while (1) {
            // Find a page with a cleared reference bit
            if (frameTable[clockHand].referenceBit == 0) { // once we've found a bit that is not set to 1, that means it hasn't been used for some time and will be evicted.
                victimIndex = clockHand;
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
                break;
            } else {
                // Give the page a second chance: clear its reference bit and continue searching.
                frameTable[clockHand].referenceBit = 0;
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
            }
        }
    }

    // Get the victim page
    victim = frameTable[victimIndex];
    
    // Replace with the new page
    frameTable[victimIndex].pageNo = -1; // Clear out the victim's page number
    frameTable[victimIndex].modified = 0; // Reset modified bit
    frameTable[victimIndex].lastAccessTime = currentTime++; // Update for LRU
    frameTable[victimIndex].referenceBit = 1; // Set reference bit for Clock (we just loaded it)

    return victim;
}


int main(int argc, char *argv[]) {
    char *tracename;
    int page_number, frame_no, done;
    int do_line;
    int no_events, disk_writes, disk_reads;
    int debugmode;
    enum repl replace;
    int allocated = 0;
    unsigned address;
    char rw;
    page Pvictim;
    FILE *trace;

    if (argc < 5) {
        printf("Usage: ./memsim inputfile numberframes replacementmode debugmode\n");
        exit(-1);
    } else {
        tracename = argv[1];
        trace = fopen(tracename, "r");
        if (trace == NULL) {
            printf("Cannot open trace file %s\n", tracename);
            exit(-1);
        }

        numFrames = atoi(argv[2]);
        if (numFrames < 1) {
            printf("Frame number must be at least 1\n");
            exit(-1);
        }

        if (strcmp(argv[3], "lru") == 0)
            replace = Lru;
        else if (strcmp(argv[3], "rand") == 0)
            replace = Random;
        else if (strcmp(argv[3], "clock") == 0)
            replace = Clock;
        else {
            printf("Replacement algorithm must be rand/lru/clock\n");
            exit(-1);
        }

        if (strcmp(argv[4], "quiet") == 0)
            debugmode = 0;
        else if (strcmp(argv[4], "debug") == 0)
            debugmode = 1;
        else {
            printf("Debug mode must be quiet/debug\n");
            exit(-1);
        }
    }

    done = createMMU(numFrames);
    if (done == -1) {
        printf("Cannot create MMU\n");
        exit(-1);
    }

    no_events = 0;
    disk_writes = 0;
    disk_reads = 0;

    do_line = fscanf(trace, "%x %c", &address, &rw);
    while (do_line == 2) {
        page_number = address >> pageoffset;
        frame_no = checkInMemory(page_number);

        if (frame_no == -1) {
            disk_reads++; // Page fault, need to load into memory
            if (debugmode)
                printf("Page fault    %8d\n", page_number);
            if (allocated < numFrames) {
                frame_no = allocateFrame(page_number);
                allocated++;
            } else {
                Pvictim = selectVictim(replace); // Select a victim
                if (Pvictim.modified) {
                    disk_writes++;
                    if (debugmode)
                        printf("Disk write    %8d\n", Pvictim.pageNo);
                } else {
                    if (debugmode)
                        printf("Discard       %8d\n", Pvictim.pageNo);
                }
                // Replace the page
                frame_no = allocateFrame(page_number);
            }
        }

        if (rw == 'R') {
            if (debugmode)
                printf("Reading       %8d\n", page_number);
        } else if (rw == 'W') {
            frameTable[frame_no].modified = 1; // Mark the page as modified
            if (debugmode)
                printf("Writing       %8d\n", page_number);
        } else {
            printf("Badly formatted file. Error on line %d\n", no_events + 1);
            exit(-1);
        }

        no_events++;
        do_line = fscanf(trace, "%x %c", &address, &rw);
    }

    printf("total memory frames:  %d\n", numFrames);
    printf("events in trace:      %d\n", no_events);
    printf("total disk reads:     %d\n", disk_reads);
    printf("total disk writes:    %d\n", disk_writes);
    printf("page fault rate:      %.4f\n", (float)disk_reads / no_events);

    fclose(trace);
    free(frameTable);

    return 0;
}

