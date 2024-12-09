// This file will be appended to the converted keymap.c file
// See compile.sh for more information

#include "gpio.h"

// Disable the Power LED
void keyboard_pre_init_user(void) {
  gpio_set_pin_output(24);
  gpio_write_pin_high(24);
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   // Handle Ctrl LED state
//   switch (keycode) {
//   case KC_SPC:
//     if (record->event.pressed) {
//       gpio_write_pin_low(24);
//     } else {
//       gpio_write_pin_high(24);
//     }
//     break;
//   }
//   return true;
// }

// layer_state_t layer_state_set_user(layer_state_t state) {
//   if (layer_state_cmp(state, 2)) {
//     gpio_write_pin_low(24);
//   } else {
//     gpio_write_pin_high(24);
//   }
//   return state;
// }
