//#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   20
#define STEPS 100

/* The devicetree node identifier for the "led0" alias. */
#define FADING_LED DT_ALIAS(pwm_led1)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct pwm_dt_spec fading_led = PWM_DT_SPEC_GET(FADING_LED);

int main(void)
{
	int ret;
	//bool led_state = true;
	int state = 0, delta = fading_led.period / STEPS;

	if (!device_is_ready(fading_led.dev)) {
		return 0;
	}

	while (1) {

		for (int i = 0; i < STEPS; i++) {
			//
			ret = pwm_set_pulse_dt(&fading_led, state);
			state += delta;
			k_msleep(SLEEP_TIME_MS);
		}
		//state = 0;
		for (int i = 0; i < STEPS; i++) {
			//
			ret = pwm_set_pulse_dt(&fading_led, state);
			state -= delta;
			k_msleep(SLEEP_TIME_MS);
		}
	}
	return 0;
}

