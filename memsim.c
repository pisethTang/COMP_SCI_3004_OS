// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// // #include <>

// typedef struct {
//     int pageNo;   // page number
//     int modified; // dirty bit 
// 	int lastAccessTime; // Timestamp for LRU tracking
// } page;

// enum repl { Random, Fifo, Lru, Clock }; // Replacement algorithms

// const int pageoffset = 12;    
// int numFrames; // Number of frames, given by the user.
// // int *frameTable;                 // Table to store page numbers in frames
// // int *modifiedBits;               // Array to store modified bits for each page
// int* referencedBit; 
// int currentTime = 0;         // Global time counter for LRU


// /******************************************************** */
// // Global variables for the page and frame tables.
// // page* pageTable; // Array of page tables
// page* frameTable; // Array of frame tables
// /******************************************************** */


// /************************************************************************************* */
// int createMMU(int frames) {
// 	// MMU uses the Page Map to translate VPN to PPN.
// 	// Therefore, we'll have t somehow create a hash table. 
//     numFrames = frames;
//     frameTable = (page *)malloc(numFrames * sizeof(page));
//     referencedBit = (int *)malloc(numFrames * sizeof(int));
	
// 	// Creating an array of page tables
	

//     for (int i = 0; i < numFrames; i++) {
//         frameTable[i].pageNo = -1; // 0 indicates an empty frame
// 		frameTable[i].modified = 0;
// 		frameTable[i].lastAccessTime = -1; // Not yet accessed
// 		referencedBit[i] = 0; ///
//     }

//     return 0;
// }

// /************************************************************************************* */


// /************************************************************************************* */
// // Checks if a page is in memory and returns the frame number or -1 if not found.
// int checkInMemory(int page_number) {
//     for (int i = 0; i < numFrames; i++) {
//         if (frameTable[i].pageNo == page_number) {
//             // pageTable[i].lastUsed = currentTime++; // Update the last used time for LRU
// 			frameTable[i].lastAccessTime = currentTime++;
//             return i; // Frame number found
//         }
//     }
//     return -1; // Page not found in memory (will trigger a page fault)
// }
// /************************************************************************************* */




// /************************************************************************************* */
// // Allocates a frame for a page if there's an empty frame available.
// int allocateFrame(int page_number) {
// 	// Checks if there is an empty frame available
// 	for (int i=0; i<numFrames; i++){
// 		if (frameTable[i].pageNo == -1) 
// 		{	// Found an empty frame.
// 			frameTable[i].pageNo = page_number;
// 			frameTable[i].modified = 0;
// 			frameTable[i].lastAccessTime = currentTime++; // Set access time for LRU
// 			return i;
// 		}	
// 	}
//     return -1; // No empty frame available
// }
// /************************************************************************************* */





// // Selects a victim for eviction/discard according to the replacement algorithm.
// page selectVictim(int page_number, enum repl mode) {
//     page victim;
//     victim.pageNo = 0; 	 // 
//     victim.modified = 0; // 0:= not modified.
// 	int victimIndex = 0;

//     // Implement replacement algorithm logic here based on `mode`.
// 	if (mode == Random){
// 		// Just pick a random page in the PM to evict
// 		victimIndex = rand() % numFrames; // Pick a random number from 0 to numFrames-1
// 		victim = frameTable[victimIndex];
		
// 		// victim.pageNo = frameTable[randomIndex];
// 		frameTable[victimIndex].pageNo = page_number;
// 	}
// 	else if (mode == Lru){
// 		// Assumes that the least recently used page is the one with the lowest index.
// 		victimIndex = 0; // Starting with the first page as the LRU and then progress.
// 		victim = frameTable[victimIndex];

