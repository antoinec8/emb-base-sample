#ifndef PROCESSADORA_H
#define PROCESSADORA_H

#include <zephyr/kernel.h>

// Thread stack
#define STACK_SIZE 1024
extern K_THREAD_STACK_DEFINE(consumer_stack_area, STACK_SIZE);

// Thread data
extern struct k_thread consumer_thread;

// Thread entry point
void consumer_thread_entry(void *p1, void *p2, void *p3);

#endif /* PROCESSADORA_H */