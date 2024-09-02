#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int pageNo;   // page number
    int modified; // dirty bit 
} page;

enum repl { Random, Fifo, Lru, Clock }; // Replacement algorithms

const int pageoffset = 12;    
int numFrames; // Number of frames, given by the user.

// Initializes the MMU and page table.
int createMMU(int frames) {
  

    return 0;
}

// Checks if a page is in memory and returns the frame number or -1 if not found.
int checkInMemory(int page_number) {
    for (int i = 0; i < numFrames; i++) {
        if (frameTable[i] == page_number) {
            pageTable[i].lastUsed = currentTime++; // Update the last used time for LRU
            return i; // Frame number found
        }
    }
    return -1; // Page not found in memory
}

// Allocates a frame for a page if there's an empty frame available.
int allocateFrame(int page_number) {
	
    return -1; // No empty frame available
}

// Selects a victim for eviction/discard according to the replacement algorithm.
page selectVictim(int page_number, enum repl mode) {
    page victim;
    victim.pageNo = -1;
    victim.modified = 0;

    // Implement replacement algorithm logic here based on `mode`.

    return victim; // Return the selected victim page
}

int main(int argc, char *argv[]) 
    // Main function code remains the same...
{
	char *tracename;
	int	page_number,frame_no, done ;
	int	do_line, i;
	int	no_events, disk_writes, disk_reads;
	int debugmode;
 	enum repl replace;
	int	allocated=0; 
	int	victim_page;
    unsigned address;
    char  rw;
	page Pvictim;
	FILE *trace;

	/*
	The simulator accepts 4 arguments, although the whole program, of course, an extra argument being the program's name.
	arg[0]: memsim.c
	arg[1]: The name of the memory trace file to use.
	arg[2]: The number of page frames in the simulated memory.
	arg[3]: The page replacement algorithm to use: rand/lru/rand
	arg[4]: the mode to run: quiet/debug.
	
	*/
    if (argc < 5) {
    	printf( "Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
    	exit ( -1);
	}else {
        tracename = argv[1];	
		trace = fopen( tracename, "r");

		// Checks whether the trace file can be opened.
		if (trace == NULL ) {
    	         printf( "Cannot open trace file %s \n", tracename);
    	         exit ( -1);
		}

		// Checks for the correct number of frames. 
		numFrames = atoi(argv[2]);
    	if (numFrames < 1) {
    	    printf( "Frame number must be at least 1\n");
    	    exit ( -1);
    	}

		// Search for a replacement algorithm.
    	if (strcmp(argv[3], "lru\0") ==  0) replace = Lru;
		else if(strcmp(argv[3], "rand\0") == 0) replace = Random;
		else if(strcmp(argv[3], "clock\0") == 0) replace = Clock;		 
		else if(strcmp(argv[3], "fifo\0") == 0) replace = Fifo;		 
		else{
    		printf( "Replacement algorithm must be rand/fifo/lru/clock  \n");
    		exit( -1);
		}

		// Checks for which mode to take into account at the end. 
    	if (strcmp(argv[4], "quiet\0") == 0) debugmode = 0;
		else if (strcmp(argv[4], "debug\0") == 0) debugmode = 1;
    	else 
		{
    		printf( "Replacement algorithm must be quiet/debug  \n");
    		exit ( -1);

		}
		// Here, all of the input parameters have been validated.

		// ========================================================
		done = createMMU (numFrames);
		if (done == -1) {
			 printf( "Cannot create MMU" ) ;
			 exit(-1);
    	}

		// Number of pages referenced by the CPU.
		no_events = 0;
		disk_writes = 0;
		disk_reads = 0;
		// Read the address, followed by a space and then a character ('R' or 'W')
		do_line = fscanf(trace,"%x %c",&address,&rw); // address and read/write 

		// While the output has 2 lines, continue.
		while (do_line == 2)
		{
			page_number =  address >> pageoffset;
			frame_no = checkInMemory(page_number);    /* ask for physical address */

			if (frame_no == -1){
				disk_reads++ ;			/* Page fault, need to load it into memory */
				if (debugmode) printf( "Page fault %8d \n", page_number) ;
				if (allocated < numFrames)  			/* allocate it to an empty frame */
				{
            		frame_no = allocateFrame(page_number);
		    		allocated++;
        		}
        		else
				{
					Pvictim = selectVictim(page_number, replace) ;   /* returns page number of the victim  */
					frame_no = checkInMemory(page_number) ;    /* find out the frame the new page is in */
		   			
					if (Pvictim.modified) /* need to know victim page and modified  */
					{
            			disk_writes++;			    
            			if (debugmode) printf( "Disk write %8d \n", Pvictim.pageNo) ;
		    		}
		   			if (debugmode) printf( "Discard    %8d \n", Pvictim.pageNo) ;
				}
			}

			// Checking the last character 
			if (rw == 'R'){
			    if (debugmode) printf( "reading    %8d \n", page_number) ;
			}
			else if (rw == 'W'){
			    // mark page in page table as written - modified  
			    if (debugmode) printf( "writting   %8d \n", page_number) ;
			}
			else {
			    printf( "Badly formatted file. Error on line %d\n", no_events+1); 
			    exit (-1);
			}
			

			no_events++; // increment the number of events to mark the end of 
        	do_line = fscanf(trace,"%x %c",&address,&rw);
		}


		// At the end, we print a summary of the disk assesses and the page fault rate. 
		printf( "total memory frames:  %d\n", numFrames);
		printf( "events in trace:      %d\n", no_events);
		printf( "total disk reads:     %d\n", disk_reads);
		printf( "total disk writes:    %d\n", disk_writes);
		printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);
	}
}		