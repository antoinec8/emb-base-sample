#include <zephyr/kernel.h>
#include "common.h"
#include "geradora.h"
#include "filtro.h"
#include "processadora.h"

// Buffers para as filas
char __aligned(4) input_msgq_buffer[MAX_QUEUE_SIZE * sizeof(struct sensor_data)];
char __aligned(4) output_msgq_buffer[MAX_QUEUE_SIZE * sizeof(struct sensor_data)];

// Definição das filas
struct k_msgq input_msgq;
struct k_msgq output_msgq;

void main(void)
{
    // Inicializa as filas de mensagens
    k_msgq_init(&input_msgq, input_msgq_buffer, sizeof(struct sensor_data), 10);
    k_msgq_init(&output_msgq, output_msgq_buffer, sizeof(struct sensor_data), 10);

    // Cria thread do sensor de temperatura
    k_thread_create(&temp_thread,
                   temp_stack_area,
                   K_THREAD_STACK_SIZEOF(temp_stack_area),
                   temp_thread_entry,
                   NULL, NULL, NULL,
                   THREAD_PRIORITY,
                   0,
                   K_NO_WAIT);

    // Cria thread do sensor de umidade
    k_thread_create(&humid_thread,
                   humid_stack_area,
                   K_THREAD_STACK_SIZEOF(humid_stack_area),
                   humid_thread_entry,
                   NULL, NULL, NULL,
                   THREAD_PRIORITY,
                   0,
                   K_NO_WAIT);

    // Cria thread do filtro
    k_thread_create(&filter_thread,
                   filter_stack_area,
                   K_THREAD_STACK_SIZEOF(filter_stack_area),
                   filter_thread_entry,
                   NULL, NULL, NULL,
                   THREAD_PRIORITY,
                   0,
                   K_NO_WAIT);

    // Cria thread consumidora
    k_thread_create(&consumer_thread,
                   consumer_stack_area,
                   K_THREAD_STACK_SIZEOF(consumer_stack_area),
                   consumer_thread_entry,
                   NULL, NULL, NULL,
                   THREAD_PRIORITY,
                   0,
                   K_NO_WAIT);

    printk("Sistema de monitoramento iniciado!\n");
}
