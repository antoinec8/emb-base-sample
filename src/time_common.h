#ifndef TIME_COMMON_H
#define TIME_COMMON_H

#include <zephyr/kernel.h>
#include <time.h>
#include <zephyr/zbus/zbus.h>

/**
 * @brief Estrutura de mensagem de tempo para o ZBus
 * Contém informações de timestamp para sincronização
 */
struct time_msg {
    int64_t timestamp;       // Unix timestamp em segundos
    int64_t uptime_ms;       // System uptime em milissegundos
    bool is_synchronized;    // Se o tempo foi sincronizado com SNTP
    struct tm time_info;     // Informação de tempo formatada
};

/**
 * @brief Declaração do canal ZBus para mensagens de tempo
 * Este canal é usado para distribuir atualizações de tempo
 * do cliente SNTP para os módulos subscribers
 */
ZBUS_CHAN_DECLARE(time_channel);

/**
 * @brief Servidor SNTP padrão
 * Pode ser sobrescrito por Kconfig
 */
#ifndef CONFIG_SNTP_SERVER
#define SNTP_SERVER "pool.ntp.org"
#else
#define SNTP_SERVER CONFIG_SNTP_SERVER
#endif

/**
 * @brief Intervalo de sincronização SNTP (em segundos)
 */
#ifndef CONFIG_SNTP_SYNC_INTERVAL
#define SNTP_SYNC_INTERVAL 60
#else
#define SNTP_SYNC_INTERVAL CONFIG_SNTP_SYNC_INTERVAL
#endif

/**
 * @brief Tamanhos de stack das threads
 */
#define SNTP_STACK_SIZE 2048
#define LOGGER_STACK_SIZE 1024
#define APP_STACK_SIZE 1024

/**
 * @brief Prioridades das threads
 */
#define SNTP_PRIORITY 5
#define LOGGER_PRIORITY 6
#define APP_PRIORITY 7

#endif /* TIME_COMMON_H */
