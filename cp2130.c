/* -*- mode: C; compile-command: "make test_cp2130"; -*- */
/*
 *      CP2130 API
 *
 *      https://www.silabs.com/documents/public/application-notes/AN792.pdf
 *
 *                                       written by skimu@me.com
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <err.h>
#include "usbcom.h"
#include "cp2130.h"

/*
 *      API :
 *
 *      TODO :  Check parameter range and reject invalid parameter.
 *
 */

/*
 *       RequestType = 0xc0,   Device-to-Host, Vendor, Device
 */
int cp2130_get_clock_divider(usbcom_t com)
{
  unsigned char buf[1];
  usbcom_control_msg(com, 0xc0, 0x46, 0, 0, buf, 1);
  return (int)buf[0];
}

void cp2130_get_event_counter(usbcom_t com, int *mode, int *count)
{
  unsigned char buf[3];
  usbcom_control_msg(com, 0xc0, 0x44, 0, 0, buf, 3);
  *mode = buf[0];
  *count = (buf[1]<<8)+buf[2];
}

int cp2130_get_full_threshold(usbcom_t com)
{
  unsigned char buf[1];
  usbcom_control_msg(com, 0xc0, 0x34, 0, 0, buf, 1);
  return buf[0];
}

void cp2130_get_gpio_chip_select(usbcom_t com, int *ch, int *pin)
{
  unsigned char buf[4];
  usbcom_control_msg(com, 0xc0, 0x24, 0, 0, buf, 4);
  *ch  = (buf[0]<<8)+buf[1];
  *pin = (buf[2]<<8)+buf[3];
}

void cp2130_get_gpio_mode_and_level(usbcom_t com, int *mode, int *level)
{
  unsigned char buf[4];
  usbcom_control_msg(com, 0xc0, 0x22, 0, 0, buf, 4);
  *mode  = (buf[0]<<8)+buf[1];
  *level = (buf[2]<<8)+buf[3];
}

int cp2130_get_gpio_values(usbcom_t com)
{
  int r;
  unsigned char buf[2];
  usbcom_control_msg(com, 0xc0, 0x20, 0, 0, buf, 2);
  r = (buf[0]<<8)+buf[1];
  return r;
}

int cp2130_get_rtr_state(usbcom_t com)
{
  unsigned char buf[1];
  usbcom_control_msg(com, 0xc0, 0x36, 0, 0, buf, 1);
  return buf[0];
}

void cp2130_get_spi_word(usbcom_t com, unsigned char *buf)
{
  usbcom_control_msg(com, 0xc0, 0x30, 0, 0, buf, 11);
}

void cp2130_get_spi_delay(usbcom_t com, int channel,
                          int *mask,
                          int *inter_byte_delay,
                          int *post_assert_delay,
                          int *pre_deassert_delay)

{
  unsigned char buf[8];
  usbcom_control_msg(com, 0xc0, 0x32, 0, channel, buf, 8);
  if (channel != buf[0])
    warnx("cp2130_get_spi_delay ch conflict given %d, but get %d", channel, buf[0]);
  *mask  = buf[1];
  *inter_byte_delay   = (buf[2]<<8)+buf[3];
  *post_assert_delay  = (buf[4]<<8)+buf[5];
  *pre_deassert_delay = (buf[6]<<8)+buf[7];
}

void cp2130_get_readonly_version(usbcom_t com, int *major, int *minor)
{
  unsigned char buf[2];
  usbcom_control_msg(com, 0xc0, 0x11, 0, 0, buf, 2);
  *major = buf[0];
  *minor = buf[1];
}

/*
 *       RequestType = 0x40,   Host-to-Device, Vendor, Device
 */

void cp2130_reset_device(usbcom_t com)
{
  usbcom_control_msg(com, 0x40, 0x10, 0, 0, NULL, 0);
}

void cp2130_set_clock_divider(usbcom_t com, int divider)
{
  unsigned char buf[1];
  buf[0] = divider;
  usbcom_control_msg(com, 0x40, 0x47, 0, 0, buf, 1);
}

void cp2130_set_event_counter(usbcom_t com, int mode, int count)
{
  unsigned char buf[3];
  buf[0] = mode;
  buf[1] = ((count>>8)&0x00ff);
  buf[2] = ((count>>0)&0x00ff);
  usbcom_control_msg(com, 0x40, 0x45, 0, 0, buf, 3);
}

