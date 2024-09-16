/*
 * rep_alg_clock.c - Implements enhanced second chance page replacement.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */


// #include "memsim.h"
#include "rep_alg_default.h"


void print_table();
page *get_victim(int, int);

extern page *page_table;
extern page *next_free_page;

extern char *progname;

extern int page_table_size;
extern page *next_victim;

int clock_create_mmu(int frame_count) {
    if (default_create_mmu(frame_count)) {
        next_victim = page_table;
        return 1;
    }
    return 0;
}

void clock_read(int page_number) {
    get_page(page_number)->reference = 1;
}

void clock_write(int page_number) {
    default_write(page_number);
    clock_read(page_number);
}

page *clock_select_victim(int page_number) {
    page *target = next_victim;
    page *victim;

#ifdef DEBUG
    printf("*** clock_select_victim() *** \n");
    printf("+before: \n");
    print_table();
#endif


    /* clear reference bits between the start point and the target */
    while (next_victim->reference == 1) {
            /* printf("clearing...\n");*/
            next_victim->reference = 0;
            next_victim++;
            if (next_victim == next_free_page) {
                /* reached end of the table, start from the start */
                next_victim = page_table;
            }
    }
    
     target = next_victim;

    /* construct the victim */
    victim = (page *) malloc(sizeof(page));
    if (victim == NULL) {
        perror(progname);
        exit(EXIT_FAILURE);
    }
    victim->page = target->page;
    victim->reference = target->reference;
    victim->modified = target->modified;

    /* update the page table */
    target->page = page_number;
    target->reference = 1;
    target->modified = 0;

    /* move the next victim past the target */
    next_victim++;
    if (next_victim == next_free_page) {
        /* reached end of the queue, start from the start */
        next_victim = page_table;
    }

#ifdef DEBUG
    printf("+after: \n");
    print_table();
#endif
    
    return victim;
}




void print_table() {
    page *frame = page_table;
    if (page_table == next_free_page) {
        printf("* page table is empty\n");
    } else {
        printf("* page table:\n");
        while (frame != next_free_page) {
            if (frame == next_victim) {
                printf("  %i: r: %i, m: %i (next victim)\n",
                       frame->page, frame->reference, frame->modified);
            } else {
                printf("  %i: r: %i, m: %i\n",
                       frame->page, frame->reference, frame->modified);
            }
            frame++;
        }
    }

}


mmu_impl clock_mmu = {
    clock_create_mmu,
    default_check_in_memory,
    default_allocate_frame,
    clock_select_victim,
    clock_read,
    clock_write
};

