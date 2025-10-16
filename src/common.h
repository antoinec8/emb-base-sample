#ifndef COMMON_H
#define COMMON_H

#include <zephyr/kernel.h>

// Definições de faixas válidas
#define TEMP_MIN 18
#define TEMP_MAX 30
#define UMID_MIN 40
#define UMID_MAX 70

// Tamanho das filas
#define MAX_QUEUE_SIZE 10
#define STACK_SIZE 1024
#define THREAD_PRIORITY 7

// Tipos de dados
enum sensor_type {
    SENSOR_TEMP,
    SENSOR_UMID
};

// Estrutura da mensagem
struct sensor_data {
    enum sensor_type type;
    int value;
    uint32_t timestamp;
};

// Declaração externa das filas
extern struct k_msgq input_msgq;
extern struct k_msgq output_msgq;

#endif /* COMMON_H */