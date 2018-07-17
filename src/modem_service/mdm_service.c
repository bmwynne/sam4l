#include "mdm_service.h"
#include "cellular.h"
#include "mdm_usart.h"
#include "gps.h"
#include "power.h"


uint8_t test_int = 0;
#define CTRL_Z 26
#define MAX_BUF_SIZE 128
// id, sent, rcvd, pending_in, pending_out
#define SI_FMT "#SI: %d,%d,%d,%d,%d"
// id, state, local IP, local port, target IP, target port
#define SS_FMT "#SS: %d,%d,%[^,],%d,%[^,],%d"
// id, num_bytes, data
#define SRCV_FMT "#SRECV: %d,%d\n%[^n]"

// UART Data buffer
char data_read_buff[MAX_BUF_SIZE];
uint8_t bytes_read = 0;

// Data buffer for socket send
char send_data_buff[MAX_BUF_SIZE];
socket_status sckt_status;
void * return_data = NULL;

// State variables for tick process
enum cmd_status curr_status; 
mdm_cb_t curr_cb;
at_cmd_t* curr_cmds;
int cmd_idx = 0;
int num_cmds = 0;

mdm_config_t mdm_cfg;
mdm_socket_t mdm_sckt;

uint8_t SRING_STATUS = 0;
int sckt_id;
char mdm_data_buff[MAX_BUF_SIZE];
int rcvd_bytes_read = 0;
int rcvd_bytes_pending = 0;


// Modem AT Command definitions
void at_ok() { 
    char* cmd = "AT\r";
    mdm_write(cmd, strlen(cmd)); 
}
void at_echo_off() { 
    char* cmd = "ATE0\r";
    mdm_write(cmd, strlen(cmd)); 
}
void at_cmee() {
    char* cmd = "AT+CMEE=2\r";
    mdm_write(cmd, strlen(cmd)); 
}
void at_cgdcont() { 
    char* fmt = "AT+CGDCONT=%c,\"%s\",\"%s\"\r";
    char s[64];
    sprintf(s, fmt, '1', "IP", mdm_cfg.APN);
    mdm_write(s, strlen(s));
}
void at_sgact() {
    char* cmd = "AT#SGACT=1,1\r";
    mdm_write(cmd, strlen(cmd)); 
}
void at_sd() {
    char* fmt = "AT#SD=1,%d,%d,\"%s\",0,1,1\r";
    char s[64];
    sprintf(s, fmt, mdm_sckt.mode, mdm_sckt.port, mdm_sckt.IP);
    mdm_write(s, strlen(s));
}
void at_ssend() {
    char* cmd = "AT#SSEND=1\r";
    mdm_write(cmd, strlen(cmd));
}
void socket_send() {
    char* fmt = "%s%c";
    char s[MAX_BUF_SIZE+1];
    sprintf(s, fmt, send_data_buff, CTRL_Z);
    mdm_write(s, strlen(s));
}
void socket_rcv() {
    char* fmt = "AT#SRECV=%d,%d\r";
    char s[64];
    sprintf(s, fmt, 1, MAX_BUF_SIZE);
    mdm_write(s, strlen(s));
}
void at_ss() {
    char* cmd = "AT#SS=1\r";
    mdm_write(cmd, strlen(cmd));
}
void at_si() {
    char* cmd = "AT#SI=1\r";
    mdm_write(cmd, strlen(cmd));
}

void at_sh() {
    char* cmd = "AT#SH=1\r";
    mdm_write(cmd, strlen(cmd));
}



// Init commands
at_cmd_t init_cmds[4] = {at_echo_off, at_cmee, at_cgdcont, at_sgact};
void mdm_init(mdm_cb_t init_cb) {
    curr_cb = init_cb;
    curr_cmds = init_cmds;
    num_cmds = 4;
    return_data = NULL;
    curr_status = AT_IDLE;
}

at_cmd_t open_cmds[1] = {at_sd};
void mdm_open(mdm_socket_t socket, mdm_cb_t open_cb) {
    curr_cb = open_cb;
    mdm_sckt = socket;
    curr_cmds = open_cmds;
    num_cmds = 1;
    return_data = NULL;
    curr_status = AT_IDLE;
}

at_cmd_t send_cmds[2] = {at_ssend, socket_send};
void mdm_send(mdm_socket_t socket, char * data, int data_len, mdm_cb_t send_cb) {
    strncpy(send_data_buff, data, data_len);
    mdm_sckt = socket;
    curr_cb = send_cb;
    curr_cmds = send_cmds;
    num_cmds = 2;
    return_data = NULL;
    curr_status = AT_IDLE;
}

uint8_t mdm_data_rdy() {
    return rcvd_bytes_pending;
}

int mdm_recv(char * data, int max_data_len) {
    if(rcvd_bytes_pending) {
        char temp_buff[MAX_BUF_SIZE];
        int num_to_read = rcvd_bytes_pending;
        if (max_data_len < rcvd_bytes_pending) { num_to_read = max_data_len; }
        memcpy(data, mdm_data_buff, num_to_read);
        rcvd_bytes_pending -= num_to_read;
        memcpy(temp_buff, &(mdm_data_buff[num_to_read]), rcvd_bytes_pending);
        memset(mdm_data_buff, 0, MAX_BUF_SIZE);
        memcpy(mdm_data_buff, temp_buff, rcvd_bytes_pending);
        return num_to_read;
    }
    return 0;
}

