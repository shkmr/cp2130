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
 *      Device context
 */

struct cp2130 {
  usbcom_t       com;
  unsigned char *wrbuf;
  size_t         wrbufsiz;
  int            oep;
  int            iep;
  int            memory_key;
};

static size_t default_wrbufsiz = 512;

void cp2130_set_default_wrbufsiz(size_t bufsiz)
{
  default_wrbufsiz = bufsiz;
}

void cp2130_set_wrbufsiz(cp2130_t dev, size_t newsize)
{
  free(dev->wrbuf);
  if ((dev->wrbuf = (unsigned char *)malloc(newsize)) == NULL) err(1, NULL);
  dev->wrbufsiz = newsize;
}

cp2130_t cp2130_open(int vendor_id, int product_id)
{
  cp2130_t dev;

  if ((dev = (cp2130_t)malloc(sizeof(struct cp2130)))  == NULL) err(1, NULL);
  if ((dev->wrbuf = (unsigned char *)malloc(default_wrbufsiz)) == NULL) err(1, NULL);
  dev->wrbufsiz = default_wrbufsiz;
  if ((dev->com = usbcom_open(vendor_id, product_id)) == NULL)  err(1, "cp2130: usb device open failed");
  dev->memory_key = 0;

  {
    unsigned char buf[9];
    cp2130otp_get_usb_config(dev, buf);
    if (buf[8] == 1) {
      dev->oep = 1; /* 1: high priority write */
      dev->iep = 2;
    } else {
      dev->oep = 2; /* 0: high priority read */
      dev->iep = 1;
    }
  }

  return dev;
}

void cp2130_close(cp2130_t dev)
{
  usbcom_close(dev->com);
  free(dev->wrbuf);
  free(dev);
}

/*
 *      API :
 */

/*
 *       Data transfer Commands
 */

int cp2130_read(cp2130_t dev, void *buf, int len)
{
  int r;
  unsigned char cmd[8];

  cmd[0] = 0;
  cmd[1] = 0;
  cmd[2] = 0;
  cmd[3] = 0;
  cmd[4] = ((len>>0)&0x00ff);
  cmd[5] = ((len>>8)&0x00ff);
  cmd[6] = ((len>>16)&0x00ff);
  cmd[7] = ((len>>24)&0x00ff);

  usbcom_send(dev->com, dev->oep, cmd, 8);
  while (len > 0) {
    r = usbcom_receive(dev->com, dev->iep, buf, len);
    if (r == 0) break;
    len = len - r;
    buf = buf + r;
  }
  return len;
}

int cp2130_write(cp2130_t dev, void *buf, int len)
{
  unsigned char cmd[64];

  cmd[0] = 0;
  cmd[1] = 0;
  cmd[2] = 1;
  cmd[3] = 0;
  cmd[4] = ((len>>0)&0x00ff);
  cmd[5] = ((len>>8)&0x00ff);
  cmd[6] = ((len>>16)&0x00ff);
  cmd[7] = ((len>>24)&0x00ff);

  if (len <= 56) {

    memcpy(cmd+8, buf, len);
    usbcom_send(dev->com, dev->oep, cmd, len+8);

  } else {

    memcpy(cmd+8, buf, 56);
    usbcom_send(dev->com, dev->oep, cmd, 64);
    buf = buf + 56;
    len = len - 56;
    usbcom_send(dev->com, dev->oep, buf, len);

  }

  return 0;
}

