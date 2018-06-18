#include <asf.h>
#include "conf_cph.h"

#define BUFFER_SIZE         100				/** Size of the receive buffer used by the PDCA, in bytes. */
#define MAX_BUF_NUM         1				/** Max buffer number. */
#define ALL_INTERRUPT_MASK  0xffffffff		/** All interrupt mask. */
#define PDCA_RX_CHANNEL  0
#define PDCA_TX_CHANNEL  1
#define TC_FREQ             10               /** Timer counter frequency in Hz. */

#define STRING_EOL    "\r"
#define STRING_HEADER "-- CPH Cellular USB Printer Online --\r\n" \
"-- "BOARD_NAME" --\r\n" \
"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL	\
"-- Repo: https://github.com/bmwynne/sam4l"

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


/**
 * \brief Configure Timer Counter 0 (TC0) to generate an interrupt every 200ms.
 * This interrupt will be used to flush USART input and echo back.
 */

void configure_tc(void);

/**
 * \brief Interrupt handler for TC00. Record the number of bytes received,
 * and then restart a read transfer on the USART if the transfer was stopped.
 */
void TC00_Handler(void);

void configure_console(void);

/**
 * \brief Interrupt handler for USART. Echo the bytes received and start the
 * next receive.
 */
void USART_Handler(void);

void pdca_config_enable(void);