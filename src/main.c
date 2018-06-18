/**
 * \file
 *
 * \brief Main functions for USB host vendor example
 *
 * Copyright (C) 2012-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <string.h>
#include "main.h"
#include "cp_console.h"
#include "conf_usb_host.h"
#include "conf_uart_serial.h"
#include "conf_clock.h"
#include "conf_cph.h"

								

static volatile bool main_b_usb_dev = false; // this will indicate that the bulk out can be started.
/*
	Initialize the USB Host Controller service.
	The UHC service launches the USB Host Driver to enable hardware peripheral.
	The UHD is responsible for sensing bus level differential and hardware interrupt 
*/

void usb_init(void) {
	uhc_start();
}

void main_usb_sof_event(void) {
	ui_usb_sof_event();
}


void main_usb_vendor_change(uhc_device_t * dev, bool b_present) {
	UNUSED(dev);
	main_b_usb_dev = b_present;
}

#define PRINT_JOB "^XA^XFR:STOREFMT.ZPL^FS^FO50,50^ADN,36,20^FDCPH^FS^XZ"
#define STATUS_JOB "~HQES"
#define PRINT_JOB_SIZE 56
#define STATUS_JOB_SIZE 5
#define SERIAL_SIZE 50
uint8_t out_buffer[PRINT_JOB_SIZE + 1] = PRINT_JOB;
uint8_t out_status_buffer[STATUS_JOB_SIZE + 1] = STATUS_JOB;
uint8_t in_buffer[SERIAL_SIZE + 1];
volatile int main_usb_in = 0;

void print_bulk_in_cb(usb_add_t add, usb_ep_t ep, uhd_trans_status_t status, iram_size_t nb_transferred) {
	if (status != UHD_TRANS_NOERROR) {
		return;
	}
	printf("-----LOG PRINT_BULK_IN_CB()-----:\n\rRECEIVED DATA FROM PRINTER.\n\r");
	printf("%s\n\r", in_buffer);
	main_usb_in = 1;
}

void print_bulk_out(void) {
	if(ui_usb_dev_dsc == 0) {
		return;
	}
	printf("-----LOG PRINT_BULK_OUT()-----\n\rSending print job to ZT230.\n\r");
	uhi_vendor_control_out_run(out_buffer, PRINT_JOB_SIZE, NULL);
	
	if (uhi_vendor_bulk_is_available()) {
		uhi_vendor_bulk_out_run(out_buffer, PRINT_JOB_SIZE, NULL);
		//delay_s(2);
		delay_ms(30);
		uhi_vendor_bulk_out_run(out_status_buffer, STATUS_JOB_SIZE, NULL);
	}
	main_usb_in = 1;
	//uhi_vendor_bulk_in_run(in_buffer, sizeof(in_buffer), print_bulk_in_cb);
}

void get_num_conn_devices(void) {
	uint8_t b_num_devices;
	b_num_devices = uhc_get_device_number();
	printf("-----LOG GET_NUM_CONN_DEVICES()-----:\n\rNumber of devices connected: %d.\n\r", b_num_devices);
}


/* brief Main function. Execution starts here. */
int main(void) {
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	board_init();
	delay_init();
	usb_init();
	//sleepmgr_init();
	pdca_config_enable();
	configure_console();
	configure_tc();
	
	usart_enable_interrupt(BOARD_USART, US_IER_RXBUFF);	/* Enable USART RXBUFF interrupt */
	NVIC_EnableIRQ(USART_IRQn);	/* Configure and enable interrupt of USART. */

	tc_start(TC0, 0);	/* Start timer. */
	
	puts(STRING_HEADER);
	
	printf("-----LOG MAIN-----:\n\rBoard Initialized\n\r");

	printf("-----LOG MAIN-----:\n\rEntering While().\n\r");
	
	while(1) {
		if (main_usb_in) {
			main_usb_in = 0;
			memset(in_buffer,0,sizeof(in_buffer));
			uhi_vendor_bulk_in_run(in_buffer, sizeof(in_buffer), print_bulk_in_cb);
		}
		if (ui_usb_dev_enum) {
			printf("-----LOG MAIN WHILE-----:\n\rUSB DEVICE ENUMERATED.\n\r");	
			//get_num_conn_devices();
			print_bulk_out();
			ui_usb_dev_enum = 0;
		}
	}
}



