#ifndef SNTP_CLIENT_H
#define SNTP_CLIENT_H

#include <zephyr/kernel.h>

/**
 * @brief Entry point da thread SNTP Client
 * 
 * Esta thread realiza sincronização periódica com servidor SNTP
 * e publica atualizações no canal ZBus quando bem-sucedida
 */
void sntp_client_entry(void *p1, void *p2, void *p3);

/**
 * @brief Declaração da thread e stack
 */
extern struct k_thread sntp_thread;
extern K_THREAD_STACK_DEFINE(sntp_stack_area, SNTP_STACK_SIZE);

#endif /* SNTP_CLIENT_H */