// 		for (int i=1; i<numFrames; i++){
// 			frameTable[i-1] = frameTable[i]; // Shift pages to the left 
// 		}
// 		frameTable[numFrames-1].pageNo = page_number;
// 		// return victim;
// 	}
// 	// else if (mode == Fifo){
// 	// 	// (Cancelled - We don't need to implement this policy.)
// 	// }
// 	else{
// 		// Clock Replacement algorithm 
//         static int hand = 0; // Clock hand pointing to the next frame to check
//         while (referencedBit[hand] != 0) {
//             referencedBit[hand] = 0; // Give second chance, set reference bit to 0
//             hand = (hand + 1) % numFrames; // Move the clock hand
//         }
//         victim = frameTable[hand];
//         frameTable[hand].pageNo = page_number;
//         referencedBit[hand] = 1; // Set reference bit for the new page
//         hand = (hand + 1) % numFrames; // Move the clock hand

// 	}

//     // Default case (if mode is not Lru)
//     // victim.pageNo = 0;
//     // victim.modified = 0;
//     return victim; // Return the selected victim page
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int pageNo;   // Page number
    int modified; // Dirty bit
    int lastAccessTime; // For LRU tracking
    int arrivalTime; // For FIFO tracking
} page;

enum repl { Random, Fifo, Lru, Clock }; // Page replacement algorithms

int createMMU(int frames);
int checkInMemory(int page_number);
int allocateFrame(int page_number);
page selectVictim(int page_number, enum repl mode);
const int pageoffset = 12; // Page size is fixed to 4 KB
int numFrames;
page *frameTable; // Array of pages in frames
int currentTime = 0; // Global time for LRU and FIFO
int clockHand = 0; // Hand for the Clock algorithm
int *referencedBit; // Reference bits for Clock algorithm

/******************************************************** */
// Creates the page table structure to record memory allocation
/******************************************************** */
int createMMU(int frames) {
    numFrames = frames;
    frameTable = (page *)malloc(numFrames * sizeof(page));
    referencedBit = (int *)malloc(numFrames * sizeof(int));

    for (int i = 0; i < numFrames; i++) {
        frameTable[i].pageNo = -1; // -1 indicates an empty frame
        frameTable[i].modified = 0;
        frameTable[i].lastAccessTime = -1; // Last access time for LRU
        frameTable[i].arrivalTime = -1; // Arrival time for FIFO
        referencedBit[i] = 0; // Initialize reference bits for the Clock algorithm
    }

    return 0;
}

/******************************************************** */
// Checks for residency: returns frame no or -1 if not found
/******************************************************** */
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i].pageNo == page_number) {
            // Update last access time for LRU
            frameTable[i].lastAccessTime = currentTime++;
            return i; // Frame number found
        }
    }
    return -1; // Page not found in memory
}

/******************************************************** */
// Allocate page to the next free frame and record where it put it
/******************************************************** */
int allocateFrame(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i].pageNo == -1) { // Found an empty frame
            frameTable[i].pageNo = page_number;
            frameTable[i].modified = 0; // Set modified bit to 0 initially
            frameTable[i].lastAccessTime = currentTime++;
            frameTable[i].arrivalTime = currentTime++; // Set arrival time for FIFO
            return i; // Return the allocated frame number
        }
    }
    return -1; // No empty frame available
}

/******************************************************** */
// Selects a victim for eviction/discard according to the replacement algorithm
// Returns the chosen victim page
/******************************************************** */
page selectVictim(int page_number, enum repl mode) {
    page victim;
    victim.pageNo = -1; // Initialize victim page
    victim.modified = 0;

    if (mode == Random) {
        // Random Replacement Algorithm
        int victimIndex = rand() % numFrames; // Select a random frame
        victim = frameTable[victimIndex]; // Get the victim page
        frameTable[victimIndex].pageNo = page_number; // Replace with the new page
    } 
    else if (mode == Lru) {
        // LRU Replacement Algorithm
        int victimIndex = 0; // Start with the first frame as the least recently used
        for (int i = 1; i < numFrames; i++) {
            if (frameTable[i].lastAccessTime < frameTable[victimIndex].lastAccessTime) {
                victimIndex = i; // Update to the least recently used page
            }
        }
        victim = frameTable[victimIndex]; // Select the victim
        frameTable[victimIndex].pageNo = page_number; // Replace with the new page
        frameTable[victimIndex].lastAccessTime = currentTime++; // Update access time
    } 
    else if (mode == Clock) {
        // Clock Replacement Algorithm
        while (1) {
            if (referencedBit[clockHand] == 0) {
                // Found a page to evict
                victim = frameTable[clockHand];
                frameTable[clockHand].pageNo = page_number; // Replace with the new page
                referencedBit[clockHand] = 1; // Set reference bit for the new page
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
                break;
            } else {
                // Clear reference bit and give second chance
                referencedBit[clockHand] = 0; 
                clockHand = (clockHand + 1) % numFrames; // Move the clock hand
            }
        }
    }
    
    return victim; // Return the selected victim page
}







