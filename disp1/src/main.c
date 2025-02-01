//#include <stdio.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

#define DISPLAY_BUFFER_PITCH 128

LOG_MODULE_REGISTER(display);

static const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200



int main(void)
{

	//lv_obj_t *hello_world_label;

	if (display == NULL) {
		LOG_ERR("device pointer is NULL");
		return 1;
	}

	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		return 1;
	}

	struct display_capabilities capabilities;
	display_get_capabilities(display, &capabilities);

	const uint16_t x_res = capabilities.x_resolution;
	const uint16_t y_res = capabilities.y_resolution;

	/*
	const struct display_buffer_descriptor buf_desc = {
		.width = x_res,
		.height = y_res,
		.buf_size = x_res * y_res,
		.pitch = DISPLAY_BUFFER_PITCH
	};

	if (display_write(display, 0, 0, &buf_desc, buf) != 0) {
		LOG_ERR("could not write to display");
	}

	if (display_set_contrast(display, 255) != 0) {
		LOG_ERR("could not set display contrast");
	}
	*/
	//hello_world_label = lv_label_create(lv_screen_active());
	//lv_label_set_text(hello_world_label, "Hello world!");
	//lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	//count_label = lv_label_create(lv_screen_active());
	//lv_obj_align(count_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	//lv_timer_handler();
	//display_blanking_off(display);


	while (1) {
		//LOG_INF("x_resolution: %d", x_res);
		//LOG_INF("y_resolution: %d", y_res);
		//LOG_INF("supported pixel formats: %d", capabilities.supported_pixel_formats);
		//LOG_INF("screen_info: %d", capabilities.screen_info);
		//LOG_INF("current_pixel_format: %d", capabilities.current_pixel_format);
		//LOG_INF("current_orientation: %d", capabilities.current_orientation);
		//lv_timer_handler();
		lv_timer_periodic_handler();
		//k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

