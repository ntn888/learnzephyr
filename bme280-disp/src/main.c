#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/rtio/rtio.h>
#include <zephyr/dsp/print_format.h>
#include <stdint.h>
#include <zephyr/display/cfb.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

LOG_MODULE_REGISTER(foo,LOG_LEVEL_DBG);

static const struct device *disp = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

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
	
	uint16_t x_res;
	uint16_t y_res;
	uint16_t rows;
	uint8_t ppt;
	
	if (disp == NULL) {
		LOG_ERR("display pointer is NULL");
		return 1;
	}

	if (!device_is_ready(disp)) {
		LOG_ERR("display device is not ready");
		return 1;
	}
        if (display_set_pixel_format(disp, PIXEL_FORMAT_MONO10) != 0) {
		if (display_set_pixel_format(disp, PIXEL_FORMAT_MONO01) != 0) {
			LOG_ERR("Failed to set required pixel format");
			return 0;
		}
	}

	//printf("Initialized %s\n", disp->name);

	if (cfb_framebuffer_init(disp)) {
		LOG_ERR("Framebuffer initialization failed!\n");
		return 0;
	}

	cfb_framebuffer_clear(disp, true);

	display_blanking_off(disp);

	x_res = cfb_get_display_parameter(disp, CFB_DISPLAY_WIDTH);
	y_res = cfb_get_display_parameter(disp, CFB_DISPLAY_HEIGH);
	rows = cfb_get_display_parameter(disp, CFB_DISPLAY_ROWS);
	ppt = cfb_get_display_parameter(disp, CFB_DISPLAY_PPT);
	
	
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

		char t[16];

		sprintf(t, "%s%d.%d\n", PRIq_arg(temp_data.readings[0].temperature, 6, temp_data.shift));
		if (cfb_print(disp, "Temp:", 0, y_res/3)) {
			LOG_ERR("Failed to print a string\n");
		}
		if (cfb_print(disp, (char* const)t, 0, y_res*2/3)) {
			LOG_ERR("Failed to print a string\n");
		}
		cfb_framebuffer_finalize(disp);
		
		LOG_INF("Hello Log!");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

