/*
 * rep_alg_rand.c - Implements random page replacement.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */

#include "rep_alg_default.h"

extern page *page_table;


int page_count;



int rand_create_mmu(int frame_count) {
    if (default_create_mmu(frame_count)) {
        page_count = frame_count;
        return 1;
    }
    return 0;
}


page *rand_select_victim(int page_number) {
    page *victim = default_select_victim(page_number);
    page *target;

    /* seed using the current time */
    srand((int) time(NULL));
    target = page_table + (rand() % page_count);

    victim->page = target->page;
    victim->reference = target->reference;
    victim->modified = target->modified;

    target->page = page_number;
    target->reference = 0;
    target->modified = 0;
    
    return victim;
}


mmu_impl rand_mmu = {
    rand_create_mmu,
    default_check_in_memory,
    default_allocate_frame,
    rand_select_victim,
    default_read,
    default_write
};

