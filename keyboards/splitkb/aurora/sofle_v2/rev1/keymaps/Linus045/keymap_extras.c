// This file will be appended to the converted keymap.c file
// See compile.sh for more information

#include "action_layer.h"
#include "gpio.h"
#include "host.h"
#include "oled_driver.h"
#include "wpm.h"

#include <stdbool.h>
#include <stdint.h>
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

#define FRAME_DURATION 100

uint32_t current_frame_idx = 0;
static uint16_t timer;
static uint16_t timer_scroll;

int32_t scroll_offsetx = 0;
int32_t scroll_offsety = 0;

void check_scroll_offset_bounds(bool wrapx, bool wrapy) {
  const int y_max = 5;
  const int x_max = 5;

  if (scroll_offsety > y_max) {
    if (wrapy)
      scroll_offsety = 0;
    else
      scroll_offsety = y_max;

  } else if (scroll_offsety < 0) {
    if (wrapy)
      scroll_offsety = y_max;
    else
      scroll_offsety = 0;
  }

  if (scroll_offsetx > x_max) {
    if (wrapx)
      scroll_offsetx = 0;
    else
      scroll_offsetx = x_max;
  } else if (scroll_offsetx < 0) {
    if (wrapx)
      scroll_offsetx = x_max;
    else
      scroll_offsetx = 0;
  }
}

void increase_scroll_offset(const uint32_t valuex, const uint32_t valuey,
                            bool wrapx, bool wrapy) {
  scroll_offsetx += valuex;
  scroll_offsety += valuey;
  check_scroll_offset_bounds(wrapx, wrapy);
}

void decrease_scroll_offset(const uint32_t valuex, const uint32_t valuey,
                            bool wrapx, bool wrapy) {
  scroll_offsetx -= valuex;
  scroll_offsety -= valuey;
  check_scroll_offset_bounds(wrapx, wrapy);
}

