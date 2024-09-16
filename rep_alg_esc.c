/*
 * rep_alg_esc.c - Implements enhanced second chance page replacement.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */

#include <stdlib.h>
#include <stdio.h>
// #include "memsim.h"
#include "rep_alg_default.h"

void print_page_table();
page *get_victim(int, int);

extern page *page_table;
extern page *next_free_page;

extern char *progname;

extern int page_table_size;
page *next_victim;


int esc_create_mmu(int frame_count) {
    if (default_create_mmu(frame_count)) {
        next_victim = page_table;
        return 1;
    }
    return 0;
}

void esc_read(int page_number) {
    get_page(page_number)->reference = 1;
}

void esc_write(int page_number) {
    default_write(page_number);
    esc_read(page_number);
}

page *esc_select_victim(int page_number) {
    page *target = next_victim;
    page *victim;

#ifdef DEBUG
    printf("*** esc_select_victim() *** \n");
    printf("+before: \n");
    print_page_table();
#endif

    /* Pass one: not referenced, clean */
    target = get_victim(0, 0);

    if (target == NULL) {
        /* Pass two: not referenced, dirty */
        target = get_victim(0, 1);

        if (target == NULL) {
            /* Pass three: referenced, clean */
            target = get_victim(1, 0);

            if (target == NULL) {
                /* Pass four: referenced, dirty */
                target = get_victim(1, 1);
            }
        }
    }

    /* clear reference bits between the start point and the target */
    if (next_victim != target) {
        do {
            /* printf("clearing...\n");*/
            next_victim->reference = 0;
            next_victim++;
            if (next_victim == next_free_page) {
                /* reached end of the table, start from the start */
                next_victim = page_table;
            }
        } while (next_victim != target);
    }

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
    print_page_table();
#endif
    
    return victim;
}

page *get_victim(int reference, int modified) {
    page *target = next_victim;
    do {
#ifdef DEBUG
        printf("get_next_victim(%i, %i): [%i] %i, %i\n",
               reference, modified,
               target->page, target->reference, target->modified);
#endif
        if (target->reference == reference && target->modified == modified) {
            return target;
        }
        target++;
        if (target == next_free_page) {
            /* reached end of the table, start from the start */
            target = page_table;
        }
    } while (target != next_victim);
        
    return NULL;
}

void print_page_table() {
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

mmu_impl esc_mmu = {
    esc_create_mmu,
    default_check_in_memory,
    default_allocate_frame,
    esc_select_victim,
    esc_read,
    esc_write
};

