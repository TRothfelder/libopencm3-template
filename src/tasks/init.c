#include "FreeRTOS.h"
#include "task.h"
#include "tasks/init.h"
#include "tasks/usb.h"
#include "tasks/ledblink.h"

#include <libopencm3/stm32/gpio.h>

xTaskHandle initTaskHandle = NULL;

void init_task(void *arg) {
	(void)arg;

	xTaskCreate(usb_task,"USB",100,NULL,configMAX_PRIORITIES-1, &usbTaskHandle);
	xTaskCreate(ledblink_task,"LEDBLINK",100,NULL,configMAX_PRIORITIES-1, &ledblinkTaskHandle);

	vTaskDelay(10000 / portTICK_RATE_MS);
	gpio_toggle(GPIOD,GPIO14);
	vTaskDelete( initTaskHandle );
	initTaskHandle = NULL;
}
