// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>

// /*********************** Page Table Construction *********************************** */
// typedef struct {
//     int pageNo;   // Page number
//     int modified; // Dirty bit
//     int lastAccessTime; // LRU: Timestamp for least recently used tracking
//     int referenceBit;   // Clock: Reference bit for second chance algorithm
// } page;

// enum repl { Random, Lru, Clock }; // Page replacement algorithms
// /************************************************************** */

// /************************ TO-DO *********************************** */
// int createMMU(int frames);
// int checkInMemory(int page_number);
// int allocateFrame(int page_number);
// page selectVictim(int page_number, enum repl mode);
// /*********************************************************** */

// /********************** Satellite Data ************************************* */
// const int pageoffset = 12; // Page size fixed at 4 KB
// int numFrames;
// page *frameTable;
// int currentTime = 0; // Global time for LRU
// int clockHand = 0;   // Clock hand for the Clock algorithm
// /********************************************************** */

// /******************************************************** */
// // Creates the page table structure to record memory allocation
// /******************************************************** */
// int createMMU(int frames) {
//     numFrames = frames;
//     frameTable = (page *)malloc(numFrames * sizeof(page));

//     for (int i = 0; i < numFrames; i++) {
//         frameTable[i].pageNo = -1; // Indicates an empty frame
//         frameTable[i].modified = 0;
//         frameTable[i].lastAccessTime = 0; // Initialize for LRU
//         frameTable[i].referenceBit = 0;   // Initialize for Clock
//     }

//     return 0;
// }

// /******************************************************** */
// // Checks for residency: returns frame number or -1 if not found
// /******************************************************** */
// int checkInMemory(int page_number) {
//     for (int i = 0; i < numFrames; i++) {
//         if (frameTable[i].pageNo == page_number) {
//             // Update for LRU: Mark as most recently used
//             frameTable[i].lastAccessTime = currentTime++;
//             // For Clock: Mark as referenced
//             frameTable[i].referenceBit = 1;
//             return i; // Page is in memory
//         }
//     }
//     return -1; // Page not found
// }

// /******************************************************** */
// // Allocates a page to the next free frame and records where it was placed
// /******************************************************** */
// int allocateFrame(int page_number) {
//     for (int i = 0; i < numFrames; i++) {
//         if (frameTable[i].pageNo == -1) { // Find an empty frame
//             frameTable[i].pageNo = page_number;
//             frameTable[i].modified = 0;   // Initially not modified
//             frameTable[i].lastAccessTime = currentTime++; // For LRU
//             frameTable[i].referenceBit = 1; // For Clock: Set reference bit
//             return i;
//         }
//     }
//     return -1; // No empty frames available
// }

// /******************************************************** */
// // Selects a victim for eviction/discard based on the replacement algorithm
// /******************************************************** */
// page selectVictim(int page_number, enum repl mode) {
//     page victim;
//     int victimIndex = -1;
// 	// int start = clockHand;

//     if (mode == Random) {
//         // Random replacement algorithm: Select a random frame
//         victimIndex = rand() % numFrames;
//     } 
//     else if (mode == Lru) {
//         // LRU replacement algorithm: Evict the least recently used page
//         victimIndex = 0;
//         for (int i = 0; i < numFrames; i++) {
//             if (frameTable[i].lastAccessTime < frameTable[victimIndex].lastAccessTime) {
//                 victimIndex = i;
//             }
//         }
//     } 
//     else if (mode == Clock) {
//         // Clock replacement algorithm
//         while (1) {
//             // Find a page with a cleared reference bit
//             if (frameTable[clockHand].referenceBit == 0) { // once we've found a a bit that it not set to 1, that means it hasn't been used for some times and will be evicted.
//                 victimIndex = clockHand; // select it for removal
//                 clockHand = (clockHand + 1) % numFrames; // Move the clock hand
//                 break; // stop the algorithm
//             } else {
//                 // Give the page a second chance: clear its reference bit and continue searching.
//                 frameTable[clockHand].referenceBit = 0;
//                 clockHand = (clockHand+ 1) % numFrames; // Move the clock hand
// 			}
//         }
//     }

//     // Get the victim page
//     victim = frameTable[victimIndex];
    
//     // Replace with the new page
//     frameTable[victimIndex].pageNo = page_number;
//     frameTable[victimIndex].modified = 0; // Reset modified bit
//     frameTable[victimIndex].lastAccessTime = currentTime++; // Update for LRU
//     frameTable[victimIndex].referenceBit = 1; // Set reference bit for Clock

//     return victim;
// }