int cp2130_write_read(cp2130_t dev, void *buf, int len)
{
  int r;

  dev->wrbuf[0] = 0;
  dev->wrbuf[1] = 0;
  dev->wrbuf[2] = 2;
  dev->wrbuf[3] = 0;
  dev->wrbuf[4] = ((len>>0)&0x00ff);
  dev->wrbuf[5] = ((len>>8)&0x00ff);
  dev->wrbuf[6] = ((len>>16)&0x00ff);
  dev->wrbuf[7] = ((len>>24)&0x00ff);

  if (len <= dev->wrbufsiz - 8) {

    memcpy(dev->wrbuf+8, buf, len);
    usbcom_send(dev->com, dev->oep, dev->wrbuf , len + 8);
    usbcom_receive(dev->com, dev->iep, buf, len);

  } else {

    memcpy(dev->wrbuf+8, buf, dev->wrbufsiz - 8);
    usbcom_send(dev->com, dev->oep, dev->wrbuf, dev->wrbufsiz);
    usbcom_receive(dev->com, dev->iep, buf, dev->wrbufsiz - 8);
    buf = buf + (dev->wrbufsiz - 8);
    len = len - (dev->wrbufsiz - 8);

    while (len > dev->wrbufsiz) {

      usbcom_send(dev->com, dev->oep, buf, dev->wrbufsiz);
      r = usbcom_receive(dev->com, dev->iep, buf, dev->wrbufsiz);
      if (r != dev->wrbufsiz) err(1, "cp2130_write_read buffer error");
      buf = buf + dev->wrbufsiz;
      len = len - dev->wrbufsiz;

    }
    usbcom_send(dev->com, dev->oep, buf, len);
    usbcom_receive(dev->com, dev->iep, buf, len);

  }
  return 0;
}

/*
 *       RequestType = 0xc0,   Device-to-Host, Vendor, Device
 */
int cp2130_get_clock_divider(cp2130_t dev)
{
  unsigned char buf[1];
  usbcom_control_msg(dev->com, 0xc0, 0x46, 0, 0, buf, 1);
  return (int)buf[0];
}

void cp2130_get_event_counter(cp2130_t dev, int *mode, int *count)
{
  unsigned char buf[3];
  usbcom_control_msg(dev->com, 0xc0, 0x44, 0, 0, buf, 3);
  *mode = buf[0];
  *count = (buf[1]<<8)+buf[2];
}

int cp2130_get_full_threshold(cp2130_t dev)
{
  unsigned char buf[1];
  usbcom_control_msg(dev->com, 0xc0, 0x34, 0, 0, buf, 1);
  return buf[0];
}

void cp2130_get_gpio_chip_select(cp2130_t dev, int *ch, int *pin)
{
  unsigned char buf[4];
  usbcom_control_msg(dev->com, 0xc0, 0x24, 0, 0, buf, 4);
  *ch  = (buf[0]<<8)+buf[1];
  *pin = (buf[2]<<8)+buf[3];
}

void cp2130_get_gpio_mode_and_level(cp2130_t dev, int *mode, int *level)
{
  unsigned char buf[4];
  usbcom_control_msg(dev->com, 0xc0, 0x22, 0, 0, buf, 4);
  *mode  = (buf[0]<<8)+buf[1];
  *level = (buf[2]<<8)+buf[3];
}

int cp2130_get_gpio_values(cp2130_t dev)
{
  int r;
  unsigned char buf[2];
  usbcom_control_msg(dev->com, 0xc0, 0x20, 0, 0, buf, 2);
  r = (buf[0]<<8)+buf[1];
  return r;
}

int cp2130_get_rtr_state(cp2130_t dev)
{
  unsigned char buf[1];
  usbcom_control_msg(dev->com, 0xc0, 0x36, 0, 0, buf, 1);
  return buf[0];
}

void cp2130_get_spi_word(cp2130_t dev, unsigned char *buf)
{
  usbcom_control_msg(dev->com, 0xc0, 0x30, 0, 0, buf, 11);
}

void cp2130_get_spi_delay(cp2130_t dev, int channel,
                          int *mask,
                          int *inter_byte_delay,
                          int *post_assert_delay,
                          int *pre_deassert_delay)
{
  unsigned char buf[8];
  usbcom_control_msg(dev->com, 0xc0, 0x32, 0, channel, buf, 8);
  if (channel != buf[0])
    warnx("cp2130_get_spi_delay: ch conflict given %d, but get %d", channel, buf[0]);
  *mask  = buf[1];
  *inter_byte_delay   = (buf[2]<<8)+buf[3];
  *post_assert_delay  = (buf[4]<<8)+buf[5];
  *pre_deassert_delay = (buf[6]<<8)+buf[7];
}

