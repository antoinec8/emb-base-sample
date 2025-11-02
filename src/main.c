#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "common.h"
#include "geradora.h"
#include "filtro.h"
#include "processadora.h"
#include "time_common.h"
#include "sntp_client.h"
#include "logger_module.h"
#include "app_module.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// Buffers para as filas
char __aligned(4) input_msgq_buffer[MAX_QUEUE_SIZE * sizeof(struct sensor_data)];
char __aligned(4) output_msgq_buffer[MAX_QUEUE_SIZE * sizeof(struct sensor_data)];

// Definição das filas
struct k_msgq input_msgq;
struct k_msgq output_msgq;

void main(void)
{
    LOG_INF("===========================================");
    LOG_INF("Sistema Embarcado - Atividade 04");
    LOG_INF("SNTP + ZBus - Sincronização de Tempo");
    LOG_INF("===========================================");

    // Inicializa as filas de mensagens (sistema da atividade 03)
    k_msgq_init(&input_msgq, input_msgq_buffer, sizeof(struct sensor_data), 10);
    k_msgq_init(&output_msgq, output_msgq_buffer, sizeof(struct sensor_data), 10);

    LOG_INF("Inicializando threads do sistema de sensores...");

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

    LOG_INF("Inicializando threads do sistema SNTP/ZBus...");

    // Cria thread do cliente SNTP
    k_thread_create(&sntp_thread,
                   sntp_stack_area,
                   K_THREAD_STACK_SIZEOF(sntp_stack_area),
                   sntp_client_entry,
                   NULL, NULL, NULL,
                   SNTP_PRIORITY,
                   0,
                   K_NO_WAIT);

    // Cria thread do Logger
    k_thread_create(&logger_thread,
                   logger_stack_area,
                   K_THREAD_STACK_SIZEOF(logger_stack_area),
                   logger_entry,
                   NULL, NULL, NULL,
                   LOGGER_PRIORITY,
                   0,
                   K_NO_WAIT);

    // Cria thread da Application
    k_thread_create(&app_thread,
                   app_stack_area,
                   K_THREAD_STACK_SIZEOF(app_stack_area),
                   app_entry,
                   NULL, NULL, NULL,
                   APP_PRIORITY,
                   0,
                   K_NO_WAIT);

    LOG_INF("Todas as threads iniciadas com sucesso!");
    LOG_INF("===========================================");
}