void oled_render_logo(void) {

  static const char PROGMEM frame1[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128,
      128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 128,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   192,
      48,  25,  13,  5,   7,   3,   1,   1,   1,   1,   1,   1,   3,   3,   3,
      7,   5,   12,  56,  96,  192, 0,   0,   0,   0,   0,   0,   0,   192, 120,
      7,   1,   0,   0,   0,   0,   56,  60,  124, 126, 126, 126, 126, 126, 124,
      60,  0,   0,   0,   0,   0,   0,   3,   126, 192, 0,   0,   0,   0,   0,
      255, 0,   0,   0,   224, 48,  0,   0,   0,   0,   128, 128, 128, 128, 128,
      128, 0,   0,   0,   32,  96,  192, 0,   0,   0,   0,   255, 0,   0,   0,
      0,   0,   31,  240, 156, 135, 1,   0,   0,   0,   14,  17,  19,  16,  28,
      28,  16,  19,  25,  14,  0,   0,   0,   1,   62,  192, 192, 112, 31,  0,
      0,   0,   0,   0,   0,   127, 0,   1,   3,   2,   6,   4,   4,   252, 4,
      4,   4,   4,   4,   4,   4,   252, 4,   2,   2,   2,   1,   127, 0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   254,
      1,   0,   0,   0,   0,   0,   0,   0,   0,   31,  224, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   128, 128, 192, 192, 192, 224, 224, 224, 248,
      231, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 227, 238, 248,
      192, 192, 192, 192, 192, 128, 128, 7,   3,   3,   3,   1,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   1,   1,   1,   1,   3,   3,   7,   7,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,
  };

  static const char PROGMEM frame2[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128,
      128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 128,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   192,
      48,  25,  13,  5,   7,   3,   1,   1,   1,   1,   1,   1,   3,   3,   3,
      7,   5,   12,  56,  96,  192, 0,   0,   0,   0,   0,   0,   0,   192, 120,
      7,   1,   0,   0,   0,   0,   48,  48,  96,  64,  64,  64,  64,  64,  64,
      96,  48,  16,  0,   0,   0,   0,   3,   126, 192, 0,   0,   0,   0,   0,
      255, 0,   0,   0,   224, 48,  0,   0,   0,   0,   128, 128, 128, 128, 128,
      128, 0,   0,   0,   32,  96,  192, 0,   0,   0,   0,   255, 0,   0,   0,
      0,   0,   31,  240, 156, 135, 1,   0,   0,   0,   14,  17,  19,  16,  28,
      28,  16,  19,  25,  14,  0,   0,   0,   1,   62,  192, 192, 112, 31,  0,
      0,   0,   0,   0,   0,   127, 0,   1,   3,   2,   6,   4,   4,   252, 4,
      4,   4,   4,   4,   4,   4,   252, 4,   2,   2,   2,   1,   127, 0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   254,
      1,   0,   0,   0,   0,   0,   0,   0,   0,   31,  224, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   128, 128, 192, 192, 192, 224, 224, 224, 248,
      231, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 227, 238, 248,
      192, 192, 192, 192, 192, 128, 128, 7,   3,   3,   3,   1,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   1,   1,   1,   1,   3,   3,   7,   7,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,
  };

  static const char PROGMEM frame3[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   128,
      128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 128, 128,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   192,
      48,  25,  13,  5,   7,   3,   1,   1,   1,   1,   1,   1,   3,   3,   3,
      7,   5,   12,  56,  96,  192, 0,   0,   0,   0,   0,   0,   0,   192, 120,
      7,   1,   0,   0,   0,   0,   56,  60,  124, 126, 126, 126, 126, 126, 124,
      60,  0,   0,   0,   0,   0,   0,   3,   126, 192, 0,   0,   0,   0,   0,
      255, 0,   0,   0,   224, 48,  0,   0,   0,   0,   128, 128, 128, 128, 128,
      128, 0,   0,   0,   32,  96,  192, 0,   0,   0,   0,   255, 0,   0,   0,
      0,   0,   31,  240, 156, 135, 1,   0,   0,   0,   14,  17,  19,  16,  28,
      28,  16,  19,  25,  14,  0,   0,   0,   1,   62,  192, 192, 112, 31,  0,
      0,   0,   0,   0,   0,   127, 0,   1,   3,   2,   6,   4,   4,   252, 4,
      4,   4,   4,   4,   4,   4,   252, 4,   2,   2,   2,   1,   127, 0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   254,
      1,   0,   0,   0,   0,   0,   0,   0,   0,   31,  224, 0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   128, 128, 192, 192, 192, 224, 224, 224, 248,
      231, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 227, 238, 248,
      192, 192, 192, 192, 192, 128, 128, 7,   3,   3,   3,   1,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   1,   1,   1,   1,   3,   3,   7,   7,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,
  };

  const char *frames[] = {
      frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1,
      frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1,
      frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1,
      frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1,
      frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1, frame1,
      frame1, frame1, frame2, frame3,
  };
  const uint32_t MAX_FRAMES = sizeof(frames) / sizeof(frames[0]);

  // char text[16];
  // sprintf(text, "%d", );
  // oled_write_P(text, false);

  if (timer_elapsed(timer) > FRAME_DURATION) {
    timer = timer_read();
    current_frame_idx = (current_frame_idx + 1) % MAX_FRAMES;

    oled_clear();
    oled_set_cursor(scroll_offsetx, scroll_offsety);
    oled_write_raw_P(frames[current_frame_idx], sizeof(frame1));
  }

  if (timer_elapsed(timer_scroll) > 25000) {
    timer_scroll = timer_read();
    increase_scroll_offset(0, 1, false, true);
  }

  // Host Keyboard Layer Status
  // oled_write_P(PSTR("Layer: "), false);

  if (scroll_offsety <= 2)
    oled_set_cursor(0, 14);
  else
    oled_set_cursor(0, 0);

  switch (get_highest_layer(layer_state)) {
  case 0:
    // oled_write("\n", false);
    break;
  case 1:
    oled_write("SHIFT", false);
    break;
  case 2:
    oled_write("*SYM*", false);
    break;
  case 3:
    oled_write("ARROW", false);
    break;
  case 4:
    oled_write("MOUSE", false);
    break;
  default:
    // oled_write_ln_P(PSTR("Undefined"), false);
    break;
  }

  // // Host Keyboard LED Status
  // led_t led_state = host_keyboard_led_state();
  // oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  // oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  // oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "),
  // false);
}

bool oled_task_user(void) {
  if (!is_keyboard_master()) {
    oled_render_anim();
  } else {
    oled_render_logo();
    // oled_render_anim();
  }
  return false;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  timer = timer_read();
  timer_scroll = timer_read();
  if (is_keyboard_master()) {
    return rotation;
  }
  return OLED_ROTATION_180;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (get_highest_layer(layer_state)) {
  case 3: {
    if (index == 1) { /* Second encoder */
      if (clockwise) {
        tap_code(KC_RIGHT);
      } else {
        tap_code(KC_LEFT);
      }
    } else if (index == 0) { /* Second encoder */
      if (clockwise) {
        tap_code(KC_UP);
      } else {
        tap_code(KC_DOWN);
      }
    }
    break;
  }
  case 2: {
    if (index == 1) { /* Second encoder */
      if (clockwise) {
        // F13 is interpreted to play the previous song (playerctl previous)
        tap_code(KC_F13);
      } else {
        // F14 is interpreted to play the next song (playerctl next)
        tap_code(KC_F14);
      }
      break;
    }
    // fall through to default otherwise
  }
  default: {
    if (index == 0) { /* First encoder */
      if (clockwise) {
        tap_code(KC_AUDIO_VOL_UP);
        // increase_scroll_offset(1, 0);
      } else {
        tap_code(KC_AUDIO_VOL_DOWN);
        // decrease_scroll_offset(1, 0);
      }
    } else if (index == 1) { /* Second encoder */
      if (clockwise) {
        tap_code(QK_MOUSE_WHEEL_DOWN);
        // oled_clear();
        increase_scroll_offset(0, 1, false, false);
      } else {
        tap_code(QK_MOUSE_WHEEL_UP);
        // oled_clear();
        decrease_scroll_offset(0, 1, false, false);
      }
    }
    break;
  }
  }
  return false;
}
