#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
//#define SLEEP_TIME_MS   200

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(butn0), gpios);

int main(void)
{
	int ret;
	bool led_state = true;
	bool prv_state = led_state;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	if (!gpio_is_ready_dt(&btn)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		if (led_state = gpio_pin_get_dt(&btn)) {
			gpio_pin_set_dt(&led, 1);
		} else {
			gpio_pin_set_dt(&led, 0);
		}

		if (led_state != prv_state) {
			printf("Changed to state: %s", led_state ? "ON" : "OFF");
			prv_state = led_state;
		}

	}
	return 0;
}

