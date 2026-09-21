#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device* dev = DEVICE_DT_GET_ANY(our_driver);
    if (!device_is_ready(dev))
    {
        return -ENODEV;
    }

    enum sensor_channel dummy_sensor_channel = SENSOR_CHAN_ACCEL_X;
    struct sensor_value dummy_sensor_value = {};
    while (1) {
        sensor_sample_fetch(dev);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        sensor_channel_get(dev,
                           dummy_sensor_channel,
                           &dummy_sensor_value);
    }
    return 0;
}
