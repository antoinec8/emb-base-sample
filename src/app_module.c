#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include "time_common.h"
#include "app_module.h"

LOG_MODULE_REGISTER(app_module, LOG_LEVEL_INF);

// Definição da thread e stack
K_THREAD_STACK_DEFINE(app_stack_area, APP_STACK_SIZE);
struct k_thread app_thread;

// Armazenamento de timestamps de eventos
static int64_t last_event_timestamp = 0;
static int64_t current_timestamp = 0;
static bool time_available = false;

/**
 * @brief Callback chamado quando nova mensagem de tempo é recebida
 */
static void app_time_callback(const struct zbus_channel *chan)
{
    const struct time_msg *msg = zbus_chan_const_msg(chan);
    
    if (msg->is_synchronized) {
        int64_t previous_timestamp = current_timestamp;
        current_timestamp = msg->timestamp;
        time_available = true;
        
        LOG_INF("=== APPLICATION EVENT ===");
        LOG_INF("Horário recebido: %04d-%02d-%02d %02d:%02d:%02d UTC",
                msg->time_info.tm_year + 1900,
                msg->time_info.tm_mon + 1,
                msg->time_info.tm_mday,
                msg->time_info.tm_hour,
                msg->time_info.tm_min,
                msg->time_info.tm_sec);
        
        if (previous_timestamp > 0) {
            int64_t time_diff = current_timestamp - previous_timestamp;
            LOG_INF("Tempo desde última atualização: %lld segundos", time_diff);
        }
        
        if (last_event_timestamp > 0) {
            int64_t event_diff = current_timestamp - last_event_timestamp;
            LOG_INF("Tempo desde último evento processado: %lld segundos", event_diff);
        }
        
        LOG_INF("========================");
    }
}

// Definir listener para o canal de tempo
ZBUS_LISTENER_DEFINE(app_listener, app_time_callback);

// Adicionar observer ao canal
ZBUS_CHAN_ADD_OBS(time_channel, app_listener, 0);

/**
 * @brief Simula processamento de evento da aplicação
 */
static void process_application_event(void)
{
    if (!time_available) {
        LOG_WRN("Tentativa de processar evento sem tempo sincronizado");
        return;
    }
    
    last_event_timestamp = current_timestamp;
    
    LOG_INF(">>> Evento da aplicação processado");
    LOG_INF(">>> Timestamp do evento: %lld", last_event_timestamp);
    
    // Aqui você pode adicionar lógica de aplicação real
    // Por exemplo: controle de eventos baseado em tempo,
    // agendamento de tarefas, etc.
}

/**
 * @brief Entry point da thread Application
 */
void app_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    LOG_INF("Thread Application iniciada");
    LOG_INF("Aguardando sincronização de tempo...");

    int event_counter = 0;

    while (1) {
        // Aguardar um período
        k_sleep(K_SECONDS(30));
        
        if (time_available) {
            // Simular processamento de evento periódico
            event_counter++;
            LOG_INF("Processando evento periódico #%d", event_counter);
            process_application_event();
            
            // Calcular próximo evento
            if (current_timestamp > 0) {
                LOG_INF("Próximo evento agendado para daqui a 30 segundos");
            }
        } else {
            LOG_WRN("Aguardando sincronização de tempo para processar eventos");
        }
    }
}
