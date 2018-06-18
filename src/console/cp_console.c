#include "cp_console.h"

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

void configure_tc(void)
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

void configure_console(void)
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

void pdca_config_enable(void) {
    pdca_enable(PDCA); /* Enable PDCA module clock */
	pdca_channel_set_config(PDCA_RX_CHANNEL, &pdca_rx_options);	/* Init PDCA channel with the pdca_options.*/
	pdca_channel_set_config(PDCA_TX_CHANNEL, &pdca_tx_options);	/* Init PDCA channel with the pdca_options.*/
	pdca_channel_enable(PDCA_RX_CHANNEL);	/* Enable PDCA channel, start receiving data. */
	pdca_channel_enable(PDCA_TX_CHANNEL);	/* Enable PDCA channel, start receiving data. */
}