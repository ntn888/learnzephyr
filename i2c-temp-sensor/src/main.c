#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200

LOG_MODULE_REGISTER(foo,LOG_LEVEL_DBG);

#define I2C_NODE DT_NODELABEL(mysensor)

static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C_NODE);

#define CTRLMEAS 0xF4
#define CALIB00	 0x88
#define ID	     0xD0
#define TEMPMSB	 0xFA
#define CHIP_ID  0x60

int main(void)
{
	if (!device_is_ready(dev_i2c.bus)) {
		printk("I2C bus %s is not ready!\n\r",dev_i2c.bus->name);
		return -1;
	}

	uint8_t id = 0;
	uint8_t regs[] = {ID};

	int ret = i2c_write_read_dt(&dev_i2c, regs, 1, &id, 1);

	if (ret != 0) {
		printk("Failed to read register %x \n", regs[0]);
		return -1;
	}

	if (id != CHIP_ID) {
		printk("Invalid chip id! %x \n", id);
		return -1;
	}

	while (1) {
		LOG_INF("Hello Log!");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

