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

enum repl {Random, Lru, Clock}; // Page replacement algorithms
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
page *pageTable;
int currentTime = 0; // Global time for LRU
int clockHand = 0;   // Clock hand for the Clock algorithm
/********************************************************** */

/******************************************************** */
// Creates the page table structure to record memory allocation
/******************************************************** */
int createMMU(int frames) {
    numFrames = frames;
    pageTable = (page *)malloc(numFrames * sizeof(page));

    for (int i = 0; i < numFrames; i++) {
        pageTable[i].pageNo = -1; // Indicates an empty frame
        pageTable[i].modified = 0;
        pageTable[i].lastAccessTime = 0; // Initialize for LRU
        pageTable[i].referenceBit = 0;   // Initialize for Clock
    }

    return 0;
}

/******************************************************** */
// Checks for residency: returns frame number or -1 if not found
/******************************************************** */
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (pageTable[i].pageNo == page_number) {
            // Update for LRU: Mark as most recently used
            pageTable[i].lastAccessTime = currentTime++;
            // For Clock: Mark as referenced
            pageTable[i].referenceBit = 1;
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
        if (pageTable[i].pageNo == -1) { // Find an empty frame
            pageTable[i].pageNo = page_number;
            pageTable[i].modified = 0;   // Initially not modified
            pageTable[i].lastAccessTime = currentTime++; // For LRU
            pageTable[i].referenceBit = 1; // For Clock: Set reference bit
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
            if (pageTable[i].lastAccessTime < pageTable[victimIndex].lastAccessTime) {
                victimIndex = i;
            }
        }
    } 
    else if (mode == Clock) {
        // Clock replacement algorithm
        while (1) {
            // Find a page with a cleared reference bit
            if (pageTable[clockHand].referenceBit == 0) { // once we've found a bit that is not set to 1, that means it hasn't been used for some time and will be evicted.
                victimIndex = clockHand;
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
                break;
            } else {
                // Give the page a second chance: clear its reference bit and continue searching.
                pageTable[clockHand].referenceBit = 0;
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
            }
        }
    }

    // Get the victim page
    victim = pageTable[victimIndex];
    
    // Replace with the new page
    pageTable[victimIndex].pageNo = -1; // Clear out the victim's page number
    pageTable[victimIndex].modified = 0; // Reset modified bit
    pageTable[victimIndex].lastAccessTime = currentTime++; // Update for LRU
    pageTable[victimIndex].referenceBit = 1; // Set reference bit for Clock (we just loaded it)

    return victim;
}


struct result{
    int numberAccesses;
    int numFrames_;
    float pageFaultRate;
};

void printResult(struct result* Result){
    printf("numberAccesses: %d\n", Result->numberAccesses);
    printf("Number of frames: %d\n", Result->numFrames_);
    printf("Page fault rate: %f\n", Result->pageFaultRate);
}

void writeToFile(struct result* Result){
    /********************************************************************************************** */
    int no_events = Result -> numberAccesses;
    int numFrames = Result -> numFrames_;
    float pageFaultRate = Result -> pageFaultRate;
    
    
    FILE* output; // write to a file called output.txt
    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Error opening output file\n");
        exit(-1);
    }
    // float pageFaultRate = (float)disk_reads / no_events;
    fprintf(output, "total memory frames:  %d\n", numFrames);
    fprintf(output, "events in trace:      %d\n", no_events);
    // fprintf(output, "total disk reads:     %d\n", disk_reads);
    // fprintf(output, "total disk writes:    %d\n", disk_writes);
    fprintf(output, "page fault rate:      %.4f\n", pageFaultRate);

    fclose(output); // Close the output file
}

void Simulation(int argc, char *argv[]){
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
            if (debugmode) printf("Page fault    %8d\n", page_number);
            // if the memory is full, allocate a new page in th ephysical memory
            if (allocated < numFrames) {
                frame_no = allocateFrame(page_number);
                allocated++;
            } 
            else {
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
            pageTable[frame_no].modified = 1; // Mark the page as modified
            if (debugmode)
                printf("Writing       %8d\n", page_number);
        } else {
            printf("Badly formatted file. Error on line %d\n", no_events + 1);
            exit(-1);
        }

        no_events++;
        do_line = fscanf(trace, "%x %c", &address, &rw);
    }

    struct result* R;
    
    R -> numberAccesses = no_events;
    R -> numFrames_ = numFrames;
    R -> pageFaultRate = (float) disk_reads/disk_writes;


/*************************************************************************************** */
    printf("total memory frames:  %d\n", numFrames);
    printf("events in trace:      %d\n", no_events);
    printf("total disk reads:     %d\n", disk_reads);
     printf("page fault rate:      %.4f\n", (float)disk_reads / no_events);
/*************************************************************************** */
    writeToFile(R);
    free(pageTable);
    fclose(trace);

    // return R;
}





void printFrames(){
    printf("Print frames: %d\n", numFrames);
    numFrames = 1;
}




int main(int argc, char *argv[]) {
    // Simulating the VM process. 
    char* workloadSizes[] = {"1", "10", "100", "1000", 
                            "10000", "100000",  "1000000"};
    int N = sizeof(workloadSizes)/sizeof(workloadSizes[0]);
    
    // struct result* Results;
    // Results = Simulation(argc, argv);
    // writeToFile(Results);
    // printf("Printing out all arguments.\n");
    // for (int i=0; i<N; i++){
    //     for (int j=0; j<argc; j++){
    //         if (j == 1){
    //            argv[j] = workloadSizes[i];
    //         }
    //         printf("argv[%d] = %s\n", i, &(*argv[j]));
    //     }
    //     printf("\n------------------------------------------\n");
    // }
    struct result* result1;
    struct result* result2;
    argv[2] = "1";
    Simulation(argc, argv);


    argv[2] = "10";
    Simulation(argc, argv);

    printResult(result1);
    printf("-----------------------------------------------------\n");
    printResult(result2);



    // for (int i=0; i<N; i++){
    //     argv[]
    //     Results = Simulation(argc, argv);
    // }


    // printFrames();
    // numFrames = 10000;
    // printFrames();




    return 0;
}

