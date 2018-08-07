#include "asf.h"

#define TC_FREQ             10               /** Timer counter frequency in Hz. */

/**
 * \brief Configure Timer Counter 0 (TC0) to generate an interrupt every 200ms.
 * This interrupt will be used to flush USART input and echo back.
 */

void configure_tc(void);

/**
 * \brief Interrupt handler for TC00. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
// void TC00_Handler(void);