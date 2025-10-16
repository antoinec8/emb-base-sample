#ifndef GERADORA_H
#define GERADORA_H

#include <zephyr/kernel.h>

// Thread stacks
#define STACK_SIZE 1024
extern K_THREAD_STACK_DEFINE(temp_stack_area, STACK_SIZE);
extern K_THREAD_STACK_DEFINE(humid_stack_area, STACK_SIZE);

// Thread data
extern struct k_thread temp_thread;
extern struct k_thread humid_thread;

// Thread entry points
void temp_thread_entry(void *p1, void *p2, void *p3);
void humid_thread_entry(void *p1, void *p2, void *p3);

#endif /* GERADORA_H */