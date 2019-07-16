/*
 *      USB com using libusb-1.0 (standard in most linux systems)
 *
 *                                    written by skimu@me.com
 */

#ifdef LIBUSB10
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <libusb-1.0/libusb.h>
#include "usbcom.h"

struct usbcom {
  libusb_device_handle            *dev;
  int                            npipe;
};

static int debug = 0;
void usbcom_set_debug_level(int x) {debug = x;}

static int timeout = 1000;
void usbcom_set_timeout(int x) {timeout = x;}


usbcom_t usbcom_open(int vendor, int product)
{
  usbcom_t                          com;

  if ((com = (usbcom_t)malloc(sizeof(struct usbcom)))  == NULL) err(1, NULL);

  libusb_init(NULL);
  if ((com->dev = libusb_open_device_with_vid_pid(NULL, vendor, product)) == NULL) {
    free(com);
    return NULL;
  }
  if (libusb_claim_interface(com->dev, 0) != 0) {
    libusb_close(com->dev);
    free(com);
    return NULL;
  }
  /*
   *    TODO: Get NumEndpoints and put it into com->npipe.
   */
  com->npipe  = 0;
  return com;
}

void usbcom_close(usbcom_t com)
{
  libusb_release_interface(com->dev, 0);
  libusb_close(com->dev);
  free(com);
}

int usbcom_npipe(usbcom_t com)
{
  return com->npipe;
}

int usbcom_re_enumerate(usbcom_t com)
{
  /* XXX I don't know how to re-enumerate with libusb...
     usb_find_devices();
  */
  return 0;
}

int usbcom_control_msg(usbcom_t com, int type, int req, int val, int index, void *buf, int size)
{
  int r;
  r = libusb_control_transfer(com->dev, type, req, val, index, buf, size, timeout);
  return 0;
}

int usbcom_send(usbcom_t com, int pipe, void *buf, int len)
{
  int    r, actual_length;
  r = libusb_bulk_transfer(com->dev, pipe, buf, len, &actual_length, timeout);
  return 0;
}

int usbcom_receive(usbcom_t com, int pipe, void *buf, int max_len)
{
  int    r, actual_length;
  r = libusb_bulk_transfer(com->dev, pipe|0x80, buf, max_len, &actual_length, timeout);
  if (debug) fprintf(stderr, "usbcom: max_len=%d r=%d\n", max_len, r);
  return actual_length;
}

#endif /* LIBUS0B1 */
