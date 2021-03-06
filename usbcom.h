/* USB COM public interafaces */

#ifndef USBCOM_H
#define USBCOM_H

typedef struct usbcom *usbcom_t;

void usbcom_set_debug_level(int x);
void usbcom_set_timeout(int x);

/* 
 * usbcom_open :  Open USB device for communication
 * returns NULL if error.
 *
 */

usbcom_t usbcom_open(int vendor_id, int product_id);

/*
 *
 */

void usbcom_close(usbcom_t com);

/*
 *  usbcom_npipe() returns the number of pipe of 
 *  the device.
 *
 */

int usbcom_npipe(usbcom_t com);

/*
 *
 *
 */

int usbcom_re_enumerate(usbcom_t com);

/*
 * usbcom_control_msg : control_transfer
 *
 */
int usbcom_control_msg(usbcom_t com, int type, int req, int val, int index, void *buf, int size);


/* 
 * usbcom_send : send 
 * returns non-zero if error.
 *
 */
 
int usbcom_send(usbcom_t com, int pipe_idx, void *buf, int size);

/* 
 *  usbcom_receive : receive 
 *  returns number of bytes received;
 *
 */

int usbcom_receive(usbcom_t com, int pipe_idx, void *buf, int size);

#endif

/* EOF */