void cp2130_set_full_threshold(usbcom_t com, int threshold)
{
  unsigned char buf[1];
  buf[0] = threshold;
  usbcom_control_msg(com, 0x40, 0x35, 0, 0, buf, 1);
}

void cp2130_set_gpio_chip_select(usbcom_t com, int channel, int control)
{
  unsigned char buf[2];
  buf[0] = channel;
  buf[1] = control;
  usbcom_control_msg(com, 0x40, 0x25, 0, 0, buf, 2);
}

void cp2130_set_gpio_mode_and_level(usbcom_t com, int index, int mode, int level)
{
  unsigned char buf[3];
  buf[0] = index;
  buf[1] = mode;
  buf[2] = level;
  usbcom_control_msg(com, 0x40, 0x23, 0, 0, buf, 3);
}

void cp2130_set_gpio_values(usbcom_t com, int level, int mask)
{
  unsigned char buf[4];
  buf[0] = ((level>>8)&0x00ff);
  buf[1] = ((level>>0)&0x00ff);
  buf[2] = ((mask>>8)&0x00ff);
  buf[3] = ((mask>>0)&0x00ff);
  usbcom_control_msg(com, 0x40, 0x21, 0, 0, buf, 4);
}

void cp2130_set_rtr_stop(usbcom_t com, int val)
{
  unsigned char buf[1];
  buf[0] = val;
  usbcom_control_msg(com, 0x40, 0x37, 0, 0, buf, 1);
}

void cp2130_set_spi_word(usbcom_t com, int channel, int word)
{
  unsigned char buf[2];
  buf[0] = channel;
  buf[1] = word;
  usbcom_control_msg(com, 0x40, 0x31, 0, 0, buf, 2);
}

void cp2130_set_spi_delay(usbcom_t com, int channel,
                          int mask,
                          int inter_byte_delay,
                          int post_assert_delay,
                          int pre_deassert_delay)

{
  unsigned char buf[8];
  buf[0] = channel;
  buf[1] = mask;
  buf[2] = ((inter_byte_delay>>8)&0x00ff);
  buf[3] = ((inter_byte_delay>>0)&0x00ff);
  buf[4] = ((post_assert_delay>>8)&0x00ff);
  buf[5] = ((post_assert_delay>>0)&0x00ff);
  buf[6] = ((pre_deassert_delay>>8)&0x00ff);
  buf[7] = ((pre_deassert_delay>>0)&0x00ff);
  usbcom_control_msg(com, 0x40, 0x33, 0, 0, buf, 8);
}

/*
 *       OTP ROM Configuration Commands Part 0xc0,  Devicet-to-Host
 */

int cp2130otp_get_lock_byte(usbcom_t com)
{
  unsigned char buf[2];
  usbcom_control_msg(com, 0xc0, 0x6e, 0, 0, buf, 2);
  return (buf[0]<<8)+buf[1];
}

void cp2130otp_get_manufacturing_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];
  int len;

  /*
   *    TODO:  Not rejecting small str size, but return string as much as possible.
   */
  if (size_of_utf16 < 124) {
    warnx("cp2130otp_get_manufacturing_string: size_of_str is too small (%d), has to be greater than or equal to 124", size_of_utf16);
    return;
  }
  usbcom_control_msg(com, 0xc0, 0x62, 0, 0, buf, 64);
  len = buf[0];
  if (buf[1] != 0x03) warnx("cp2130otp_get_manufacturing_string: descriptor type is not 0x03 but 0x%02x", buf[1]);
  if (len > 63) warnx("cp2130otp_get_manufacturing_string: length is more than 63 (got %d)", len);
  memset(utf16, 0, size_of_utf16);
  memcpy(utf16, buf + 2, len - 3); /* buf[63] is not part of the string */
  if (len == 63) {
    usbcom_control_msg(com, 0xc0, 0x64, 0, 0, buf, 64);
    memcpy(utf16+61, buf, 63);     /* buf[63] is not part of the string */
  }
}

void cp2130otp_get_pin_config(usbcom_t com, void *buf)
{
  usbcom_control_msg(com, 0xc0, 0x6c, 0, 0, buf, 20);
}