void cp2130_get_readonly_version(cp2130_t dev, int *major, int *minor)
{
  unsigned char buf[2];
  usbcom_control_msg(dev->com, 0xc0, 0x11, 0, 0, buf, 2);
  *major = buf[0];
  *minor = buf[1];
}

/*
 *       RequestType = 0x40,   Host-to-Device, Vendor, Device
 */

void cp2130_reset_device(cp2130_t dev)
{
  usbcom_control_msg(dev->com, 0x40, 0x10, 0, 0, NULL, 0);
}

void cp2130_set_clock_divider(cp2130_t dev, int divider)
{
  unsigned char buf[1];
  buf[0] = divider;
  usbcom_control_msg(dev->com, 0x40, 0x47, 0, 0, buf, 1);
}

void cp2130_set_event_counter(cp2130_t dev, int mode, int count)
{
  unsigned char buf[3];
  buf[0] = mode;
  buf[1] = ((count>>8)&0x00ff);
  buf[2] = ((count>>0)&0x00ff);
  usbcom_control_msg(dev->com, 0x40, 0x45, 0, 0, buf, 3);
}

void cp2130_set_full_threshold(cp2130_t dev, int threshold)
{
  unsigned char buf[1];
  buf[0] = threshold;
  usbcom_control_msg(dev->com, 0x40, 0x35, 0, 0, buf, 1);
}

void cp2130_set_gpio_chip_select(cp2130_t dev, int channel, int control)
{
  unsigned char buf[2];
  buf[0] = channel;
  buf[1] = control;
  usbcom_control_msg(dev->com, 0x40, 0x25, 0, 0, buf, 2);
}

void cp2130_set_gpio_mode_and_level(cp2130_t dev, int index, int mode, int level)
{
  unsigned char buf[3];
  buf[0] = index;
  buf[1] = mode;
  buf[2] = level;
  usbcom_control_msg(dev->com, 0x40, 0x23, 0, 0, buf, 3);
}

void cp2130_set_gpio_values(cp2130_t dev, int level, int mask)
{
  unsigned char buf[4];
  buf[0] = ((level>>8)&0x00ff);
  buf[1] = ((level>>0)&0x00ff);
  buf[2] = ((mask>>8)&0x00ff);
  buf[3] = ((mask>>0)&0x00ff);
  usbcom_control_msg(dev->com, 0x40, 0x21, 0, 0, buf, 4);
}

void cp2130_set_rtr_stop(cp2130_t dev, int val)
{
  unsigned char buf[1];
  buf[0] = val;
  usbcom_control_msg(dev->com, 0x40, 0x37, 0, 0, buf, 1);
}

void cp2130_set_spi_word(cp2130_t dev, int channel, int word)
{
  unsigned char buf[2];
  buf[0] = channel;
  buf[1] = word;
  usbcom_control_msg(dev->com, 0x40, 0x31, 0, 0, buf, 2);
}

void cp2130_set_spi_delay(cp2130_t dev, int channel,
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
  usbcom_control_msg(dev->com, 0x40, 0x33, 0, 0, buf, 8);
}

/*
 *       OTP ROM Configuration Commands Part 0xc0,  Devicet-to-Host
 */

int cp2130otp_get_lock_byte(cp2130_t dev)
{
  unsigned char buf[2];
  usbcom_control_msg(dev->com, 0xc0, 0x6e, 0, 0, buf, 2);
  return (buf[0]<<8)+buf[1];
}

void cp2130otp_get_manufacturing_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
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
  usbcom_control_msg(dev->com, 0xc0, 0x62, 0, 0, buf, 64);
  len = buf[0];
  if (buf[1] != 0x03) warnx("cp2130otp_get_manufacturing_string: descriptor type is not 0x03 but 0x%02x", buf[1]);
  if (len > 63) warnx("cp2130otp_get_manufacturing_string: length is more than 63 (got %d)", len);
  memset(utf16, 0, size_of_utf16);
  memcpy(utf16, buf + 2, len - 3); /* buf[63] is not part of the string */
  if (len == 63) {
    usbcom_control_msg(dev->com, 0xc0, 0x64, 0, 0, buf, 64);
    memcpy(utf16+61, buf, 63);     /* buf[63] is not part of the string */
  }
}