// /******************************************************** */
// // Main function remains unchanged from the skeleton
// /******************************************************** */
// int main(int argc, char *argv[]) {
//     char *tracename;
//     int page_number, frame_no, done;
//     int do_line, i;
//     int no_events, disk_writes, disk_reads;
//     int debugmode;
//     enum repl replace;
//     int allocated = 0;
//     unsigned address;
//     char rw;
//     page Pvictim;
//     FILE *trace;

//     if (argc < 5) {
//         printf("Usage: ./memsim inputfile numberframes replacementmode debugmode\n");
//         exit(-1);
//     } else {
//         tracename = argv[1];
//         trace = fopen(tracename, "r");
//         if (trace == NULL) {
//             printf("Cannot open trace file %s\n", tracename);
//             exit(-1);
//         }

//         numFrames = atoi(argv[2]);
//         if (numFrames < 1) {
//             printf("Frame number must be at least 1\n");
//             exit(-1);
//         }

//         if (strcmp(argv[3], "lru\0") == 0)
//             replace = Lru;
//         else if (strcmp(argv[3], "rand\0") == 0)
//             replace = Random;
//         else if (strcmp(argv[3], "clock\0") == 0)
//             replace = Clock;
//         else {
//             printf("Replacement algorithm must be rand/fifo/lru/clock\n");
//             exit(-1);
//         }

//         if (strcmp(argv[4], "quiet\0") == 0)
//             debugmode = 0;
//         else if (strcmp(argv[4], "debug\0") == 0)
//             debugmode = 1;
//         else {
//             printf("Replacement algorithm must be quiet/debug\n");
//             exit(-1);
//         }
//     }

//     done = createMMU(numFrames);
//     if (done == -1) {
//         printf("Cannot create MMU\n");
//         exit(-1);
//     }

//     no_events = 0;
//     disk_writes = 0;
//     disk_reads = 0;

//     do_line = fscanf(trace, "%x %c", &address, &rw);
//     while (do_line == 2) {
//         page_number = address >> pageoffset;
//         frame_no = checkInMemory(page_number);

//         if (frame_no == -1) {
//             disk_reads++; // Page fault, need to load into memory
//             if (debugmode)
//                 printf("Page fault %8d\n", page_number);
//             if (allocated < numFrames) {
//                 frame_no = allocateFrame(page_number);
//                 allocated++;
//             } else {
//                 Pvictim = selectVictim(page_number, replace); // Select a victim
//                 if (Pvictim.modified) {
//                     disk_writes++;
//                     if (debugmode)
//                         printf("Disk write %8d\n", Pvictim.pageNo);
//                 } else {
//                     if (debugmode)
//                         printf("Discard %8d\n", Pvictim.pageNo);
//                 }
//             }
//         }

//         if (rw == 'R') {
//             if (debugmode)
//                 printf("Reading %8d\n", page_number);
//         } else if (rw == 'W') {
//             frameTable[frame_no].modified = 1; // Mark the page as modified
//             if (debugmode)
//                 printf("Writing %8d\n", page_number);
//         } else {
//             printf("Badly formatted file. Error on line %d\n", no_events + 1);
//             exit(-1);
//         }

//         no_events++;
//         do_line = fscanf(trace, "%x %c", &address, &rw);
//     }

//     printf("total memory frames:  %d\n", numFrames);
//     printf("events in trace:      %d\n", no_events);
//     printf("total disk reads:     %d\n", disk_reads);
//     printf("total disk writes:    %d\n", disk_writes);
//     printf("page fault rate:      %.4f\n", (float)disk_reads / no_events);
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int pageNo;
    int modified;
} page;

enum repl { Random, Lru, Clock };

int createMMU(int frames);
int checkInMemory(int page_number);
int allocateFrame(int page_number);
page selectVictim(int page_number, enum repl mode);

const int pageoffset = 12; /* Page size is fixed to 4 KB */
int numFrames;
int *frameTable; /* Array to store page numbers */
int *lruCounter; /* For LRU */
int clockHand = 0; /* For Clock */
int *secondChance; /* For Clock */
int *modifiedBit; /* Track modified state */

/* Creates the page table structure to record memory allocation */
int createMMU(int frames) {
    numFrames = frames;
    frameTable = (int *)malloc(numFrames * sizeof(int));
    lruCounter = (int *)malloc(numFrames * sizeof(int));
    secondChance = (int *)malloc(numFrames * sizeof(int));
    modifiedBit = (int *)malloc(numFrames * sizeof(int));

    if (!frameTable || !lruCounter || !secondChance || !modifiedBit) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    for (int i = 0; i < numFrames; i++) {
        frameTable[i] = -1; // Initialize with invalid page numbers
        lruCounter[i] = 0;  // Initialize LRU counter to zero
        secondChance[i] = 0; // Initialize second chance to false
        modifiedBit[i] = 0;  // Initialize modified bit to false
    }

    return 0;
}