void cp2130otp_get_product_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];
  int len;

  /*
   *    TODO:  Not rejecting small str size, but return string as much as possible.
   */
  if (size_of_utf16 < 124) {
    warnx("cp2130otp_get_product_string: size_of_utf16 is too small (%d), has to be greater than or equal to 124", size_of_utf16);
    return;
  }
  usbcom_control_msg(com, 0xc0, 0x66, 0, 0, buf, 64);
  len = buf[0];
  if (buf[1] != 0x03) warnx("cp2130otp_get_product_string: descriptor type is not 0x03 but 0x%02x", buf[1]);
  if (len > 63) warnx("cp2130otp_get_product_string: length is more than 63 (got %d)", len);
  memset(utf16, 0, size_of_utf16);
  memcpy(utf16, buf + 2, len - 3); /* buf[63] is not part of the string */
  if (len == 63) {
    usbcom_control_msg(com, 0xc0, 0x68, 0, 0, buf, 64);
    memcpy(utf16+61, buf, 63);     /* buf[63] is not part of the string */
  }
}

void cp2130otp_get_prom_config(usbcom_t com, int index, void *buf)
{
  usbcom_control_msg(com, 0xc0, 0x70, 0, index, buf, 64);
}

void cp2130otp_get_serial_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];
  int len;
  usbcom_control_msg(com, 0xc0, 0x6a, 0, 0, buf, 64);
  len = buf[0];
  if (buf[1] != 0x03) warnx("cp2130otp_get_serial_string: descriptor type is not 0x03 but 0x%02x", buf[1]);
  if (len > 63) warnx("cp2130otp_get_serial_string: length is more than 63 (got %d)", len);
  memset(utf16, 0, size_of_utf16);
  if (size_of_utf16 > len - 3) {
    memcpy(utf16, buf + 2, len - 3);
  } else {
    memcpy(utf16, buf + 2, size_of_utf16);
  }
}

void cp2130otp_get_usb_config(usbcom_t com, void *buf)
{
  usbcom_control_msg(com, 0xc0, 0x60, 0, 0, buf, 9);
}

/*
 *       OTP ROM Configuration Commands Part 0x40,  Host-to-Device
 *
 */

static int memory_key = 0;  /* should belong to device context */

void cp2130otp_set_memory_key(usbcom_t com, int key)
{
  memory_key = key;
}

void cp2130otp_set_lock_byte(usbcom_t com, void *buf)
{
  usbcom_control_msg(com, 0x40, 0x6f, memory_key, 0, buf, 2);
}

