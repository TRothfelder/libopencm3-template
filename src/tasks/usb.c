/*
 * usb.c
 *
 *  Created on: 23.05.2018
 *      Author: kt
 */
#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/usbstd.h>

#include "tasks/usb.h"
#include <ctype.h>

xTaskHandle usbTaskHandle = NULL;

static usbd_device *udev = NULL; // USB Device

void led(int on);

void led(int on) {
	if ( on )
		gpio_set(GPIOD,GPIO12);
	else
		gpio_clear(GPIOD,GPIO12);
}


static const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_VENDOR,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = 0x16c0,		// V-USB
	.idProduct = 0x05dc,			// V-USB Vendor Class + libusb PID
	.bcdDevice = 0x0200,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

static const struct usb_endpoint_descriptor data_endp[] = {
	{
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = 0x02,		/* From Host */
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,				/* Ignored for bulk */
	}, {
		.bLength = USB_DT_ENDPOINT_SIZE,
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = 0x83,		/* To Host */
		.bmAttributes = USB_ENDPOINT_ATTR_BULK,
		.wMaxPacketSize = 64,
		.bInterval = 0,				/* Ignored for bulk */
	}
};

static const struct usb_interface_descriptor data_iface[] = {
	{
		.bLength = USB_DT_INTERFACE_SIZE,
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = 0xFF,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface = 0,

		.endpoint = data_endp,
	}
};

static const struct usb_interface ifaces[] = {
	{
		.num_altsetting = 1,
		.altsetting = data_iface,
	}
};

static const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0x32,
	.interface = ifaces,
};

static const char * usb_strings[] = {
	"custom manufacturer",
	"custom product",
	"custom serial",
};

/*
 * Control Requests:
 */
static uint8_t usbd_control_buffer[128];

static enum usbd_request_return_codes
custom_control_request(
  usbd_device *usbd_dev,
  struct usb_setup_data *req,
  uint8_t **buf,
  uint16_t *len,
  void (**complete)(usbd_device *usbd_dev,struct usb_setup_data *req)
) {
	(void)complete;
	(void)buf;
	(void)usbd_dev;
	(void)len;

	switch ( req->bRequest ) {
	case USB_REQ_GET_STATUS:
		return USBD_REQ_HANDLED;

	case USB_REQ_SET_FEATURE:
		led(req->wValue&1);	// Set/reset LED
		return USBD_REQ_HANDLED;
	default:
		;
	}

	return USBD_REQ_NOTSUPP;
}

/*
 * Bulk receive callback:
 */
static void
bulk_rx_cb(usbd_device *usbd_dev, uint8_t ep) {
	char buf[64];			// rx buffer & ptr
	int len, x;				// Received len..

	// Read what we can, leave rest:
	len = usbd_ep_read_packet(usbd_dev,ep,buf,sizeof buf);

	// Invert case of message received:
	for ( x=0; x<len; ++x ) {
		if ( isalpha(buf[x]) )
			buf[x] ^= 0x20;		// Invert case
	}

	if (len) {
		usbd_ep_write_packet(usbd_dev, 0x83, buf, len);
		buf[len] = 0;
  }
}

/*
 * Configure:
 */
static void
set_config(usbd_device *usbd_dev, uint16_t wValue) {
	(void)wValue;

	usbd_ep_setup(usbd_dev,0x02,USB_ENDPOINT_ATTR_BULK,64,bulk_rx_cb);
	usbd_ep_setup(usbd_dev,0x83,USB_ENDPOINT_ATTR_BULK,64,NULL);
	usbd_register_control_callback(
		usbd_dev,
		USB_REQ_TYPE_VENDOR,
		USB_REQ_TYPE_TYPE,
		custom_control_request);
}

/*
 * Poll any USB device events:
 */
void usb_task(void *arg) {
	//unsigned istr;
	(void)arg;

	udev = usbd_init(&otgfs_usb_driver,&dev,&config,
		usb_strings,3,
		usbd_control_buffer,sizeof(usbd_control_buffer));

	usbd_register_set_config_callback(udev,set_config);

	for (;;) {
		//istr = *USB_ISTR_REG;		// Capture USB interrupt flags
		usbd_poll(udev);		// Handle interrupt flags
		//if ( *USB_ISTR_REG == istr )	// No change?
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
