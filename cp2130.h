/*
 *      CP2130 API
 *
 *      https://www.silabs.com/documents/public/application-notes/AN792.pdf
 *
 *                                       written by skimu@me.com
 */

#ifndef CP2130_H
#define CP2130_H

typedef struct cp2130 *cp2130_t;

void     cp2130_set_default_wrbufsiz(size_t newbufsiz);
size_t   cp2130_wrbufsiz(cp2130_t dev);
cp2130_t cp2130_open(int vendor_id, int product_id);
void     cp2130_close(cp2130_t dev);

/*
 *       Data transfer Commands
 */

int cp2130_read       (cp2130_t dev, void *buf, int len);
int cp2130_write      (cp2130_t dev, void *buf, int len);
int cp2130_write_read (cp2130_t dev, void *buf, int len);

/*
 *       Config
 */
int  cp2130_get_clock_divider(cp2130_t dev);
void cp2130_get_event_counter(cp2130_t dev, int *mode, int *count);
int  cp2130_get_full_threshold(cp2130_t dev);
void cp2130_get_gpio_chip_select(cp2130_t dev, int *ch, int *pin);
void cp2130_get_gpio_mode_and_level(cp2130_t dev, int *mode, int *level);
int  cp2130_get_gpio_values(cp2130_t dev);
int  cp2130_get_rtr_state(cp2130_t dev);
void cp2130_get_spi_word(cp2130_t dev, unsigned char *buf);
void cp2130_get_spi_delay(cp2130_t dev, int channel,
                          int *mask,
                          int *inter_byte_delay,
                          int *post_assert_delay,
                          int *pre_deassert_delay);
void cp2130_get_readonly_version(cp2130_t dev, int *major, int *minor);

void cp2130_reset_device(cp2130_t dev);
void cp2130_set_clock_divider(cp2130_t dev, int divider);
void cp2130_set_event_counter(cp2130_t dev, int mode, int count);
void cp2130_set_full_threshold(cp2130_t dev, int threshold);
void cp2130_set_gpio_chip_select(cp2130_t dev, int channel, int control);
void cp2130_set_gpio_mode_and_level(cp2130_t dev, int index, int mode, int level);
void cp2130_set_gpio_values(cp2130_t dev, int level, int mask);
void cp2130_set_rtr_stop(cp2130_t dev, int val);
void cp2130_set_spi_word(cp2130_t dev, int channel, int word);
void cp2130_set_spi_delay(cp2130_t dev, int channel,
                          int mask,
                          int inter_byte_delay,
                          int post_assert_delay,
                          int pre_deassert_delay);

/*
 *        OTP
 */
int  cp2130otp_get_lock_byte(cp2130_t dev);
void cp2130otp_get_manufacturing_string(cp2130_t dev, void *str, unsigned size_of_str);
void cp2130otp_get_pin_config(cp2130_t dev, void *buf);
void cp2130otp_get_product_string(cp2130_t dev, void *str, unsigned size_of_str);
void cp2130otp_get_prom_config(cp2130_t dev, int index, void *buf);
void cp2130otp_get_serial_string(cp2130_t dev, void *str, unsigned size_of_str);
void cp2130otp_get_usb_config(cp2130_t dev, void *buf);

void cp2130otp_set_memory_key(cp2130_t dev, int key);

void cp2130otp_set_lock_byte(cp2130_t dev, void *buf);
void cp2130otp_set_manufacturing_string(cp2130_t dev, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_pin_config(cp2130_t dev, void *buf);
void cp2130otp_set_product_string(cp2130_t dev, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_prom_config(cp2130_t dev, int index, void *buf);
void cp2130otp_set_serial_string(cp2130_t dev, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_usb_config(cp2130_t dev, void *buf);

/*
 *       Convenience functions
 */
int  cp2130_get_event_counter_count(cp2130_t dev);
int  cp2130_get_event_counter_mode(cp2130_t dev);
int  cp2130_get_gpio00(cp2130_t dev);
int  cp2130_get_gpio01(cp2130_t dev);
int  cp2130_get_gpio02(cp2130_t dev);
int  cp2130_get_gpio03(cp2130_t dev);
int  cp2130_get_gpio04(cp2130_t dev);
int  cp2130_get_gpio05(cp2130_t dev);
int  cp2130_get_gpio06(cp2130_t dev);
int  cp2130_get_gpio07(cp2130_t dev);
int  cp2130_get_gpio08(cp2130_t dev);
int  cp2130_get_gpio09(cp2130_t dev);
int  cp2130_get_gpio10(cp2130_t dev);
void cp2130_set_gpio00(cp2130_t dev);
void cp2130_set_gpio01(cp2130_t dev);
void cp2130_set_gpio02(cp2130_t dev);
void cp2130_set_gpio03(cp2130_t dev);
void cp2130_set_gpio04(cp2130_t dev);
void cp2130_set_gpio05(cp2130_t dev);
void cp2130_set_gpio06(cp2130_t dev);
void cp2130_set_gpio07(cp2130_t dev);
void cp2130_set_gpio08(cp2130_t dev);
void cp2130_set_gpio09(cp2130_t dev);
void cp2130_set_gpio10(cp2130_t dev);
void cp2130_reset_gpio00(cp2130_t dev);
void cp2130_reset_gpio01(cp2130_t dev);
void cp2130_reset_gpio02(cp2130_t dev);
void cp2130_reset_gpio03(cp2130_t dev);
void cp2130_reset_gpio04(cp2130_t dev);
void cp2130_reset_gpio05(cp2130_t dev);
void cp2130_reset_gpio06(cp2130_t dev);
void cp2130_reset_gpio07(cp2130_t dev);
void cp2130_reset_gpio08(cp2130_t dev);
void cp2130_reset_gpio09(cp2130_t dev);
void cp2130_reset_gpio10(cp2130_t dev);

#endif