// int main(int argc, char *argv[]) 
//     // Main function code remains the same...
// {
// 	char *tracename; // name of the file 
// 	int	page_number,frame_no, done;
// 	int	do_line, i;

// 	// disk_writes and reads are operations that do not convern with the physical memory, only with the disk.
// 	int	no_events, disk_writes, disk_reads;
// 	int debugmode;
//  	enum repl replace;
// 	int	allocated=0; 
// 	int	victim_page;
//     unsigned address;
//     char  rw;
// 	page Pvictim;
// 	FILE *trace;

// 	/*
// 	The simulator accepts 4 arguments, although the whole program, of course, an extra argument being the program's name.
// 	arg[0]: memsim.c
// 	arg[1]: The name of the memory trace file to use.
// 	arg[2]: The number of page frames in the simulated memory.
// 	arg[3]: The page replacement algorithm to use: rand/lru/rand
// 	arg[4]: the mode to run: quiet/debug.
// 	*/


	
//     if (argc < 5) {
//     	printf( "Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
//     	exit ( -1);
// 	}else {
//         tracename = argv[1];	
// 		trace = fopen( tracename, "r");

// 		// Checks whether the trace file can be opened.
// 		if (trace == NULL ) {
//     	         printf( "Cannot open trace file %s \n", tracename);
//     	         exit ( -1);
// 		}

// 		// Checks for the correct number of frames. 
// 		numFrames = atoi(argv[2]);
//     	if (numFrames < 1) {
//     	    printf( "Frame number must be at least 1\n");
//     	    exit ( -1);
//     	}

// 		// Search for a replacement algorithm.
//     	if (strcmp(argv[3], "lru\0") ==  0) replace = Lru;
// 		else if(strcmp(argv[3], "rand\0") == 0) replace = Random;
// 		else if(strcmp(argv[3], "clock\0") == 0) replace = Clock;		 
// 		else if(strcmp(argv[3], "fifo\0") == 0) replace = Fifo;		 
// 		else{
//     		printf( "Replacement algorithm must be rand/fifo/lru/clock  \n");
//     		exit( -1);
// 		}

// 		// Checks for which mode to take into account at the end. 
//     	if (strcmp(argv[4], "quiet\0") == 0) debugmode = 0;
// 		else if (strcmp(argv[4], "debug\0") == 0) debugmode = 1;
//     	else 
// 		{
//     		printf( "Replacement algorithm must be quiet/debug  \n");
//     		exit ( -1);

// 		}
// 		// Here, all of the input parameters have been validated.

// 		// ========================================================
// 		done = createMMU (numFrames);
// 		if (done == -1) {
// 			 printf( "Cannot create MMU");
// 			 exit(-1);
//     	}

// 		// Number of pages referenced by the CPU.
// 		no_events = 0;
// 		disk_writes = 0;
// 		disk_reads = 0;

// 		// Read the address, followed by a space and then a character ('R' or 'W')
// 		do_line = fscanf(trace,"%x %c",&address,&rw); // address and read/write 

// 		// While the output has 2 lines, continue.
// 		while (do_line == 2)
// 		{
// 			page_number =  address >> pageoffset; // perform a right-bit shift pageoffset times to get the VPN.
// 			frame_no = checkInMemory(page_number);    /* ask for physical address */

// 			if (frame_no == -1){ // page is not in the physical memory. It's a page fault.
// 				disk_reads++;	/* Page fault, need to read from HD and load it into memory  */
// 				if (debugmode) printf( "Page fault %8d \n", page_number) ;
				
