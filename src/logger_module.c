#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include "time_common.h"
#include "logger_module.h"

LOG_MODULE_REGISTER(logger_module, LOG_LEVEL_INF);

// Definição da thread e stack
K_THREAD_STACK_DEFINE(logger_stack_area, LOGGER_STACK_SIZE);
struct k_thread logger_thread;

// Relógio interno do logger
static struct tm internal_clock;
static bool clock_initialized = false;

/**
 * @brief Callback chamado quando nova mensagem de tempo é recebida
 */
static void time_msg_callback(const struct zbus_channel *chan)
{
    const struct time_msg *msg = zbus_chan_const_msg(chan);
    
    if (msg->is_synchronized) {
        // Atualizar relógio interno
        memcpy(&internal_clock, &msg->time_info, sizeof(struct tm));
        clock_initialized = true;
        
        // Registrar log com timestamp atualizado
        LOG_INF("=== LOG ENTRY ===");
        LOG_INF("Timestamp: %lld", msg->timestamp);
        LOG_INF("Data/Hora: %04d-%02d-%02d %02d:%02d:%02d UTC",
                internal_clock.tm_year + 1900,
                internal_clock.tm_mon + 1,
                internal_clock.tm_mday,
                internal_clock.tm_hour,
                internal_clock.tm_min,
                internal_clock.tm_sec);
        LOG_INF("Uptime: %lld ms", msg->uptime_ms);
        LOG_INF("Status: Relógio sincronizado com SNTP");
        LOG_INF("================");
    } else {
        LOG_WRN("Recebida mensagem de tempo não sincronizada");
        LOG_WRN("Uptime: %lld ms", msg->uptime_ms);
    }
}

// Definir listener para o canal de tempo
ZBUS_LISTENER_DEFINE(logger_listener, time_msg_callback);

// Adicionar observer ao canal
ZBUS_CHAN_ADD_OBS(time_channel, logger_listener, 0);

/**
 * @brief Entry point da thread Logger
 */
void logger_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    LOG_INF("Thread Logger iniciada");
    LOG_INF("Aguardando sincronização de tempo...");

    // A thread pode fazer outras tarefas enquanto aguarda eventos ZBus
    // Os callbacks serão chamados automaticamente quando mensagens chegarem
    
    while (1) {
        if (clock_initialized) {
            // Pode fazer logging periódico ou outras operações
            // Por enquanto, apenas aguarda
        }
        
        // Sleep para reduzir uso de CPU
        k_sleep(K_SECONDS(10));
        
        if (clock_initialized) {
            LOG_DBG("Logger heartbeat - Clock: %02d:%02d:%02d",
                    internal_clock.tm_hour,
                    internal_clock.tm_min,
                    internal_clock.tm_sec);
        }
    }
}
