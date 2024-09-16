/*
 * rep_alg_lru.c - Implements least recently used page replacement.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */


#include "rep_alg_default.h"

extern char *progname;

/* stack for LRU implementation */
struct page_stack {
    int page;
    struct page_stack *next;
    struct page_stack *prev;
} *page_stack_top, *page_stack_bottom;

void print_stack();

void lru_read(int page_number) {
    if (page_stack_top == NULL) {
        /* stack is empty, push a new element on */
        struct page_stack *new =
            (struct page_stack *) malloc(sizeof(struct page_stack));
        if (new == NULL) {
            perror(progname);
            exit(EXIT_FAILURE);
        }
        new->page = page_number;
        new->next = NULL;
        new->prev = NULL;
        page_stack_top = page_stack_bottom = new;
    } else {
        /* see if the page is already listed, if so, remove it. */
        struct page_stack *element = page_stack_top;
        while (element != NULL) {
            if (element->page == page_number) {
                if (element->next != NULL) {
                    /* somewhere below the top */
                    element->next->prev = element->prev;
                } else {
                    /* at the top */
                    page_stack_top = element->prev;
                }
                if (element->prev != NULL) {
                    /* somewhere above the last element */
                    element->prev->next = element->next;
                } else {
                    /* at the bottom */
                    page_stack_bottom = element->next;
                }
                break;
            }
            element = element->prev;
        }
        
        if (element == NULL) {
            /* didn't find the element, allocate a new one */
            element = (struct page_stack *) malloc(sizeof(struct page_stack));
            if (element == NULL) {
                perror(progname);
                exit(EXIT_FAILURE);
            }
            element->page = page_number;
        }

        /* push it on the top */
        if (page_stack_top != NULL) {
            page_stack_top->next = element;
        }
        element->next = NULL;
        element->prev = page_stack_top;
        page_stack_top = element;
    }
    /* printf("referenced a page\n");
       print_stack(); */
}

void lru_write(int page_number) {
    /* do the default write */
    default_write(page_number);
    /* put back on the top of the stack */
    lru_read(page_number);
}



page *lru_select_victim(int page_number) {
    page *victim = NULL;
    if (page_stack_bottom != NULL) {
        struct page_stack *new_bottom = page_stack_bottom->next;
        page *target = get_page(page_stack_bottom->page);
        victim = (page *) malloc(sizeof(page));
        if (victim == NULL) {
            perror(progname);
            exit(EXIT_FAILURE);
        }

        /* copy page information to victim */
        victim->page = target->page;
        victim->modified = target->modified;

        /* update info in page table */
        target->page = page_number;
        target->modified = 0;
        
        /* 
         * Remove the LRU page from the bottom of the stack,
         * as it has been evicted from the page table. We do
         * do not need to add the new one in as that will be
         * done later by either mmu_impl.read() or
         * mmu_impl.write()
         */
        if (new_bottom != NULL) {
            /* there is a next element */
            new_bottom->prev = NULL;
        } else {
            /* no more elements */
            page_stack_top = NULL;
        }
        free(page_stack_bottom);
        page_stack_bottom = new_bottom;
    }
    /* printf("selected a victim\n");
       print_stack(); */
    return victim;
}



void print_stack() {
    struct page_stack *element = page_stack_top;
    if (element == NULL) {
        printf("stack is empty\n");
    } else {
        printf("stack:\n");
        while (element != NULL) {
            printf("  page: %i\n", element->page);
            element = element->prev;
        }
    }
}




mmu_impl lru_mmu = {
    default_create_mmu,
    default_check_in_memory,
    default_allocate_frame,
    lru_select_victim,
    lru_read,
    lru_write
};

