#include <zephyr/kernel.h>
#include <stdlib.h>
#include "common.h"
#include "geradora.h"

// Define stacks para as threads
K_THREAD_STACK_DEFINE(temp_stack_area, STACK_SIZE);
K_THREAD_STACK_DEFINE(humid_stack_area, STACK_SIZE);

// Estruturas das threads
struct k_thread temp_thread;
struct k_thread humid_thread;

// Simula leitura de sensor de temperatura
static int read_temperature(void)
{
    // Simula valores entre 15°C e 35°C
    return (rand() % 21) + 15;
}

// Simula leitura de sensor de umidade
static int read_humidity(void)
{
    // Simula valores entre 30% e 80%
    return (rand() % 51) + 30;
}

// Thread do sensor de temperatura
void temp_thread_entry(void *p1, void *p2, void *p3)
{
    struct sensor_data data;
    data.type = SENSOR_TEMP;

    while (1) {
        data.value = read_temperature();
        data.timestamp = k_uptime_get_32();

        if (k_msgq_put(&input_msgq, &data, K_NO_WAIT) != 0) {
            printk("Fila de entrada cheia, descartando leitura de temperatura\n");
        }

        k_sleep(K_SECONDS(2));
    }
}

// Thread do sensor de umidade
void humid_thread_entry(void *p1, void *p2, void *p3)
{
    struct sensor_data data;
    data.type = SENSOR_UMID;

    while (1) {
        data.value = read_humidity();
        data.timestamp = k_uptime_get_32();

        if (k_msgq_put(&input_msgq, &data, K_NO_WAIT) != 0) {
            printk("Fila de entrada cheia, descartando leitura de umidade\n");
        }

        k_sleep(K_SECONDS(2));
    }
}