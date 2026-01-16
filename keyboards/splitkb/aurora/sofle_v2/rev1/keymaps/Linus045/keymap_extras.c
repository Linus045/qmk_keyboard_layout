// This file will be appended to the converted keymap.c file
// See compile.sh for more information

#include "action_layer.h"
#include "gpio.h"
#include "host.h"
#include "oled_driver.h"
#include "wpm.h"
#include <stdio.h>

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

// bool oled_task_user(void) {
//   // Host Keyboard Layer Status
//   oled_write_P(PSTR("Layer: "), false);

//   switch (get_highest_layer(layer_state)) {
//   case 1:
//     oled_write_P(PSTR("Default\n"), false);
//     break;
//   case 2:
//     oled_write_P(PSTR("FN\n"), false);
//     break;
//   case 3:
//     oled_write_P(PSTR("ADJ\n"), false);
//     break;
//   default:
//     // Or use the write_ln shortcut over adding '\n' to the end of your
//     oled_write_P(PSTR("Undefined"), false);
//   }

//   // // Host Keyboard LED Status
//   // led_t led_state = host_keyboard_led_state();
//   // oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
//   // oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
//   // oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "),
//   false);

//   return false;
// }

/**
 * Put this somewhere at the beginning of the file --
 * Make sure you import only one of animations at a time
 * They all have same function exported, so it won't compile if you
 * include more than one at a time. You can also configure some options
 * before including the animation. Not all animations support them, but some do
 * :P.
 */
#define ANIM_INVERT true
#define ANIM_RENDER_WPM true
#define FAST_TYPE_WPM 45 // Switch to fast animation when over words per minute

#include "bars_animation.c"

void oled_render_logo(void) {
  // Host Keyboard Layer Status
  // oled_write_P(PSTR("Layer: "), false);

  switch (get_highest_layer(layer_state)) {
  case 0:
    oled_write_P(PSTR("\n"), false);
    break;
  case 1:
    oled_write_P(PSTR("SHIFT\n"), false);
    break;
  case 2:
    oled_write_P(PSTR("*SYM*\n"), false);
    break;
  case 3:
    oled_write_P(PSTR("ARROW\n"), false);
    break;
  case 4:
    oled_write_P(PSTR("MOUSE\n"), false);
    break;
  default:
    // Or use the write_ln shortcut over adding '\n' to the end of your string
    oled_write_ln_P(PSTR("Undefined"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}

bool oled_task_user(void) {
  if (!is_keyboard_master()) {
    oled_render_anim();
  } else {
    oled_render_logo();
  }
  return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_keyboard_master()) {
    return rotation;
  }
  return OLED_ROTATION_180;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { /* First encoder */
    if (clockwise) {
      tap_code(KC_AUDIO_VOL_UP);
    } else {
      tap_code(KC_AUDIO_VOL_DOWN);
    }
  } else if (index == 1) { /* Second encoder */
    if (clockwise) {
      tap_code(QK_MOUSE_WHEEL_DOWN);
    } else {
      tap_code(QK_MOUSE_WHEEL_UP);
    }
  }
  return false;
}
