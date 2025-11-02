#ifndef LOGGER_MODULE_H
#define LOGGER_MODULE_H

#include <zephyr/kernel.h>

/**
 * @brief Entry point da thread Logger
 * 
 * Esta thread subscreve ao time_channel e registra
 * timestamps quando recebe atualizações
 */
void logger_entry(void *p1, void *p2, void *p3);

/**
 * @brief Declaração da thread e stack
 */
extern struct k_thread logger_thread;
extern K_THREAD_STACK_DEFINE(logger_stack_area, LOGGER_STACK_SIZE);

#endif /* LOGGER_MODULE_H */
