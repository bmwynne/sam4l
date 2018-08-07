#include "cp_printer.h"

void printer_read(void) {
    if (main_usb_in) {
        main_usb_in = 0;
        memset(in_buffer, 0, sizeof(in_buffer));
        uhi_vendor_bulk_in_run(in_buffer, sizeof(in_buffer), print_bulk_in_cb);
    }
}
void print(void) {
    if (ui_usb_dev_enum) {
        printf("-----LOG MAIN WHILE-----:\n\rUSB DEVICE ENUMERATED.\n\r");
        //get_num_conn_devices();
        print_bulk_out();
        ui_usb_dev_enum = 0;
    }
    printer_read();
}
