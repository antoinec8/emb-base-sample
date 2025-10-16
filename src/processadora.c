#include <zephyr/kernel.h>
#include "common.h"
#include "processadora.h"

// Define stack para a thread
K_THREAD_STACK_DEFINE(consumer_stack_area, STACK_SIZE);

// Estrutura da thread
struct k_thread consumer_thread;

// Thread consumidora
void consumer_thread_entry(void *p1, void *p2, void *p3)
{
    struct sensor_data data;

    while (1) {
        // Aguarda dados na fila de saída
        k_msgq_get(&output_msgq, &data, K_FOREVER);

        // Processa os dados válidos
        switch (data.type) {
            case SENSOR_TEMP:
                printk("Temperatura válida: %d°C (timestamp: %u)\n",
                       data.value, data.timestamp);
                break;
            case SENSOR_UMID:
                printk("Umidade válida: %d%% (timestamp: %u)\n",
                       data.value, data.timestamp);
                break;
            default:
                printk("Tipo de sensor desconhecido\n");
                break;
        }

        k_sleep(K_MSEC(100));  // Evita sobrecarregar o sistema
    }
}