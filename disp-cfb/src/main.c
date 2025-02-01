#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
//#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(display);

static const struct device *disp = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200



int main(void)
{

	uint16_t x_res;
	uint16_t y_res;
	uint16_t rows;
	uint8_t ppt;
	uint8_t font_width;
	uint8_t font_height;


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

	/*
	for (int idx = 0; idx < 42; idx++) {
		if (cfb_get_font_size(disp, idx, &font_width, &font_height)) {
			break;
		}
		cfb_framebuffer_set_font(disp, idx);
		LOG_INF("font width %d, font height %d\n",
		       font_width, font_height);
	}
	*/

	LOG_INF("x_res %d, y_res %d, ppt %d, rows %d, cols %d\n",
	       x_res,
	       y_res,
	       ppt,
	       rows,
	       cfb_get_display_parameter(disp, CFB_DISPLAY_COLS));

	//cfb_framebuffer_invert(disp);

	//cfb_set_kerning(disp, 3);
	
	if (cfb_print(disp, "Hello World!", 0, y_res/3)) {
	    LOG_ERR("Failed to print a string\n");
	}
	if (cfb_print(disp, "Hello again!", 0, y_res*2/3)) {
	    LOG_ERR("Failed to print a string\n");
	}
	cfb_framebuffer_finalize(disp);
	
	while (1) {
		//k_msleep(SLEEP_TIME_MS);
		/*
		for (int i = 0; i < MIN(x_res, y_res); i++) {
			cfb_framebuffer_clear(disp, false);
			if (cfb_print(disp,
				      "0123456789mMgj!\"§$%&/()=",
				      i, i)) {
				LOG_ERR("Failed to print a string\n");
				continue;
			}

			cfb_framebuffer_finalize(disp);

			k_sleep(K_MSEC(20));
		}
		*/
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

