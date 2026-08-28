#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifier for the "app-led" alias. */
#define APP_LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec app_led = GPIO_DT_SPEC_GET(APP_LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool app_led_state = true;

    if (!gpio_is_ready_dt(&app_led)) return 0;

    if (gpio_pin_configure_dt(&app_led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&app_led) < 0) return 0;

        app_led_state = !app_led_state;
        LOG_INF("App LED state: %s", app_led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
