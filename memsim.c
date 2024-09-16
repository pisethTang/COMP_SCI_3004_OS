/*
 * memsim.c - Main simulator program code.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 * Based on supplied memsim.c file.
 */


#include "rep_alg_default.h"

/*
 * Notes: I have modified the provided memsim.c a fair bit, mainly
 * to allow MMU implementations to be used in a nicer way.
 * Each implementation creates a mmu_impl struct that contains
 * contains pointers to its implementation of those functions. Then,
 * when the code below reads the command line arguments supplied to
 * memsim, it takes the appropriate implementations's mmu_impl struct
 * and assigns it to the "mmu" variable in this file. When the MMU
 * needs to be invoked, these function pointers are used.
 * 
 * This allowed the removal of the repl enum and the MMU functions
 * from this file, simplifying it somewhat and letting new
 * implementations to be easily plugged in in the future.
 *
 * While I cleaned up the formatting of main() somewhat, the actual
 * algorithm remains the same.
 */

void print_error(const char *);
void perror2(const char *);
void print_usage();
int parse_arguments(char *argv[], FILE **trace, int *frame_count);

/* selected algorithim implementation */
mmu_impl mmu;

/* available algorithim implementations */
extern mmu_impl rand_mmu;
extern mmu_impl lru_mmu;
extern mmu_impl esc_mmu;
extern mmu_impl clock_mmu;

/* the page table and next frame marker */
page *page_table;
page *next_free_page;

int page_offset = 12; /* Page size is fixed to 4 KB */
char *progname;
int debug;



int main(int argc, char *argv[]) {
    /* program arguments */
    FILE *trace;
    int frame_count;

    int event_count, disk_writes, disk_reads; /* stats */

	int	page_number, frame_no;
	int	allocated = 0;
    // int modified = 0;
	// int	victim_page;
    unsigned address;
    char rw;
	// int i;
	page *victim;

    /* save for printing errors, etc */
    progname = argv[0];

    if (argc != 5 ||
        parse_arguments(argv, &trace, &frame_count) == 0) {
        print_usage();
        exit(EXIT_FAILURE);
    }
	
	if (mmu.create_mmu(frame_count) != 1) {
        exit(EXIT_FAILURE);
    }

	event_count = 0 ;
	disk_writes = 0 ;
	disk_reads = 0 ;

	while (fscanf(trace, "%x %c", &address, &rw) == 2) {
		page_number =  address >> page_offset;

        /* ask for physical address */
		frame_no = mmu.check_in_memory(page_number);

		if (frame_no == -1) {
            /* Page fault, need to load it into memory */
            disk_reads++;
            if (debug) {
                printf("Page fault %8d \n", page_number);
            }
            if (allocated < frame_count) { /* allocate it to an empty frame */
                mmu.allocate_frame(page_number);
                allocated++;
            } else {
                /* find frameNo of the victim  */
                victim = mmu.select_victim(page_number);

                /* need to know victim page and modified  */
                if (victim->modified) {
                    disk_writes++;			    
                    if (debug) {
                        printf( "Disk write %8d \n", victim->page);
                    }
                } else if (debug) {
                    printf( "Discard    %8d \n", victim->page);
                }

                free(victim);
            }
        }

        if (rw == 'R') {
            mmu.read(page_number);
			if (debug) {
                printf( "reading    %8d \n", page_number);
            }
        } else if (rw == 'W') {
            mmu.write(page_number);
            if (debug) {
                printf( "writting   %8d \n", page_number);
            }
        } else {
            printf("Badly formatted file. Error on line %d\n", event_count + 1);
            exit(EXIT_FAILURE);
        }
        event_count++;
	}

	printf("total memory frames:  %d\n", frame_count);
	printf("events in trace:      %d\n", event_count);
	printf("total disk reads:     %d\n", disk_reads);
	printf("total disk writes:    %d\n", disk_writes);
	printf("page fault rate:      %.4f\n",  (float) disk_reads / event_count);
}

int parse_arguments(char *argv[], FILE **trace, int *frame_count) {
    *trace = fopen(argv[1], "r");
    if (*trace == NULL) {
        perror2(argv[1]);
        return 0;
    }
    *frame_count = atoi(argv[2]);
    if (*frame_count < 1) {
        print_error("Frame number must be at least 1.");
        return 0;
    }
    if (strcmp(argv[3], "lru\0") == 0) {
        mmu = lru_mmu;
    } else if (strcmp(argv[3], "rand\0") == 0) {
        mmu = rand_mmu;
    } else if (strcmp(argv[3], "esc\0") == 0) {
        mmu = esc_mmu;
    } else if (strcmp(argv[3], "clock\0") == 0) {
        mmu = clock_mmu;
    } else {
        print_error("Invalid replacement algorithim.");
        return 0;
    }
    if (strcmp(argv[4], "quiet\0") == 0) {
        debug = 0;
    } else if (strcmp(argv[4], "debug\0") == 0) {
        debug = 1;
    } else {
        print_error("Invalid debugging level");
        return 0;
    }

    return 1;
}

    
    		
void perror2(const char *s) {
    int errsv = errno;
    fprintf(stderr, "%s: ", progname);
    errno = errsv;
    perror(s);
}
	
void print_error(const char *s) {
    fprintf(stderr, "%s: %s\n", progname, s);
}
	
void print_usage() {
    fprintf
        (stderr,
         "Usage: %s [TRACE-FILE] [FRAME-COUNT] [rand|lru|esc] [quiet|debug]\n",
         progname);
    fprintf(stderr, "  TRACE-FILE  - path to file containing memory traces\n");
    fprintf(stderr, "  FRAME-COUNT - size of the page table to use\n");
    fprintf(stderr, "\n");
}
