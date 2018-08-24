/*
 * usb.h
 *
 *  Created on: 23.05.2018
 *      Author: kt
 */

#ifndef INC_TASKS_USB_H_
#define INC_TASKS_USB_H_

#include "FreeRTOS.h"
#include "task.h"

extern xTaskHandle usbTaskHandle;
void usb_task(void *arg);

#endif /* INC_TASKS_USB_H_ */
