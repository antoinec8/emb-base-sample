#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include "time_common.h"
#include "sntp_client.h"

LOG_MODULE_REGISTER(sntp_client, LOG_LEVEL_DBG);

// Definição da thread e stack
K_THREAD_STACK_DEFINE(sntp_stack_area, SNTP_STACK_SIZE);
struct k_thread sntp_thread;

// Definição do canal ZBus para mensagens de tempo
ZBUS_CHAN_DEFINE(time_channel,
                 struct time_msg,
                 NULL,
                 NULL,
                 ZBUS_OBSERVERS_EMPTY,
                 ZBUS_MSG_INIT(0));

/**
 * @brief Simula sincronização SNTP e atualiza o relógio do sistema
 * 
 * @return 0 se sucesso, negativo em caso de erro
 */
static int sync_sntp_time(void)
{
    // Simular timestamp SNTP (2025-11-02 14:30:00 UTC)
    time_t simulated_time = 1730558400 + (k_uptime_get() / 1000);
    
    LOG_INF("Simulando sincronização SNTP com %s", SNTP_SERVER);

    // Atualizar relógio do sistema
    struct timespec ts;
    ts.tv_sec = simulated_time;
    ts.tv_nsec = 0;

    int ret = clock_settime(CLOCK_REALTIME, &ts);
    if (ret < 0) {
        LOG_ERR("Falha ao configurar relógio: %d", errno);
        return -errno;
    }

    LOG_INF("Sincronização SNTP bem-sucedida: %lld.%09ld",
            (long long)ts.tv_sec, ts.tv_nsec);

    return 0;
}

/**
 * @brief Entry point da thread SNTP Client
 */
void sntp_client_entry(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    struct time_msg msg;
    int ret;

    LOG_INF("Thread SNTP Client iniciada");
    LOG_INF("NOTA: Usando simulação de SNTP (sem rede real)");

    // Aguardar um pouco para o sistema estar pronto
    k_sleep(K_SECONDS(2));

    while (1) {
        // Simular sincronização com servidor SNTP
        ret = sync_sntp_time();
        
        if (ret == 0) {
            // Sincronização bem-sucedida, preparar mensagem
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            
            msg.timestamp = ts.tv_sec;
            msg.uptime_ms = k_uptime_get();
            msg.is_synchronized = true;
            
            // Converter para struct tm
            gmtime_r(&ts.tv_sec, &msg.time_info);
            
            // Publicar no canal ZBus
            ret = zbus_chan_pub(&time_channel, &msg, K_SECONDS(1));
            if (ret < 0) {
                LOG_ERR("Falha ao publicar no ZBus: %d", ret);
            } else {
                LOG_INF("Horário publicado: %04d-%02d-%02d %02d:%02d:%02d UTC",
                        msg.time_info.tm_year + 1900,
                        msg.time_info.tm_mon + 1,
                        msg.time_info.tm_mday,
                        msg.time_info.tm_hour,
                        msg.time_info.tm_min,
                        msg.time_info.tm_sec);
            }
        } else {
            LOG_WRN("Sincronização SNTP falhou, tentando novamente em breve");
            
            // Publicar mensagem indicando falha na sincronização
            msg.timestamp = 0;
            msg.uptime_ms = k_uptime_get();
            msg.is_synchronized = false;
            memset(&msg.time_info, 0, sizeof(struct tm));
        }

        // Aguardar intervalo antes da próxima sincronização
        k_sleep(K_SECONDS(SNTP_SYNC_INTERVAL));
    }
}
