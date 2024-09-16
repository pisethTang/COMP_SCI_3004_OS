/*
 * rep_alg_default.h - Default MMU function implementations.
 *
 * Copright (C) 2005, Michael Gratton (1045374).
 */
 #include "memsim.h"


// int page_table_size;
// page *next_victim;


int default_create_mmu(int);
int	default_check_in_memory(int);
void default_allocate_frame(int);
page *default_select_victim(int);
void default_read(int);
void default_write(int);

page *get_page(int);

