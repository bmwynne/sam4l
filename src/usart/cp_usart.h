#include <asf.h>
#include <string.h>
#include "conf_cph.h"
#include "conf_uart_serial.h"

#define BUFFER_SIZE 100               /** Size of the receive buffer used by the PDCA, in bytes. */
#define MAX_BUF_NUM 1                 /** Max buffer number. */
#define ALL_INTERRUPT_MASK 0xffffffff /** All interrupt mask. */
#define PDCA_RX_CHANNEL 0
#define PDCA_TX_CHANNEL 1
#define TC_FREQ 10 /** Timer counter frequency in Hz. */

#define STRING_EOL "\r"
#define STRING_HEADER                           \
    "-- CPH Cellular USB Printer Online --\r\n" \
    "-- " BOARD_NAME                            \
    " --\r\n"                                   \
    "-- Compiled: "__DATE__                     \
    " "__TIME__                                 \
    " --" STRING_EOL                            \
    "-- Repo: https://github.com/bmwynne/sam4l\n\r"

typedef mdm_config_t;
typedef void (*console_line_cb_t)(void);

/** Receive buffer. */
extern volatile uint8_t gs_puc_buffer[2][BUFFER_SIZE];
/** Next Receive buffer. */
static gs_puc_nextbuffer[2][BUFFER_SIZE];
/** Current bytes in buffer. */
static uint32_t gs_ul_size_buffer;
/** Current bytes in next buffer. */
static uint32_t gs_ul_size_nextbuffer;
/** Buffer number in use. */
extern volatile uint8_t gs_uc_buf_num;
/** Flag of one transfer end. */
static uint8_t g_uc_transend_flag;

extern volatile uint8_t g_v_puc_linebuffer[10];
extern volatile uint8_t linebuffer_data[BUFFER_SIZE * 2 + 1];

int tail;
extern int count;

int parse_linebuffer(char* s);
void concat_dma_buf_cb(uint8_t* buff0, uint8_t* buff1, int len_b0, int len_b1);

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

void configure_console(void);

/**
 * \brief Interrupt handler for USART. Echo the bytes received and start the
 * next receive.
 */
void USART_Handler(void);
void pdca_config_enable(void);
void run_console(void);
void terminal(void);
void console_init(uint8_t* buf, console_line_cb_t cb);
int read(void * ptr, size_t size);
void mdm_config(mdm_config_t cfg, char* port);
int mdm_write(char* data, int data_len);
int mdm_read(char* data_buf, int data_len);
void mdm_start(void);
void mdm_stop(void);