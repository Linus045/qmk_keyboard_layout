// This file will be appended to the converted keymap.c file
// See compile.sh for more information

#include "gpio.h"

// Disable the Power LED
void keyboard_pre_init_user(void) {
  gpio_set_pin_output(24);
  gpio_write_pin_high(24);
}