void cp2130otp_get_pin_config(cp2130_t dev, void *buf)
{
  usbcom_control_msg(dev->com, 0xc0, 0x6c, 0, 0, buf, 20);
}

void cp2130otp_get_product_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
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
  usbcom_control_msg(dev->com, 0xc0, 0x66, 0, 0, buf, 64);
  len = buf[0];
  if (buf[1] != 0x03) warnx("cp2130otp_get_product_string: descriptor type is not 0x03 but 0x%02x", buf[1]);
  if (len > 63) warnx("cp2130otp_get_product_string: length is more than 63 (got %d)", len);
  memset(utf16, 0, size_of_utf16);
  memcpy(utf16, buf + 2, len - 3); /* buf[63] is not part of the string */
  if (len == 63) {
    usbcom_control_msg(dev->com, 0xc0, 0x68, 0, 0, buf, 64);
    memcpy(utf16+61, buf, 63);     /* buf[63] is not part of the string */
  }
}

void cp2130otp_get_prom_config(cp2130_t dev, int index, void *buf)
{
  usbcom_control_msg(dev->com, 0xc0, 0x70, 0, index, buf, 64);
}

void cp2130otp_get_serial_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];
  int len;
  usbcom_control_msg(dev->com, 0xc0, 0x6a, 0, 0, buf, 64);
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

void cp2130otp_get_usb_config(cp2130_t dev, void *buf)
{
  usbcom_control_msg(dev->com, 0xc0, 0x60, 0, 0, buf, 9);
}

/*
 *       OTP ROM Configuration Commands Part 0x40,  Host-to-Device
 *
 */

void cp2130otp_set_memory_key(cp2130_t dev, int key)
{
  dev->memory_key = key;
}

void cp2130otp_set_lock_byte(cp2130_t dev, void *buf)
{
  usbcom_control_msg(dev->com, 0x40, 0x6f, dev->memory_key, 0, buf, 2);
}