void cp2130otp_set_manufacturing_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 61) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(com, 0x40, 0x63, memory_key, 0, buf, 64);

  } else if (size_of_utf16 < 124) {

    memset(buf, 0 , 64);
    buf[0] = 61;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, 61);
    usbcom_control_msg(com, 0x40, 0x63, memory_key, 0, buf, 64);

    memset(buf, 0 , 64);
    memcpy(buf, utf16 + 61, size_of_utf16 - 61);
    usbcom_control_msg(com, 0x40, 0x65, memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_manufacturing_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_pin_config(usbcom_t com, void *buf)
{
  usbcom_control_msg(com, 0x40, 0x6d, memory_key, 0, buf, 20);
}

void cp2130otp_set_product_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 61) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(com, 0x40, 0x67, memory_key, 0, buf, 64);

  } else if (size_of_utf16 < 124) {

    memset(buf, 0 , 64);
    buf[0] = 61;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, 61);
    usbcom_control_msg(com, 0x40, 0x67, memory_key, 0, buf, 64);

    memset(buf, 0 , 64);
    memcpy(buf, utf16 + 61, size_of_utf16 - 61);
    usbcom_control_msg(com, 0x40, 0x69, memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_product_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_prom_config(usbcom_t com, int index, void *buf)
{
  if ((index >= 0)  && (index <= 7)) {
    usbcom_control_msg(com, 0x40, 0x71, memory_key, index, buf, 64);
  } else {
    warnx("cp2130otp_set_prom_config: invalid index (%d), ignored", index);
  }
}

void cp2130otp_set_serial_string(usbcom_t com, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 60) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(com, 0x40, 0x67, memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_serial_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_usb_config(usbcom_t com, void *buf)
{
  usbcom_control_msg(com, 0x40, 0x61, memory_key, 0, buf, 10);
}

/*
 *       Convenience
 */

int cp2130_get_event_counter_count(usbcom_t com)
{
  int mode, count;
  cp2130_get_event_counter(com, &mode, &count);
  return count;
}

int cp2130_get_event_counter_mode(usbcom_t com)
{
  int mode, count;
  cp2130_get_event_counter(com, &mode, &count);
  return mode;
}

enum {
      GPIO_00 = 0x0008,
      GPIO_01 = 0x0010,
      GPIO_02 = 0x0020,
      GPIO_03 = 0x0040,
      GPIO_04 = 0x0080,
      GPIO_05 = 0x0100,
      GPIO_06 = 0x0400,
      GPIO_07 = 0x0800,
      GPIO_08 = 0x1000,
      GPIO_09 = 0x2000,
      GPIO_10 = 0x4000,
};

int  cp2130_get_gpio00(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_00);}
int  cp2130_get_gpio01(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_01);}
int  cp2130_get_gpio02(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_02);}
int  cp2130_get_gpio03(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_03);}
int  cp2130_get_gpio04(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_04);}
int  cp2130_get_gpio05(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_05);}
int  cp2130_get_gpio06(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_06);}
int  cp2130_get_gpio07(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_07);}
int  cp2130_get_gpio08(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_08);}
int  cp2130_get_gpio09(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_09);}
int  cp2130_get_gpio10(usbcom_t com)   {return (cp2130_get_gpio_values(com)&GPIO_10);}
void cp2130_set_gpio00(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_00, GPIO_00);}
void cp2130_set_gpio01(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_01, GPIO_01);}
void cp2130_set_gpio02(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_02, GPIO_02);}
void cp2130_set_gpio03(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_03, GPIO_03);}
void cp2130_set_gpio04(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_04, GPIO_04);}
void cp2130_set_gpio05(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_05, GPIO_05);}
void cp2130_set_gpio06(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_06, GPIO_06);}
void cp2130_set_gpio07(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_07, GPIO_07);}
void cp2130_set_gpio08(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_08, GPIO_08);}
void cp2130_set_gpio09(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_09, GPIO_09);}
void cp2130_set_gpio10(usbcom_t com)   {cp2130_set_gpio_values(com, GPIO_10, GPIO_10);}
void cp2130_reset_gpio00(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_00);}
void cp2130_reset_gpio01(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_01);}
void cp2130_reset_gpio02(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_02);}
void cp2130_reset_gpio03(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_03);}
void cp2130_reset_gpio04(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_04);}
void cp2130_reset_gpio05(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_05);}
void cp2130_reset_gpio06(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_06);}
void cp2130_reset_gpio07(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_07);}
void cp2130_reset_gpio08(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_08);}
void cp2130_reset_gpio09(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_09);}
void cp2130_reset_gpio10(usbcom_t com) {cp2130_set_gpio_values(com,       0, GPIO_10);}

#ifdef CP2130_MAIN

#include <unistd.h>
#include <iconv.h>

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

void print_dev_info(usbcom_t com)
{
  enum {
        BUFLEN = 256
  };
  char utf16[BUFLEN];
  char utf8[BUFLEN];
  int major, minor;

  cp2130_get_readonly_version(com, &major, &minor);
  printf("Connected. cp2130 version %d.%d\n", major, minor);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_manufacturing_string(com, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  /* dump("utf16", utf16, BUFLEN);
     dump("utf8",  utf8,  BUFLEN); */
  printf("manufacturer: \"%s\"\n", utf8);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_product_string(com, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  printf("product: \"%s\"\n", utf8);

  memset(utf16, 0, BUFLEN);
  memset(utf8,  0, BUFLEN);
  cp2130otp_get_serial_string(com, utf16, BUFLEN);
  utf16_to_utf8(utf16, BUFLEN, utf8, BUFLEN);
  printf("serial: \"%s\"\n", utf8);

  cp2130otp_get_usb_config(com, utf8);
  printf("USB device release number: %d.%d\n", utf8[6], utf8[7]);
  if (utf8[8] == 1)
    printf("USB high priority: write\n");
  else if (utf8[8] == 0)
    printf("USB high priority: read\n");
  else
    printf("USB high priority: error\n");
}

int main(int c, char *v[])
{
  int vendor, product, n;
  usbcom_t com;

  printf("cp2130 main\n");

  vendor  = 0x10c4;
  product = 0x87a0;

  com = usbcom_open(vendor, product);
  if (com == NULL)
    err(1, "cp2130: open failed");

  print_dev_info(com);

  for (n=0; n < 2; n++) {
    printf("LED ON:  Event Counter=%4d\n", cp2130_get_event_counter_count(com));
    cp2130_reset_gpio07(com);
    sleep(1);
    printf("LED OFF: Event Counter=%4d\n", cp2130_get_event_counter_count(com));
    cp2130_set_gpio07(com);
    sleep(1);
  }

  printf("bye\n");
  usbcom_close(com);
}

#endif
