//#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200

LOG_MODULE_REGISTER(foo,LOG_LEVEL_DBG);

int main(void)
{
	while (1) {
		LOG_INF("Hello Log!");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