void cp2130otp_set_manufacturing_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 61) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(dev->com, 0x40, 0x63, dev->memory_key, 0, buf, 64);

  } else if (size_of_utf16 < 124) {

    memset(buf, 0 , 64);
    buf[0] = 61;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, 61);
    usbcom_control_msg(dev->com, 0x40, 0x63, dev->memory_key, 0, buf, 64);

    memset(buf, 0 , 64);
    memcpy(buf, utf16 + 61, size_of_utf16 - 61);
    usbcom_control_msg(dev->com, 0x40, 0x65, dev->memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_manufacturing_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_pin_config(cp2130_t dev, void *buf)
{
  usbcom_control_msg(dev->com, 0x40, 0x6d, dev->memory_key, 0, buf, 20);
}

void cp2130otp_set_product_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 61) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(dev->com, 0x40, 0x67, dev->memory_key, 0, buf, 64);

  } else if (size_of_utf16 < 124) {

    memset(buf, 0 , 64);
    buf[0] = 61;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, 61);
    usbcom_control_msg(dev->com, 0x40, 0x67, dev->memory_key, 0, buf, 64);

    memset(buf, 0 , 64);
    memcpy(buf, utf16 + 61, size_of_utf16 - 61);
    usbcom_control_msg(dev->com, 0x40, 0x69, dev->memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_product_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_prom_config(cp2130_t dev, int index, void *buf)
{
  if ((index >= 0)  && (index <= 7)) {
    usbcom_control_msg(dev->com, 0x40, 0x71, dev->memory_key, index, buf, 64);
  } else {
    warnx("cp2130otp_set_prom_config: invalid index (%d), ignored", index);
  }
}

void cp2130otp_set_serial_string(cp2130_t dev, void *utf16, unsigned size_of_utf16)
{
  unsigned char buf[64];

  if (size_of_utf16 < 60) {

    memset(buf, 0 , 64);
    buf[0] = size_of_utf16 + 2;
    buf[1] = 0x03;
    memcpy(buf+2, utf16, size_of_utf16);
    usbcom_control_msg(dev->com, 0x40, 0x67, dev->memory_key, 0, buf, 64);

  } else{

    errx(1, "cp2130otp_set_serial_string: string too large (%d)", size_of_utf16);

  }
}

void cp2130otp_set_usb_config(cp2130_t dev, void *buf)
{
  usbcom_control_msg(dev->com, 0x40, 0x61, dev->memory_key, 0, buf, 10);
}

/*
 *       Convenience
 */

int cp2130_get_event_counter_count(cp2130_t dev)
{
  int mode, count;
  cp2130_get_event_counter(dev, &mode, &count);
  return count;
}

int cp2130_get_event_counter_mode(cp2130_t dev)
{
  int mode, count;
  cp2130_get_event_counter(dev, &mode, &count);
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

int  cp2130_get_gpio00(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_00);}
int  cp2130_get_gpio01(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_01);}
int  cp2130_get_gpio02(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_02);}
int  cp2130_get_gpio03(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_03);}
int  cp2130_get_gpio04(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_04);}
int  cp2130_get_gpio05(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_05);}
int  cp2130_get_gpio06(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_06);}
int  cp2130_get_gpio07(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_07);}
int  cp2130_get_gpio08(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_08);}
int  cp2130_get_gpio09(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_09);}
int  cp2130_get_gpio10(cp2130_t dev)   {return (cp2130_get_gpio_values(dev)&GPIO_10);}
void cp2130_set_gpio00(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_00, GPIO_00);}
void cp2130_set_gpio01(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_01, GPIO_01);}
void cp2130_set_gpio02(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_02, GPIO_02);}
void cp2130_set_gpio03(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_03, GPIO_03);}
void cp2130_set_gpio04(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_04, GPIO_04);}
void cp2130_set_gpio05(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_05, GPIO_05);}
void cp2130_set_gpio06(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_06, GPIO_06);}
void cp2130_set_gpio07(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_07, GPIO_07);}
void cp2130_set_gpio08(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_08, GPIO_08);}
void cp2130_set_gpio09(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_09, GPIO_09);}
void cp2130_set_gpio10(cp2130_t dev)   {cp2130_set_gpio_values(dev, GPIO_10, GPIO_10);}
void cp2130_reset_gpio00(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_00);}
void cp2130_reset_gpio01(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_01);}
void cp2130_reset_gpio02(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_02);}
void cp2130_reset_gpio03(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_03);}
void cp2130_reset_gpio04(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_04);}
void cp2130_reset_gpio05(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_05);}
void cp2130_reset_gpio06(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_06);}
void cp2130_reset_gpio07(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_07);}
void cp2130_reset_gpio08(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_08);}
void cp2130_reset_gpio09(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_09);}
void cp2130_reset_gpio10(cp2130_t dev) {cp2130_set_gpio_values(dev,       0, GPIO_10);}

#ifdef CP2130_MAIN

#include <unistd.h>

extern void print_dev_info(cp2130_t dev);

int main(int c, char *v[])
{
  int vendor, product, n;
  cp2130_t dev;

  printf("cp2130 main\n");

  vendor  = 0x10c4;
  product = 0x87a0;

  dev = cp2130_open(vendor, product);

  print_dev_info(dev);

  for (n=0; n < 2; n++) {
    printf("LED ON:  Event Counter=%4d\n", cp2130_get_event_counter_count(dev));
    cp2130_reset_gpio07(dev);
    sleep(1);
    printf("LED OFF: Event Counter=%4d\n", cp2130_get_event_counter_count(dev));
    cp2130_set_gpio07(dev);
    sleep(1);
  }

  printf("bye\n");
  cp2130_close(dev);
}

#endif
