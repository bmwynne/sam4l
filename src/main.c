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
#include "cp_console.h"
#include "cp_usb.h"
#include "main.h"

int main(void)
{
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	board_init();
	delay_init();
	usb_init();
	run_console();
	//sleepmgr_init();

	puts(STRING_HEADER);
	printf("CPH Wireless Printer loaded; please select an application to run.\n\r");
	printf("1. Send a print job\n\r");
	printf("2. Console\n\r");
	printf("3. Modem Comms\n\r");
	
	while (1)
	{
		if (g_v_puc_linebuffer == 1) {
			if (main_usb_in) {
				main_usb_in = 0;
				memset(in_buffer, 0, sizeof(in_buffer));
				uhi_vendor_bulk_in_run(in_buffer, sizeof(in_buffer), print_bulk_in_cb);
			}
			if (ui_usb_dev_enum)
			{
				printf("-----LOG MAIN WHILE-----:\n\rUSB DEVICE ENUMERATED.\n\r");
				//get_num_conn_devices();
				print_bulk_out();
				ui_usb_dev_enum = 0;
			}
		}
		if (g_v_puc_linebuffer == 2)
		{
			printf("console\n\r");
		}
		if (g_v_puc_linebuffer == 3)
		{
			printf("Feature disabled\n\r");
		}
	}
}