// 				/**************************************** */
// 				if (allocated < numFrames)  			/* allocate it to an empty frame */
// 				{
//             		frame_no = allocateFrame(page_number);
// 		    		allocated++;
//         		}
// 				/**************************************** */
        		
				
// 				else // Implementing the page replacement algorithms.
// 				{
// 					Pvictim = selectVictim(page_number, replace) ;   /* returns page number of the page to-be-evicted*/
// 					frame_no = checkInMemory(page_number) ;    /* find out the frame the new page is in */
		   			
// 					if (Pvictim.modified) /*Is the to-be-evicted page dirty/modified?*/
// 					{
//             			disk_writes++; // if so, write it to disk.
//             			if (debugmode) printf( "Disk write %8d \n", Pvictim.pageNo) ;
// 		    		}
// 					// Pages that are not dirty/modified may simbly be discarded as their content is already on disk.
// 		   			if (debugmode) printf( "Discard    %8d \n", Pvictim.pageNo) ;
// 				}
// 			}



// 			// Checking the last character 
// 			if (rw == 'R'){
// 			    if (debugmode) printf( "reading    %8d \n", page_number) ;
// 			}
// 			else if (rw == 'W'){
// 			    // mark page in page table as written - modified  
// 			    if (debugmode) printf( "writting   %8d \n", page_number) ;
// 			}
// 			else {
// 			    printf( "Badly formatted file. Error on line %d\n", no_events+1); 
// 			    exit (-1);
// 			}
			

// 			no_events++; // increment the number of events 

//         	do_line = fscanf(trace,"%x %c",&address,&rw);
// 		}


// 		// At the end, we print a summary of the disk assesses and the page fault rate. 
// 		printf( "total memory frames:  %d\n", numFrames);
// 		printf( "events in trace:      %d\n", no_events);
// 		printf( "total disk reads:     %d\n", disk_reads);
// 		printf( "total disk writes:    %d\n", disk_writes);
// 		printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);
// 	}

// 	// fclose(trace);
// }		


// Main function
int main(int argc, char *argv[]) 
{
    char *tracename; // Name of the file 
    int	page_number, frame_no;
    int	no_events = 0, disk_writes = 0, disk_reads = 0;
    int debugmode;
    enum repl replace;
    page Pvictim;
    FILE *trace;
    unsigned address;
    char rw;

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

        if (strcmp(argv[3], "lru\0") == 0) replace = Lru;
        else if (strcmp(argv[3], "rand\0") == 0) replace = Random;
        else if (strcmp(argv[3], "clock\0") == 0) replace = Clock;		 
        else if (strcmp(argv[3], "fifo\0") == 0) replace = Fifo;		 
        else {
            printf("Replacement algorithm must be rand/fifo/lru/clock\n");
            exit(-1);
        }

        if (strcmp(argv[4], "quiet\0") == 0) debugmode = 0;
        else debugmode = 1;

        createMMU(numFrames);

        while (fscanf(trace, "%x %c\n", &address, &rw) != EOF) {
            page_number = address >> pageoffset;
            no_events++;

            if ((frame_no = checkInMemory(page_number)) != -1) {
                // Page hit
                if (rw == 'W') frameTable[frame_no].modified = 1;
            } else {
                // Page miss
                disk_reads++;
                if ((frame_no = allocateFrame(page_number)) == -1) {
                    // No empty frame, select a victim
                    Pvictim = selectVictim(page_number, replace);
                    if (Pvictim.modified == 1) {
                        disk_writes++; // Write to disk before evicting
                    }
                }
            }

            // if (debugmode) {
            //     printf("Address: 0x%x Page: %d %s\n", address, page_number, frame_no != -1 ? "HIT" : "MISS");
            // }
        }

        fclose(trace);

        // printf("Number of events: %d\n", no_events);
        // printf("Disk reads: %d\n", disk_reads);
        // printf("Disk writes: %d\n", disk_writes);
		printf( "total memory frames:  %d\n", numFrames);
		printf( "events in trace:      %d\n", no_events);
		printf( "total disk reads:     %d\n", disk_reads);
		printf( "total disk writes:    %d\n", disk_writes);
		printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);

    }

    return 0;
}