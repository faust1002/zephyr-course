#include <zephyr/init.h>
#include <zephyr/kernel.h>

void board_early_init_hook(void)
{
    printk("Board Initialized\n");
}