at_cmd_t close_cmds[1] = {at_sh};
void mdm_close(mdm_socket_t socket, mdm_cb_t close_cb) {
    curr_cb = close_cb;
    mdm_sckt = socket;
    curr_cmds = close_cmds;
    num_cmds = 1;
    curr_status = AT_IDLE;
}

at_cmd_t status_cmds[2] = {at_ss, at_si};
void mdm_status(mdm_socket_t socket, mdm_cb_t status_cb) {
    curr_cb = status_cb;
    mdm_sckt = socket;
    curr_cmds = status_cmds;
    num_cmds = 2;
    return_data = &sckt_status;
    curr_status = AT_IDLE;
}

void mdm_loc_config(mdm_loc_config_t * config, mdm_cb_t loc_config_cb);
void mdm_loc_start(mdm_cb_t loc_cb);
void mdm_loc_stop(mdm_cb_t loc_cb);

uint8_t mdm_tick() {
    test_int += 1;

    bytes_read += mdm_read(data_read_buff, MAX_BUF_SIZE);
    if(strstr(data_read_buff, "SRING: 1")) {
        SRING_STATUS = 1;
    }
#ifdef AUTO_RECCONECT
    if(strstr(data_read_buff, "NO CARRIER")) {
        at_sd();
        // printf("RECONNECTED\r\n");
    }
#endif


    // Main state loop, handle any active command set if there is one, calls the approriate call back on ERROR or FINISHED
    switch (curr_status) {
        case AT_INACTIVE:
            if (SRING_STATUS == 1) {
                memset(data_read_buff, 0, MAX_BUF_SIZE);
                socket_rcv();
                bytes_read += mdm_read(data_read_buff, MAX_BUF_SIZE);
                sscanf(strstr(data_read_buff, "#SRECV: "), SRCV_FMT,
                &(sckt_id),
                &(rcvd_bytes_read),
                &(mdm_data_buff[rcvd_bytes_pending]));
                rcvd_bytes_pending += rcvd_bytes_read;
                // printf("Bytes Read: %d, Bytes Pending: %d, Data: %s\r\n", rcvd_bytes_read, rcvd_bytes_pending, mdm_data_buff);
                rcvd_bytes_read = 0;
                SRING_STATUS = 0;
            }
            break;
            
        case AT_IDLE:
            curr_cmds[cmd_idx]();
            curr_status = AT_PENDING;
            break;

        case AT_PENDING:
            if(strstr(data_read_buff, "OK")) {
                if(curr_cmds[cmd_idx] == at_ss) {
                    sscanf(strstr(data_read_buff, "#SS: "), SS_FMT, 
                    &(sckt_status.sock_id), 
                    &(sckt_status.state), 
                    sckt_status.bound_ip, 
                    &(sckt_status.bound_port), 
                    sckt_status.conn_ip, 
                    &(sckt_status.conn_port));

                }
                if (curr_cmds[cmd_idx] == at_si) {
                    sscanf(strstr(data_read_buff, "#SI: "), SI_FMT, 
                    &(sckt_status.sock_id), 
                    &(sckt_status.sent_bytes), 
                    &(sckt_status.rcvd_bytes), 
                    &(sckt_status.sent_pnding), 
                    &(sckt_status.rcvd_pnding));

                }
                curr_status = AT_SUCCESS;
                memset(data_read_buff, 0, MAX_BUF_SIZE);
            }
            if(strstr(data_read_buff, "ERROR")) { 
                curr_status = AT_ERROR;
                memset(data_read_buff, 0, MAX_BUF_SIZE);
            }
            if(strstr(data_read_buff, ">") && curr_cmds[cmd_idx] == at_ssend) { 
                curr_status = AT_SUCCESS;
                memset(data_read_buff, 0, MAX_BUF_SIZE);
            }
            break;

        case AT_SUCCESS:    
            cmd_idx += 1;
            if(cmd_idx == num_cmds) { curr_status = AT_FINISHED; }
            else { curr_status = AT_IDLE; }
            break;

        case AT_FINISHED:
            (*curr_cb)(curr_status, return_data);
            cmd_idx = 0;
            num_cmds = 0;
            curr_status = AT_INACTIVE;            
            break;

        default:
            (*curr_cb)(curr_status, return_data);
            cmd_idx = 0;
            num_cmds = 0;
            curr_status = AT_INACTIVE;            
            break;
    }
    
    // Return state on each pass, enabling user to use while(!mdm_tick()); to block if desired.
    return curr_status;
}

int test_func() {
    return (test_int + test_cell() + test_gps() + test_power());
}

int uart_fd;

int config_serial(char* port) {
    return port;
}

void mdm_config(mdm_config_t cfg, char* port) {
    mdm_cfg = cfg;
    // TODO: Add error handling in case port can't be opened
    uart_fd = config_serial(port);
}
int mdm_write(char* data, int data_len) {
    return write(uart_fd, data, data_len);
}
int mdm_read(char* data_buf, int data_len) {
    return read(uart_fd, data_buf, data_len);
}
void mdm_start(void) {
    
}
void mdm_stop(void) {
    if(uart_fd) {
        close(uart_fd);
    }
}
