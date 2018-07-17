/*
 * CFile1.c
 *
 * Created: 6/7/2018 10:43:22 AM
 *  Author: Brandon
 */ 

/**
 * \file
 *
 * \brief USART serial example configuration.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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

#ifndef CONF_CPH_H_INCLUDED
#define CONF_CPH_H_INCLUDED

#include "conf_board.h"
#include "conf_clock.h"

#define BOARD_ID_USART             USART2
#define BOARD_USART                USART2
#define BOARD_USART_BAUDRATE       9600
#define USART_Handler              USART2_Handler
#define USART_IRQn                 USART2_IRQn

/*
The following two defines are DMA Peripheral Identity Values this is in the Sam4l8 data sheet under 16. Peripheral DMA 16.7 Module Configuration
USART0:
    RX: PID0
    TX: PID18
USART1:
    RX: PID1
    TX: PID19
USART2:
    RX: PID2
    TX: PID20
*/
#define PDCA_PID_USART_RX          2    
#define PDCA_PID_USART_TX          20


#endif /* CONF_CPH_H_INCLUDED */