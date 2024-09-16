/*
 * memsim.h - Common simulator types.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 * Based on supplied memsim.c file.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int page;
	int modified;
	int reference;
} page;



typedef struct {
    int (*create_mmu)(int);
    int	(*check_in_memory)(int);
    void (*allocate_frame)(int);
    page *(*select_victim)(int);
    void (*read)(int);
    void (*write)(int);
} mmu_impl;

