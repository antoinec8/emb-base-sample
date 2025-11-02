#ifndef APP_MODULE_H
#define APP_MODULE_H

#include <zephyr/kernel.h>

/**
 * @brief Entry point da thread Application
 * 
 * Esta thread subscreve ao time_channel e calcula
 * intervalos de tempo entre eventos
 */
void app_entry(void *p1, void *p2, void *p3);

/**
 * @brief Declaração da thread e stack
 */
extern struct k_thread app_thread;
extern K_THREAD_STACK_DEFINE(app_stack_area, APP_STACK_SIZE);

#endif /* APP_MODULE_H */
