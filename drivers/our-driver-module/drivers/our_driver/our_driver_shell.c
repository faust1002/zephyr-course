#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "cmd_sensor_fetch called");

    if (argc != 2)
    {
        shell_error(sh, "usage sensor fetch <device name>");
        return -EFAULT;
    }

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
    shell_print(sh, "cmd_sensor_read called");

    if (argc != 2)
    {
        shell_error(sh, "usage sensor fetch <device name>");
        return -EFAULT;
    }

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
    shell_print(sh, "cmd_sensor_info called");

    if (argc != 2)
    {
        shell_error(sh, "usage sensor fetch <device name>");
        return -EFAULT;
    }

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

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcommands,
    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Call sensor_channel_get()", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Print device name and ready state", cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcommands, "Sensor commands", NULL);
