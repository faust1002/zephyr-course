#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

#define APP_LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec app_led = GPIO_DT_SPEC_GET(APP_LED_NODE, gpios);

LOG_MODULE_REGISTER(our_module, LOG_LEVEL_INF);

static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    LOG_INF("Hello from our_driver_sample_fetch, channel: %d", chan);
    gpio_pin_set_dt(&app_led, 0);
    return 0;
}

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    LOG_INF("Hello from our_driver_channel_get, channel: %d", chan);
    gpio_pin_set_dt(&app_led, 1);
    return 0;
}

static DEVICE_API(sensor, our_driver_api) = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get
};

static int our_driver_init(const struct device* dev)
{
    LOG_INF("Hello from our_driver_init");

    if (!gpio_is_ready_dt(&app_led)) {
        return -ENODEV;
    }
    if (gpio_pin_configure_dt(&app_led, GPIO_OUTPUT_ACTIVE) < 0) {
        return -ENODEV;
    }

    return 0;
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, NULL, NULL, POST_KERNEL, 80, &our_driver_api);
DT_INST_FOREACH_STATUS_OKAY(DEV_INST)
