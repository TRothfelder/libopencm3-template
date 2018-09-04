#include "FreeRTOS.h"
#include "task.h"
#include "tasks/init.h"
#include "tasks/usb.h"
#include "tasks/ledblink.h"

#include "gfx.h"

#include <libopencm3/stm32/gpio.h>

xTaskHandle initTaskHandle = NULL;

void init_task(void *arg) {
	(void)arg;
	//unsigned istr;
	(void)arg;
	coord_t		width, height;
	coord_t		i, j;

	gfxInit();
	// Code Here
	// Get the screen size

	gdispSetOrientation(GDISP_ROTATE_270);
	width = gdispGetWidth();
	height = gdispGetHeight();

	gdispDrawBox(10, 10, width/2, height/2, Yellow);
	gdispFillArea(width/2, height/2, width/2-10, height/2-10, Blue);
	gdispDrawLine(5, 30, width-50, height-40, Red);
	for(i = 5, j = 0; i < width && j < height; i += 7, j += i/20)
			gdispDrawPixel(i, j, White);


	xTaskCreate(usb_task,"USB",100,NULL,configMAX_PRIORITIES-1, &usbTaskHandle);
	xTaskCreate(ledblink_task,"LEDBLINK",100,NULL,configMAX_PRIORITIES-1, &ledblinkTaskHandle);

	vTaskDelay(10000 / portTICK_RATE_MS);
	vTaskDelete( initTaskHandle );
	initTaskHandle = NULL;
}
