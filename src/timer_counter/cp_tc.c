#include "cp_tc.h"


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