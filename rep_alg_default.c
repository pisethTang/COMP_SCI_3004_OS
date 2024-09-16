/*
 * rep_alg_default.c - Default MMU function implementations.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */

#include <stdlib.h>
// #include "memsim.h"
#include "rep_alg_default.h"
#include <stdio.h>

extern page *page_table;
extern page *next_free_page;

extern char *progname;

int default_create_mmu(int frame_count) {
    /* uses a stack to emulate physical memory */
    page_table = (page *) malloc(sizeof(page) * frame_count);
    if (page_table == NULL) {
        perror(progname);
        return 0;
    }
    next_free_page = page_table;
    return 1;
}

page *get_page(int page_number) {
    page *frame = page_table;
    while (frame != next_free_page) {
        if (frame->page == page_number) {
            return frame;
        }
        frame++;
    }
    return NULL;
}
 
int	default_check_in_memory(int page_number) {
    int index = 0;
    page *frame = page_table;
    
    while (frame != next_free_page) {
        if (frame->page == page_number) {
            return index;
        }
        frame++;
        index++;
    }
    return -1;
}
 
void default_allocate_frame(int page_number) {
    next_free_page->page = page_number;
    next_free_page->reference = 0;
    next_free_page->modified = 0;
    next_free_page++;
}

page *default_select_victim(int page_number) {
    page *victim = (page *) malloc(sizeof(page));
    if (victim == NULL) {
        // perror(progname);
        exit(EXIT_FAILURE);
    }
    victim->page = 0;
    victim->reference = 0;
    victim->modified = 0;
	return victim;
}

void default_read(int page_number) {
    /* do nothing */
}

void default_write(int page_number) {
    get_page(page_number)->modified = 1;
}

