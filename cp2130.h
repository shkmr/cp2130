/* cp2130 API and Convenience functions */
#ifndef CP2130_H
#define CP2130_H

/*
 *       Config
 */
int  cp2130_get_clock_divider(usbcom_t com);
void cp2130_get_event_counter(usbcom_t com, int *mode, int *count);
int  cp2130_get_full_threshold(usbcom_t com);
void cp2130_get_gpio_chip_select(usbcom_t com, int *ch, int *pin);
void cp2130_get_gpio_mode_and_level(usbcom_t com, int *mode, int *level);
int  cp2130_get_gpio_values(usbcom_t com);
int  cp2130_get_rtr_state(usbcom_t com);
void cp2130_get_spi_word(usbcom_t com, unsigned char *buf);
void cp2130_get_spi_delay(usbcom_t com, int channel,
                          int *mask,
                          int *inter_byte_delay,
                          int *post_assert_delay,
                          int *pre_deassert_delay);
void cp2130_get_readonly_version(usbcom_t com, int *major, int *minor);

void cp2130_reset_device(usbcom_t com);
void cp2130_set_clock_divider(usbcom_t com, int divider);
void cp2130_set_event_counter(usbcom_t com, int mode, int count);
void cp2130_set_full_threshold(usbcom_t com, int threshold);
void cp2130_set_gpio_chip_select(usbcom_t com, int channel, int control);
void cp2130_set_gpio_mode_and_level(usbcom_t com, int index, int mode, int level);
void cp2130_set_gpio_values(usbcom_t com, int level, int mask);
void cp2130_set_rtr_stop(usbcom_t com, int val);
void cp2130_set_spi_word(usbcom_t com, int channel, int word);
void cp2130_set_spi_delay(usbcom_t com, int channel,
                          int mask,
                          int inter_byte_delay,
                          int post_assert_delay,
                          int pre_deassert_delay);

/*
 *        OTP 
 */
int  cp2130otp_get_lock_byte(usbcom_t com);
void cp2130otp_get_manufacturing_string(usbcom_t com, void *str, unsigned size_of_str);
void cp2130otp_get_pin_config(usbcom_t com, void *buf);
void cp2130otp_get_product_string(usbcom_t com, void *str, unsigned size_of_str);
void cp2130otp_get_prom_config(usbcom_t com, int index, void *buf);
void cp2130otp_get_serial_string(usbcom_t com, void *str, unsigned size_of_str);
void cp2130otp_get_usb_config(usbcom_t com, void *buf);

void cp2130otp_set_memory_key(usbcom_t com, int key);

void cp2130otp_set_lock_byte(usbcom_t com, void *buf);
void cp2130otp_set_manufacturing_string(usbcom_t com, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_pin_config(usbcom_t com, void *buf);
void cp2130otp_set_product_string(usbcom_t com, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_prom_config(usbcom_t com, int index, void *buf);
void cp2130otp_set_serial_string(usbcom_t com, void *utf16, unsigned size_of_utf16);
void cp2130otp_set_usb_config(usbcom_t com, void *buf);

/*
 *       Convenience functions
 */
int  cp2130_get_event_counter_count(usbcom_t com);
int  cp2130_get_event_counter_mode(usbcom_t com);
int  cp2130_get_gpio00(usbcom_t com);
int  cp2130_get_gpio01(usbcom_t com);
int  cp2130_get_gpio02(usbcom_t com);
int  cp2130_get_gpio03(usbcom_t com);
int  cp2130_get_gpio04(usbcom_t com);
int  cp2130_get_gpio05(usbcom_t com);
int  cp2130_get_gpio06(usbcom_t com);
int  cp2130_get_gpio07(usbcom_t com);
int  cp2130_get_gpio08(usbcom_t com);
int  cp2130_get_gpio09(usbcom_t com);
int  cp2130_get_gpio10(usbcom_t com);
void cp2130_set_gpio00(usbcom_t com);
void cp2130_set_gpio01(usbcom_t com);
void cp2130_set_gpio02(usbcom_t com);
void cp2130_set_gpio03(usbcom_t com);
void cp2130_set_gpio04(usbcom_t com);
void cp2130_set_gpio05(usbcom_t com);
void cp2130_set_gpio06(usbcom_t com);
void cp2130_set_gpio07(usbcom_t com);
void cp2130_set_gpio08(usbcom_t com);
void cp2130_set_gpio09(usbcom_t com);
void cp2130_set_gpio10(usbcom_t com);
void cp2130_reset_gpio00(usbcom_t com);
void cp2130_reset_gpio01(usbcom_t com);
void cp2130_reset_gpio02(usbcom_t com);
void cp2130_reset_gpio03(usbcom_t com);
void cp2130_reset_gpio04(usbcom_t com);
void cp2130_reset_gpio05(usbcom_t com);
void cp2130_reset_gpio06(usbcom_t com);
void cp2130_reset_gpio07(usbcom_t com);
void cp2130_reset_gpio08(usbcom_t com);
void cp2130_reset_gpio09(usbcom_t com);
void cp2130_reset_gpio10(usbcom_t com);

#endif
