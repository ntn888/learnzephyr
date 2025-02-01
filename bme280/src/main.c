//#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

LOG_MODULE_REGISTER(foo,LOG_LEVEL_DBG);

static const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bme280));

SENSOR_DT_READ_IODEV(iodev, DT_NODELABEL(bme280),
		{SENSOR_CHAN_AMBIENT_TEMP, 0},
		{SENSOR_CHAN_HUMIDITY, 0},
		{SENSOR_CHAN_PRESS, 0});

RTIO_DEFINE(ctx, 1, 1);

int main(void)
{
	if (dev == NULL) {
		/* No such node, or the node does not have status "okay". */
		LOG_ERR("\nError: no device found.\n");
		return -1;
	}

	if (!device_is_ready(dev)) {
		LOG_ERR("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		return -1;
	}

	printk("Found device \"%s\", getting sensor data\n", dev->name);
	
	while (1) {
		
		uint8_t buf[128];

		int rc = sensor_read(&iodev, &ctx, buf, 128);

		if (rc != 0) {
			LOG_ERR("%s: sensor_read() failed: %d\n", dev->name, rc);
			return rc;
		}
		
		///
		
		const struct sensor_decoder_api *decoder;

		rc = sensor_get_decoder(dev, &decoder);

		if (rc != 0) {
			LOG_ERR("%s: sensor_get_decode() failed: %d\n", dev->name, rc);
			return rc;
		}

		uint32_t temp_fit = 0;
		struct sensor_q31_data temp_data = {0};

		decoder->decode(buf,
			(struct sensor_chan_spec) {SENSOR_CHAN_AMBIENT_TEMP, 0},
			&temp_fit, 1, &temp_data);

		uint32_t press_fit = 0;
		struct sensor_q31_data press_data = {0};

		decoder->decode(buf,
				(struct sensor_chan_spec) {SENSOR_CHAN_PRESS, 0},
				&press_fit, 1, &press_data);

		uint32_t hum_fit = 0;
		struct sensor_q31_data hum_data = {0};

		decoder->decode(buf,
				(struct sensor_chan_spec) {SENSOR_CHAN_HUMIDITY, 0},
				&hum_fit, 1, &hum_data);

		LOG_INF("temp: %s%d.%d; press: %s%d.%d; humidity: %s%d.%d\n",
			PRIq_arg(temp_data.readings[0].temperature, 6, temp_data.shift),
			PRIq_arg(press_data.readings[0].pressure, 6, press_data.shift),
			PRIq_arg(hum_data.readings[0].humidity, 6, hum_data.shift));
		
		///
		
		LOG_INF("Hello Log!");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

