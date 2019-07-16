/* -*- mode: C; compile-command: "make test_cp2130ek"; -*- */
/*
 *      CP2130EK
 *
 *      https://www.silabs.com/documents/public/user-guides/CP2130-EK.pdf
 *
 *      Si8902
 *
 *      https://www.silabs.com/documents/public/data-sheets/Si8900-1-2.pdf
 *
 *      Serial EEPROM  25AA02E48-I/SN
 *
 *      http://ww1.microchip.com/downloads/en/DeviceDoc/20002123F.pdf
 *
 *      Temp Sensor   MPC9701AT-E/OT
 *
 *      http://ww1.microchip.com/downloads/en/devicedoc/20001942g.pdf
 *
 *                                       written by skimu@me.com
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include "cp2130.h"

int ek_read_adc(cp2130_t ek, int ain)
{
  unsigned char buf[3];
  int adc;

  if ( (ain > 2) || (ain < 0) ) return -1;

  /*
   *     AIN=0   Potentiometer.
   *     AIN=1   Tempsensor.
   *     AIN=2   GPIO.5 (clock divier)
   */

  /*
   *      CNFG_0
   */
  buf[0] = ((ain<<4)&0x30);
  buf[0] |= 0xc0;   /* bit7,6 : has to be 1 */
  buf[0] |= 0x08;   /* bit3   1: VDD as VREF,    0: External     */
  buf[0] |= 0x02;   /* bit1   1: ADC Demand mode 0: Burst mode   */
  buf[0] |= 0x01;   /* bit0   1: PGA gain 1      0: PGA Gain 0.5 */

  buf[1] = 0;   /* ADC_H */
  buf[2] = 0;   /* ADC_L */

  cp2130_set_gpio_chip_select(ek, 0, 2);
  cp2130_write(ek, buf, 1);
  usleep(8);
  cp2130_read(ek, buf, 3);
  cp2130_set_gpio_chip_select(ek, 0, 0);

  if ( ((buf[1]>>4)&0x03) != ain)
    warnx("got ain different from input.  given=%d, got=%d", ain, (buf[1]>>4)&0x03);

  adc = ((buf[1]&0x0f)<<6) + ((buf[2]&0x7e)>>1);

  return adc;
}

void ek_init(cp2130_t ek)
{
  /*
   *       Si8902 (10bit ADC) is on CS0.
   *       CPHA=Trailing Edge, CPOL=Active Low
   *       SCK should be less than 2MHz.
   *
   *       7654 3210
   *       0011 1011 = 0x3b
   */
  cp2130_set_spi_word(ek, 0, 0x3b);

  /*
   *       Reset ADC by driving GPIO.7.
   */
  cp2130_reset_gpio07(ek); /* Assert RST on ADC (Active low) */
  usleep(500);
  cp2130_set_gpio07(ek);   /* Negate RST on ADC */
}


void print_bar(int n)
{
  n = n/10;
  while (n-- > 0)
    printf(" ");
  printf("*\n");
}

int main(int c, char *v[])
{
  void print_dev_info(cp2130_t dev);
  cp2130_t ek;

  printf("cp2130 main\n");

  ek  = cp2130_open(0x10c4, 0x87a0);

  ek_init(ek);

  print_dev_info(ek);

  while (1) {
    print_bar(ek_read_adc(ek, 0));
    usleep(100000);
  }

  printf("bye\n");
  cp2130_close(ek);
  return 0;
}

/* EOF */
