/*
 *     Test code using libusb-0.1
 */

#include <stdio.h>
#include <usb.h>

int main(int c, char *v[])
{
  struct usb_bus                   *bus;
  struct usb_device                *dev;
  struct usb_config_descriptor    *conf;
  struct usb_interface            *intf;
  struct usb_interface_descriptor  *ifd;
  int                          i, npipe;

  usb_init();
  usb_find_busses();
  usb_find_devices();

  for (bus = usb_get_busses(); bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      printf("%04x:%04x\n",
             dev->descriptor.idVendor,
             dev->descriptor.idProduct);
    }
  }
}
