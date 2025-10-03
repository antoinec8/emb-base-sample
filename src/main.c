#include <zephyr/kernel.h>      // Funções básicas do Zephyr
#include <zephyr/logging/log.h> // Sistema de logging do Zephyr

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL); // Registra o módulo de log

// Define o timer
struct k_timer hello_timer;

// Callback do timer
void timer_handler(struct k_timer *timer)
{
    static uint32_t count = 0;
    count++;

    // Usando diferentes níveis de log
    // Log normal
    LOG_INF("Hello World! Count: %d", count);
    
    // Log de aviso quando o count é múltiplo de 5
    if (count % 5 == 0) {
        LOG_WRN("Reached multiple of 5 counts!");
    }

    // Log de erro quando o count é múltiplo de 10
    if (count % 10 == 0) {
        LOG_ERR("Reached multiple of 10 counts!");
    }

    LOG_DBG("Debug message - Timer fired!");
}

int main(void)
{
    LOG_INF("Initializing Hello World Timer Application");

    // Inicializa o timer
    k_timer_init(&hello_timer, timer_handler, NULL);

    // Inicia o timer com o intervalo definido no Kconfig (1 segundo)
    k_timer_start(&hello_timer, K_MSEC(CONFIG_HELLO_TIMER_INTERVAL), 
                  K_MSEC(CONFIG_HELLO_TIMER_INTERVAL));

    LOG_INF("Timer started with interval of %d ms", CONFIG_HELLO_TIMER_INTERVAL);

    // Loop infinito para manter o programa rodando
    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}