/* Checks for residency: returns frame no or -1 if not found */
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i] == page_number) {
            lruCounter[i] = 0; // Reset LRU counter for the page
            return i;          // Page found, return frame number
        }
    }
    return -1; // Page not found
}

/* allocate page to the next free frame and record where it put it */
int allocateFrame(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i] == -1) {
            frameTable[i] = page_number; // Allocate the page
            lruCounter[i] = 0;           // Initialize LRU counter
            secondChance[i] = 1;         // Initialize second chance for Clock
            modifiedBit[i] = 0;          // Initialize modified bit to false
            return i;                    // Return allocated frame number
        }
    }
    return -1; // No free frame
}

/* LRU Replacement Algorithm */
int findLRU() {
    int max = 0, victim = 0;
    for (int i = 0; i < numFrames; i++) {
        if (lruCounter[i] > max) {
            max = lruCounter[i];
            victim = i;
        }
    }
    return victim;
}

/* Clock Replacement Algorithm */
int findClockVictim() {
    while (1) {
        if (secondChance[clockHand] == 0) {
            int victim = clockHand;
            clockHand = (clockHand + 1) % numFrames;
            return victim;
        } else {
            secondChance[clockHand] = 0;
            clockHand = (clockHand + 1) % numFrames;
        }
    }
}

/* Random Replacement Algorithm */
int findRandomVictim() {
    return rand() % numFrames;
}

/* Selects a victim for eviction according to the replacement algorithm, returns chosen frame_no */
page selectVictim(int page_number, enum repl mode) {
    page victim;
    int victimFrame = 0;

    switch (mode) {
        case Lru:
            victimFrame = findLRU();
            break;
        case Clock:
            victimFrame = findClockVictim();
            break;
        case Random:
            victimFrame = findRandomVictim();
            break;
        default:
            printf("Invalid replacement mode.\n");
            exit(-1);
    }

    victim.pageNo = frameTable[victimFrame];
    victim.modified = modifiedBit[victimFrame]; // Record if the victim was modified
    frameTable[victimFrame] = page_number; // Replace victim with new page
    modifiedBit[victimFrame] = 0; // New page starts as not modified
    lruCounter[victimFrame] = 0; // Reset LRU counter
    secondChance[victimFrame] = 1; // Set second chance for Clock
    return victim;
}

int main(int argc, char *argv[]) {
    char *tracename;
    int page_number, frame_no, done;
    int do_line, i;
    int no_events, disk_writes, disk_reads;
    int debugmode;
    enum repl replace;
    int allocated = 0;
    unsigned address;
    char rw;
    page Pvictim;
    FILE *trace;

    if (argc < 5) {
        printf("Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
        exit(-1);
    } else {
        tracename = argv[1];
        trace = fopen(tracename, "r");
        if (trace == NULL) {
            printf("Cannot open trace file %s \n", tracename);
            exit(-1);
        }

        numFrames = atoi(argv[2]);
        if (numFrames < 1) {
            printf("Frame number must be at least 1\n");
            exit(-1);
        }

        if (strcmp(argv[3], "lru") == 0)
            replace = Lru;
        else if (strcmp(argv[3], "random") == 0)
            replace = Random;
        else if (strcmp(argv[3], "clock") == 0)
            replace = Clock;
        else {
            printf("Replacement algorithm must be Random/Lru/Clock\n");
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
        frame_no = checkInMemory(page_number); /* ask for physical address */

        if (frame_no == -1) {
            disk_reads++; /* Page fault, need to load it into memory */
            if (debugmode)
                printf("Page fault %8d \n", page_number);
            if (allocated < numFrames) { /* allocate it to an empty frame */
                frame_no = allocateFrame(page_number);
                allocated++;
            } else {
                Pvictim = selectVictim(page_number, replace); /* returns page number of the victim */
                frame_no = checkInMemory(page_number); /* find out the frame the new page is in */
                if (Pvictim.modified) {
                    disk_writes++;
                    if (debugmode)
                        printf("Disk write %8d \n", Pvictim.pageNo);
                } else if (debugmode)
                    printf("Discard    %8d \n", Pvictim.pageNo);
            }
        }

        if (rw == 'R') {
            if (debugmode)
                printf("Reading    %8d \n", page_number);
        } else if (rw == 'W') {
            modifiedBit[frame_no] = 1; // Mark page as modified
            if (debugmode)
                printf("Writing    %8d \n", page_number);
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
    free(lruCounter);
    free(secondChance);
    free(modifiedBit);

    return 0;
}
