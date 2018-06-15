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
#include "conf_usb_host.h"
#include "conf_uart_serial.h"
#include "conf_clock.h"
#include "conf_cph.h"

								
#define BUFFER_SIZE         100				/** Size of the receive buffer used by the PDCA, in bytes. */
#define MAX_BUF_NUM         1				/** Max buffer number. */
#define ALL_INTERRUPT_MASK  0xffffffff		/** All interrupt mask. */
#define PDCA_RX_CHANNEL  0
#define PDCA_TX_CHANNEL  1


/** Receive buffer. */
static uint8_t gs_puc_buffer[2][BUFFER_SIZE];

/** Next Receive buffer. */
static uint8_t gs_puc_nextbuffer[2][BUFFER_SIZE];

/** Current bytes in buffer. */
static uint32_t gs_ul_size_buffer = BUFFER_SIZE;

/** Current bytes in next buffer. */
static uint32_t gs_ul_size_nextbuffer = BUFFER_SIZE;

/** Buffer number in use. */
static uint8_t gs_uc_buf_num = 0;

/** Flag of one transfer end. */
static uint8_t g_uc_transend_flag = 0;

pdca_channel_config_t pdca_rx_options = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_USART_RX, /* select peripheral - USART0 RX line.*/
	.size = BUFFER_SIZE, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = BUFFER_SIZE, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE, /* select size of the transfer */
	.etrig = false, /* disable event trigger*/
	.ring = false /* not use ring buffer*/
};
pdca_channel_config_t pdca_tx_options = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_USART_TX, /* select peripheral - USART0 TX line.*/
	.size = 0, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = 0, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE, /* select size of the transfer */
	.etrig = false, /* disable event trigger*/
	.ring = false /* not use ring buffer*/
};

/** Timer counter frequency in Hz. */
#define TC_FREQ             10

/**
 * \brief Configure Timer Counter 0 (TC0) to generate an interrupt every 200ms.
 * This interrupt will be used to flush USART input and echo back.
 */
static void configure_tc(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	static uint32_t ul_pbaclk;

	/* Configure clock service. */
	sysclk_enable_peripheral_clock(TC0);

	/* Get system clock. */
	ul_pbaclk = sysclk_get_peripheral_bus_hz(TC0);

	/* Configure TC for a 1Hz frequency and trigger on RC compare. */
	tc_find_mck_divisor(TC_FREQ, ul_pbaclk, &ul_div, &ul_tcclks, ul_pbaclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, (ul_pbaclk / ul_div) / TC_FREQ);

	/* Configure and enable interrupt on RC compare. */
	NVIC_EnableIRQ(TC00_IRQn);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
}

/**
 * \brief Interrupt handler for TC00. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
void TC00_Handler(void)
{
	uint32_t ul_status;
	uint32_t ul_byte_total = 0;

	/* Read TC0 Status. */
	ul_status = tc_get_status(TC0, 0);

	/* RC compare. */
	if ((ul_status & TC_SR_CPCS) == TC_SR_CPCS) {
		/* Flush PDC buffer. */
		ul_byte_total = BUFFER_SIZE -
				pdca_channel_read_load_size(PDCA_RX_CHANNEL);
		if ((ul_byte_total != 0) && (ul_byte_total != BUFFER_SIZE)) {
			/* Log current size. */
			g_uc_transend_flag = 1;
			if (pdca_channel_read_reload_size(PDCA_RX_CHANNEL) == 0) {
				gs_ul_size_buffer = BUFFER_SIZE;
				gs_ul_size_nextbuffer = ul_byte_total;
			} else {
				gs_ul_size_buffer = ul_byte_total;
				gs_ul_size_nextbuffer = 0;
			}

			/* Trigger USART Receive Buffer Full Interrupt. */
			/* Restart read on buffer. */
			pdca_channel_write_reload(PDCA_RX_CHANNEL,
					(void *)gs_puc_nextbuffer[gs_uc_buf_num], 0);
			pdca_channel_write_load(PDCA_RX_CHANNEL,
					(void *)gs_puc_buffer[gs_uc_buf_num], 0);
		}
	}
}

#define STRING_EOL    "\r"
#define STRING_HEADER "-- CPH Cellular USB Printer Online --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL	\
"-- Repo: github.com/bmwynne/sam4l"

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
		#endif
		.paritytype = CONF_UART_PARITY,
		#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
		#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Interrupt handler for USART. Echo the bytes received and start the
 * next receive.
 */
void USART_Handler(void)
{
	uint32_t ul_status;

	/* Read USART Status. */
	ul_status = usart_get_status(BOARD_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		/* Disable timer. */
		tc_stop(TC0, 0);

		/* Echo back buffer. */
		pdca_channel_write_load(PDCA_TX_CHANNEL,
				(void *)gs_puc_buffer[gs_uc_buf_num],
				gs_ul_size_buffer);
		pdca_channel_write_reload(PDCA_TX_CHANNEL,
				(void *)gs_puc_nextbuffer[gs_uc_buf_num],
				gs_ul_size_nextbuffer);

		if (g_uc_transend_flag) {
			gs_ul_size_buffer = BUFFER_SIZE;
			gs_ul_size_nextbuffer = BUFFER_SIZE;
			g_uc_transend_flag = 0;
		}

		gs_uc_buf_num = MAX_BUF_NUM - gs_uc_buf_num;

		/* Restart read on buffer. */
		pdca_channel_write_load(PDCA_RX_CHANNEL,
				(void *)gs_puc_buffer[gs_uc_buf_num], BUFFER_SIZE);
		pdca_channel_write_reload(PDCA_RX_CHANNEL,
				(void *)gs_puc_nextbuffer[gs_uc_buf_num], BUFFER_SIZE);

		/* Restart timer. */
		tc_start(TC0, 0);
	}
}



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
	
	configure_console();
	configure_tc();

	pdca_enable(PDCA); /* Enable PDCA module clock */
	pdca_channel_set_config(PDCA_RX_CHANNEL, &pdca_rx_options);	/* Init PDCA channel with the pdca_options.*/
	pdca_channel_set_config(PDCA_TX_CHANNEL, &pdca_tx_options);	/* Init PDCA channel with the pdca_options.*/
	pdca_channel_enable(PDCA_RX_CHANNEL);	/* Enable PDCA channel, start receiving data. */
	pdca_channel_enable(PDCA_TX_CHANNEL);	/* Enable PDCA channel, start receiving data. */
	
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



