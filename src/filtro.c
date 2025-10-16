#include <zephyr/kernel.h>
#include "common.h"
#include "filtro.h"

// Define stack para a thread
K_THREAD_STACK_DEFINE(filter_stack_area, STACK_SIZE);

// Estrutura da thread
struct k_thread filter_thread;

// Verifica se o valor está dentro da faixa válida
static bool is_value_valid(enum sensor_type type, int value)
{
    switch (type) {
        case SENSOR_TEMP:
            return (value >= TEMP_MIN && value <= TEMP_MAX);
        case SENSOR_UMID:
            return (value >= UMID_MIN && value <= UMID_MAX);
        default:
            return false;
    }
}

// Thread de filtragem
void filter_thread_entry(void *p1, void *p2, void *p3)
{
    struct sensor_data data;

    while (1) {
        // Aguarda dados na fila de entrada
        k_msgq_get(&input_msgq, &data, K_FOREVER);

        // Valida os dados
        if (is_value_valid(data.type, data.value)) {
            // Dados válidos: envia para fila de saída
            if (k_msgq_put(&output_msgq, &data, K_NO_WAIT) != 0) {
                printk("Fila de saída cheia, descartando dado válido\n");
            }
        } else {
            // Dados inválidos: registra no log
            printk("Dado inválido: tipo=%d, valor=%d, timestamp=%u\n",
                   data.type, data.value, data.timestamp);
        }

        k_sleep(K_MSEC(100));  // Evita sobrecarregar o sistema
    }
}