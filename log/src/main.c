//#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include "myfunction.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   200

LOG_MODULE_REGISTER(foo,LOG_LEVEL_DBG);

int main(void)
{
	int a = 3, b = 4;


	while (1) {

		//printf("Hello world!\n");
		//printf("Sum of %d and %d is %d\n", a, b, sum(a,b));
		LOG_INF("Hello Log!");
		LOG_DBG("Sum of %d and %d is %d\n", a, b, sum(a,b));
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

