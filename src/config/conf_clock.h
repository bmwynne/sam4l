/**
 * \file
 *
 * \brief Chip-specific system clock manager configuration
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
#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

//#define CONFIG_SYSCLK_INIT_CPUMASK  (1 << SYSCLK_OCD)
//#define CONFIG_SYSCLK_INIT_PBAMASK  (1 << SYSCLK_IISC)
//#define CONFIG_SYSCLK_INIT_PBBMASK  (1 << SYSCLK_USBC_REGS)
//#define CONFIG_SYSCLK_INIT_PBCMASK  (1 << SYSCLK_CHIPID)
//#define CONFIG_SYSCLK_INIT_PBDMASK  (1 << SYSCLK_AST)
//#define CONFIG_SYSCLK_INIT_HSBMASK  (1 << SYSCLK_PDCA_HSB)

//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_RCSYS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_OSC0
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL0
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_DFLL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_RC80M
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_RCFAST
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_RC1M

/* RCFAST frequency selection: 0 for 4MHz, 1 for 8MHz and 2 for 12MHz */
//#define CONFIG_RCFAST_FRANGE    0
//#define CONFIG_RCFAST_FRANGE    1
//#define CONFIG_RCFAST_FRANGE    2

/* Fbus = Fsys / (2 ^ BUS_div) */
#define CONFIG_SYSCLK_CPU_DIV         0
#define CONFIG_SYSCLK_PBA_DIV         0
#define CONFIG_SYSCLK_PBB_DIV         0
#define CONFIG_SYSCLK_PBC_DIV         0
#define CONFIG_SYSCLK_PBD_DIV         0

// ===== Disable all non-essential peripheral clocks
//#define CONFIG_SYSCLK_INIT_CPUMASK  0
//#define CONFIG_SYSCLK_INIT_PBAMASK  SYSCLK_USART1
//#define CONFIG_SYSCLK_INIT_PBBMASK  0
//#define CONFIG_SYSCLK_INIT_PBCMASK  0
//#define CONFIG_SYSCLK_INIT_PBDMASK  0
//#define CONFIG_SYSCLK_INIT_HSBMASK  0

// ===== PLL Options
#define CONFIG_PLL0_SOURCE          PLL_SRC_OSC0
//#define CONFIG_PLL0_SOURCE          PLL_SRC_GCLK9

/* Fpll0 = (Fclk * PLL_mul) / PLL_div */
#define CONFIG_PLL0_MUL             (48000000UL / BOARD_OSC0_HZ)
#define CONFIG_PLL0_DIV             1
//#define CONFIG_PLL0_MUL             (192000000 / FOSC0) /* Fpll = (Fclk * PLL_mul) / PLL_div */
//#define CONFIG_PLL0_DIV             4 /* Fpll = (Fclk * PLL_mul) / PLL_div */

// ==== DFLL Options
//#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC0
//#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_RCSYS
#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_OSC32K
//#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_RC120M
//#define CONFIG_DFLL0_SOURCE         GENCLK_SRC_RC32K

/* Fdfll = (Fclk * DFLL_mul) / DFLL_div */
#define CONFIG_DFLL0_FREQ           48000000UL
//#define CONFIG_DFLL0_MUL            ((4 * CONFIG_DFLL0_FREQ) / BOARD_OSC32_HZ)
//#define CONFIG_DFLL0_DIV            4
#define CONFIG_DFLL0_MUL            (CONFIG_DFLL0_FREQ / BOARD_OSC32_HZ)
#define CONFIG_DFLL0_DIV            1

// ===== USB Clock Source Options
#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
//#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_DFLL

/* Fusb = Fsys / USB_div */
#define CONFIG_USBCLK_DIV           1

// ===== GCLK9 option
//#define CONFIG_GCLK9_SOURCE           GENCLK_SRC_GCLKIN0
//#define CONFIG_GCLK9_DIV              1


//! [tc_define_peripheral]
/* use tc peripheral 0. */
//#define tc             tc0
//#define tc_peripheral  0
//! [tc_define_peripheral]

//! [tc_define_ch1]
/* configure tc0 channel 1 as waveform output. */
//#define tc_channel_waveform 1
//#define id_tc_waveform      id_tc1
//#define pin_tc_waveform     pin_tc0_tioa1
//#define pin_tc_waveform_mux pin_tc0_tioa1_mux
//! [tc_define_ch1]

//! [tc_define_ch2]
/* configure tc0 channel 2 as capture input. */
//#define tc_channel_capture 2
//#define id_tc_capture id_tc2
//#define pin_tc_capture pin_tc0_tioa2
//#define pin_tc_capture_mux pin_tc0_tioa2_mux
//! [tc_define_ch2]

//! [tc_define_irq_handler]
/* use tc2_handler for tc capture interrupt. */
//#define tc_handler  tc2_handler
//#define tc_irqn     tc2_irqn
//! [tc_define_irq_handler]

#endif /* CONF_CLOCK_H_INCLUDED */
