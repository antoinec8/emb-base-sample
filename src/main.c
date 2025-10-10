#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define LED_NODE DT_ALIAS(led0)
#define SW_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec sw = GPIO_DT_SPEC_GET(SW_NODE, gpios);

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static struct gpio_callback button_cb;
static bool toggle_mode = false;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    toggle_mode = !toggle_mode;
    printk("Mode changed to: %s\n", toggle_mode ? "PWM" : "Digital");
}

void main(void)
{
    int ret;

    if (!gpio_is_ready_dt(&led)) {
        printk("Error: LED device not ready!\n");
        return;
    }

    if (!gpio_is_ready_dt(&sw)) {
        printk("Error: Button device not ready!\n");
        return;
    }

    // Configura LED
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error: LED pin configuration failed!\n");
        return;
    }

    // Configura botão
    ret = gpio_pin_configure_dt(&sw, GPIO_INPUT);
    if (ret < 0) {
        printk("Error: Button pin configuration failed!\n");
        return;
    }

    // Configura callback do botão
    ret = gpio_pin_interrupt_configure_dt(&sw, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        printk("Error: Button interrupt configuration failed!\n");
        return;
    }

    gpio_init_callback(&button_cb, button_pressed, BIT(sw.pin));
    gpio_add_callback(sw.port, &button_cb);

    printk("Starting LED control demo\n");

    while (1) {
        gpio_pin_toggle_dt(&led);
        if (toggle_mode) {
            k_msleep(100);  // Modo rápido
        } else {
            k_msleep(500);  // Modo normal
        }
    }
}
