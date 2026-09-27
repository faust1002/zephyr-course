#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);

    shell_print(sh, "cmd_sensor_fetch called");

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret != 0)
    {
        shell_error(sh, "Could not fetch channel, got %d", ret);
        return -EFAULT;
    }

    shell_info(sh, "Fetching channel was successful");

    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &val);
    if (ret != 0)
    {
        shell_error(sh, "Could not fetch channel, got %d", ret);
        return -EFAULT;
    }

    shell_info(sh, "sensor->val1 = %d, sensor->val2 = %d", val.val1, val.val2);
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);

    shell_print(sh, "cmd_sensor_info called");

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    bool device_ready = device_is_ready(dev);

    shell_info(sh, "Device name = %s, ready = %d", dev->name, device_ready);

    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);

    shell_print(sh, "cmd_sensor_set called");

    int error = 0;
    long value = shell_strtol(argv[2], 10, &error);
    if (error != 0)
    {
        shell_error(sh, "Invalid integer value = %s, (error = %d)", argv[2], error);
        return -EFAULT;
    }

    const int min_value = 0x37;
    const int max_value = 0x42;
    if ((value < min_value) || (value > max_value))
    {
        shell_error(sh, "Value out of range (min: %d, max: %d), actual value = %ld", min_value, max_value, value);
        return -EFAULT;
    }

    int actual_value = (int)value;

    const struct device *dev = shell_device_get_binding(argv[1]);
    if (!dev)
    {
        shell_error(sh, "Could not find device %s", argv[1]);
        return -EFAULT;
    }

    our_driver_set(dev, actual_value);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcommands,
    SHELL_CMD_ARG(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch, 2, 0),
    SHELL_CMD_ARG(read, NULL, "Call sensor_channel_get()", cmd_sensor_read, 2, 0),
    SHELL_CMD_ARG(info, NULL, "Print device name and ready state", cmd_sensor_info, 2, 0),
    SHELL_CMD_ARG(set, NULL, "Set new counter value", cmd_sensor_set, 3, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcommands, "Sensor commands", NULL);
