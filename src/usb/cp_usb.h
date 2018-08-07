#include <asf.h>
#include <string.h>
#include "conf_usb_host.h"
#include "conf_cph.h"

#define PRINT_JOB "^XA^XFR:STOREFMT.ZPL^FS^FO50,50^ADN,36,20^FDCPH^FS^XZ"
#define STATUS_JOB "~HQES"
#define PRINT_JOB_SIZE 56
#define STATUS_JOB_SIZE 5
#define SERIAL_SIZE 50

volatile bool main_b_usb_dev; // this will indicate that the bulk out can be started.
volatile int main_usb_in;
uint8_t out_buffer[PRINT_JOB_SIZE + 1];
uint8_t out_status_buffer[STATUS_JOB_SIZE + 1];
uint8_t in_buffer[SERIAL_SIZE + 1];


void main_usb_sof_event(void);
void main_usb_vendor_change(uhc_device_t *dev, bool b_present);
void get_num_conn_devices(void);
void usb_init(void);
void print_bulk_out(void);
void print_bulk_in_cb(usb_add_t add, usb_ep_t ep, uhd_trans_status_t status, iram_size_t nb_transferred);