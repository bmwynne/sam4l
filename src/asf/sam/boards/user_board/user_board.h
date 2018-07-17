/**
 * \file
 *
 * \brief SAM4L8 Xplained Pro board definition
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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

#ifndef USER_BOARD_INCLUDED 
#define USER_BOARD_INCLUDED 

#include <conf_board.h>

/**
 * \ingroup group_common_boards
 * \defgroup sam4l8_xplained_pro_group SAM4L8 Xplained Pro board
 *
 * @{
 */

/**
 * \defgroup sam4l8_xplained_pro_config_group Configuration
 *
 * Symbols to use for configuring the board and its initialization.
 *
 * @{
 */
#ifdef __DOXYGEN__

//! \name Initialization
//@{

/**
 * \def CONF_BOARD_KEEP_WATCHDOG_AT_INIT
 * \brief Let watchdog remain enabled
 *
 * If this symbol is defined, the watchdog is left running with its current
 * configuration. Otherwise, it gets disabled during board initialization.
 */
# ifndef CONF_BOARD_KEEP_WATCHDOG_AT_INIT
#  define CONF_BOARD_KEEP_WATCHDOG_AT_INIT
# endif

//@}

#endif // __DOXYGEN__
/** @} */

//! Name string macro
#define BOARD_NAME          "CPHT Printer Gateway 1.0"
#define MCU_SOC_NAME        "ATSAM4LC2A"

//! \name Board oscillator definitions
//@{
//! Osc frequency (Hz.) and startup time (RCOsc periods)
#define FOSC0                     (25000000UL)
//! Osc32 frequency (Hz.) and startup time (RCOsc periods)
#define FOSC32                    (32768UL)
#define BOARD_OSC32_IS_XTAL       true
#define BOARD_OSC32_HZ            FOSC32
#define BOARD_OSC32_STARTUP_US    (750000UL)
#define BOARD_OSC32_SELCURR       BSCIF_OSCCTRL32_SELCURR(10)
#define BOARD_OSC0_IS_XTAL        true
#define BOARD_OSC0_HZ             FOSC0
#define BOARD_OSC0_STARTUP_US     (6100UL)
//@}

//! \name LED0 definitions
//@{
#define LED0                      PIN_PA04
#define LED0_PIN                  PIN_PA04 /* Wrapper definition */
#define LED0_GPIO                 PIN_PA04 /* Wrapper definition */
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
//@}

/**
 * \name USB pin definitions
 *
 * These are named according to the USBC driver's requirements.
 */
//@{
//#define USB_ID_PIN                PIN_PC10
//#define USB_ID_FLAGS              (IOPORT_MODE_PULLUP | IOPORT_MODE_GLITCH_FILTER)
//#define USB_VBUS_PIN              PIN_PC11
//#define USB_VBUS_FLAGS            IOPORT_MODE_GLITCH_FILTER
#define USB_VBOF_PIN              PIN_PA09
#define USB_VBOF_ACTIVE_LEVEL     1
#define USB_VBOF_INACTIVE_LEVEL   0
//@}

//! \ Modem USART
// @{
// #define COM_PORT_USART            USART0
// #define COM_PORT_USART_ID         ID_USART0
// #define COM_PORT_RX_PIN           PIN_PA05B_USART0_RXD
// #define COM_PORT_RX_GPIO          GPIO_PA05B_USART0_RXD
// #define COM_PORT_RX_MUX           MUX_PA05B_USART0_RXD
// #define COM_PORT_TX_PIN           PIN_PA07B_USART0_TXD
// #define COM_PORT_TX_GPIO          GPIO_PA07B_USART0_TXD 
// #define COM_PORT_TX_MUX           MUX_PA07B_USART0_TXD
// @}

// MCU USART 
#define COM_PORT_USART            USART2
#define COM_PORT_USART_ID         ID_USART2
#define COM_PORT_RX_PIN           PIN_PA19A_USART2_RXD
#define COM_PORT_RX_GPIO          GPIO_PA19A_USART2_RXD
#define COM_PORT_RX_MUX           MUX_PA19A_USART2_RXD
#define COM_PORT_TX_PIN           PIN_PA20A_USART2_TXD
#define COM_PORT_TX_GPIO          GPIO_PA20A_USART2_TXD
#define COM_PORT_TX_MUX           MUX_PA20A_USART2_TXD

/**
 * \name LED #0 definitions
 *
 * Wrapper macros for LED0, to ensure common naming across all Xplained Pro
 * boards.
 */
//@{
#define LED_0_NAME                "LED0"
#define LED_0_PIN                 LED0_PIN
#define LED_0_ACTIVE              LED0_ACTIVE
#define LED_0_INACTIVE            LED0_INACTIVE
//@}

//! Number of on-board LEDs
#define LED_COUNT                 1

#endif /* USER_BOARD_INCLUDED */
