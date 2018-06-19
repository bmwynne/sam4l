#include "cp_usb.h"

volatile bool main_b_usb_dev = false;
volatile int main_usb_in = 0;
uint8_t out_buffer[PRINT_JOB_SIZE + 1] = PRINT_JOB;
uint8_t out_status_buffer[STATUS_JOB_SIZE + 1] = STATUS_JOB;

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

void get_num_conn_devices(void) {
	uint8_t b_num_devices;
	b_num_devices = uhc_get_device_number();
	printf("-----LOG GET_NUM_CONN_DEVICES()-----:\n\rNumber of devices connected: %d.\n\r", b_num_devices);
}

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