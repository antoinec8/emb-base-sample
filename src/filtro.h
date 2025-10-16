#ifndef FILTRO_H
#define FILTRO_H

#include <zephyr/kernel.h>

// Thread stack
#define STACK_SIZE 1024
extern K_THREAD_STACK_DEFINE(filter_stack_area, STACK_SIZE);

// Thread data
extern struct k_thread filter_thread;

// Thread entry point
void filter_thread_entry(void *p1, void *p2, void *p3);

#endif /* FILTRO_H */