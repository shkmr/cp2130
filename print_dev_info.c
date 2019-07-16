/*
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iconv.h>
#include "cp2130.h"

void dump(char *name, char *buf, size_t siz)
{
  int n;
  printf("%s siz=%zu\n",  name, siz);
  for (n=0; n < siz; n++) {
    printf("%04d: %2d '%c'\n", n, buf[n], buf[n]);
  }
}

void utf16_to_utf8(char *utf16, size_t utf16len,
                   char *utf8,  size_t utf8len)
{
  iconv_t conv;
  conv = iconv_open("UTF-8", "UTF-16LE");
  iconv(conv, &utf16, &utf16len, &utf8, &utf8len);
  iconv_close(conv);
}

void print_dev_info(cp2130_t dev)
{
  enum { BUFLEN = 256 };
  char utf16[BUFLEN];
  char utf8[BUFLEN];
  int major, minor;

  cp2130_get_readonly_version(dev, &major, &minor);
  printf("Connected. cp2130 version %d.%d\n", major, minor);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_manufacturing_string(dev, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  /* dump("utf16", utf16, BUFLEN);
     dump("utf8",  utf8,  BUFLEN); */
  printf("manufacturer: \"%s\"\n", utf8);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_product_string(dev, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  printf("product: \"%s\"\n", utf8);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_serial_string(dev, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  printf("serial: \"%s\"\n", utf8);

  cp2130otp_get_usb_config(dev, utf8);
  printf("USB device release number: %d.%d\n", utf8[6], utf8[7]);
  if (utf8[8] == 1)
    printf("USB high priority: write\n");
  else if (utf8[8] == 0)
    printf("USB high priority: read\n");
  else
    printf("USB high priority: error\n");
}

/* EOF */
