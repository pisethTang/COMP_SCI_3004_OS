#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
        int pageNo;   // page number
        int modified; // dirty bit 
} page;


enum    repl { Random, Fifo, Lru, Clock};
/*
By default: 
Random: 0
Fifo: 1
Lru: 2
Clock: 3
*/

/*=====================================To-Do====================================*/
int     createMMU( int);
int     checkInMemory( int ) ;
int     allocateFrame( int ) ;
page    selectVictim( int, enum repl) ;
/*=====================================To-Do====================================*/



const int physicalMemorySize = 4096;
const   int pageoffset = 12;    /* Page size is fixed to 4 KB */
int     numFrames = physicalMemorySize / pageoffset;

// The frame offset := number of bits required to represent a word in a frame.
// Frame offset = Page offset 
// ====> numFrames = physicalMemorySize/Page offset




/* Creates the page table structure to record memory allocation */
int createMMU (int frames)
{

    // to do   
	return 0;
}

/* Checks for residency: returns frame no or -1 if not found */
int checkInMemory( int page_number)
{
	int result = -1;


    // to do
    return result ;
}

/* allocate page to the next free frame and record where it put it */
int allocateFrame( int page_number)
{
    // to do
    return 0;
}

/* Selects a victim for eviction/discard according to the replacement algorithm,  returns chosen frame_no  */
page selectVictim(int page_number, enum repl  mode)
{
	page victim;
	// to do 
	victim.pageNo = 0;
	victim.modified = 0;
	return (victim) ;
}

		
int main(int argc, char *argv[])
{
	char	*tracename;
	int	page_number,frame_no, done ;
	int	do_line, i;
	int	no_events, disk_writes, disk_reads;
	int     debugmode;
 	enum	repl  replace;
	int	allocated=0; 
	int	victim_page;
    unsigned address;
    char 	rw;
	page	Pvictim;
	FILE	*trace;

	/*
	The simulator accepts 4 arguments, although the whole program, of course, an extra argument being the program's name.
	arg[0]: memsim.c
	arg[1]: The name of the memory trace file to use.
	arg[2]: The number of page frames in the simulated memory.
	arg[3]: The page replacement algorithm to use: rand/lru/rand
	arg[4]: the mode to run: quiet/debug.
	
	*/
	printf("Hello, world!\n");
    // if (argc < 5) {
    // 	printf( "Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
    // 	exit ( -1);
	// }else {
    //     tracename = argv[1];	
	// 	trace = fopen( tracename, "r");
	
	// 	if (trace == NULL ) {
    // 	         printf( "Cannot open trace file %s \n", tracename);
    // 	         exit ( -1);
	// 	}
	// 	numFrames = atoi(argv[2]);
    // 	if (numFrames < 1) {
    // 	    printf( "Frame number must be at least 1\n");
    // 	    exit ( -1);
    // 	}


    // 	if (strcmp(argv[3], "lru\0") ==  0)replace = Lru;
	// 	else if (strcmp(argv[3], "rand\0") == 0) replace = Random;
	// 	else if (strcmp(argv[3], "clock\0") == 0) replace = Clock;		 
	// 	else if (strcmp(argv[3], "fifo\0") == 0) replace = Fifo;		 
	// 	else {
    // 		printf( "Replacement algorithm must be rand/fifo/lru/clock  \n");
    // 		exit ( -1);
	// 	}

    // 	if (strcmp(argv[4], "quiet\0") == 0) debugmode = 0;
	// 	else if (strcmp(argv[4], "debug\0") == 0) debugmode = 1;
    // 	else 
	// 	{
    // 		printf( "Replacement algorithm must be quiet/debug  \n");
    // 		exit ( -1);

	// 	}
	
	// 	done = createMMU (numFrames);
	// 	if ( done == -1 ) {
	// 		 printf( "Cannot create MMU" ) ;
	// 		 exit(-1);
    // 	}

	// 	no_events = 0 ;
	// 	disk_writes = 0 ;
	// 	disk_reads = 0 ;
	// 	do_line = fscanf(trace,"%x %c",&address,&rw);
	
	// 	while (do_line == 2)
	// 	{
	// 		page_number =  address >> pageoffset;
	// 		frame_no = checkInMemory( page_number) ;    /* ask for physical address */

	// 		if ( frame_no == -1 )
	// 		{
	// 		disk_reads++ ;			/* Page fault, need to load it into memory */
	// 		if (debugmode) printf( "Page fault %8d \n", page_number) ;
	// 		if (allocated < numFrames)  			/* allocate it to an empty frame */
	// 		{
    //         	frame_no = allocateFrame(page_number);
	// 	    	allocated++;
    //     	}

    //     	else
	// 		{
	// 			Pvictim = selectVictim(page_number, replace) ;   /* returns page number of the victim  */
	// 			frame_no = checkInMemory( page_number) ;    /* find out the frame the new page is in */
	// 	   		if (Pvictim.modified)           /* need to know victim page and modified  */
	//  	    	{
    //         		disk_writes++;			    
    //         		if (debugmode) printf( "Disk write %8d \n", Pvictim.pageNo) ;
	// 	    	}
	// 	   		if (debugmode) printf( "Discard    %8d \n", Pvictim.pageNo) ;
	// 		}

	// 		}


	// 		if ( rw == 'R'){
	// 		    if (debugmode) printf( "reading    %8d \n", page_number) ;
	// 		}
	// 		else if ( rw == 'W'){
	// 		    // mark page in page table as written - modified  
	// 		    if (debugmode) printf( "writting   %8d \n", page_number) ;
	// 		}
	// 		else {
	// 		    printf( "Badly formatted file. Error on line %d\n", no_events+1); 
	// 		    exit (-1);
	// 		}

	// 		no_events++;
    //     	do_line = fscanf(trace,"%x %c",&address,&rw);
	// 	}

	// 	printf( "total memory frames:  %d\n", numFrames);
	// 	printf( "events in trace:      %d\n", no_events);
	// 	printf( "total disk reads:     %d\n", disk_reads);
	// 	printf( "total disk writes:    %d\n", disk_writes);
	// 	printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);
	// }
}		