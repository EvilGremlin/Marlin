/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "tft_ui.h"

static xy_uint_t cursor;

void quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if BOTH(HAS_MARLINUI_MENU, HAS_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_MARLINUI_MENU
      delay(10);
    #endif
  #endif
}

void disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

void drawBtn(int x, int y, const char *label, intptr_t data, MarlinImage img, uint16_t fgColor, uint16_t bgColor, bool enabled = true) {
  uint16_t width = Images[img].width;
  uint16_t height = Images[img].height;

  if (!enabled) fgColor = COLOR_CONTROL_DISABLED;

  tft.canvas(x, y, width, height);
  tft.set_background(bgColor);
  tft.add_image(0, 0, img, fgColor, bgColor, COLOR_BLACK);

  if (label) {
    tft_string.set(label);
    tft_string.trim();
    tft.add_text(tft_string.center(width), height / 2 - tft_string.font_height() / 2, fgColor, tft_string);
  }
  else {
    tft.add_image(0, 0, img, fgColor, bgColor, COLOR_BLACK);
  }

  TERN_(TOUCH_SCREEN, if (enabled) touch.add_control(BUTTON, x, y, width, height, data));
}

// TODO: add font n stuff
void drawAxisValue(const AxisEnum axis) {
  const float value = (
    TERN_(HAS_BED_PROBE, axis == Z_AXIS && tftui.motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? probe.offset.z :)
    ui.manual_move.axis_value(axis)
  );
  xy_int_t pos;
  bool not_homed = axis_should_home(axis);
  switch (axis) {
    case X_AXIS: pos = tftui.motionAxisState.xValuePos; break;
    case Y_AXIS: pos = tftui.motionAxisState.yValuePos; break;
    case Z_AXIS: pos = tftui.motionAxisState.zValuePos; break;
    case E_AXIS: pos = tftui.motionAxisState.eValuePos; break;
    default: return;
  }
  tft.canvas(pos.x, pos.y, 80, FONT_LINE_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(blink && not_homed ? "?" : ftostr52sign(value));
  tft.add_text(0, 0, not_homed ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);
}

// TODO: make it messagebox with timeout and close button ?
void drawMessage(PGM_P const msg) {
  tft.canvas(64, 64, 416, TFT_HEIGHT - 128);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(240), (TFT_HEIGHT-128)/2-FONT_LINE_HEIGHT/2, COLOR_MENU_TEXT, msg);
}

void menu_line(const uint8_t row, uint16_t color) {
  cursor.set(0, row);
  tft.canvas(0, TFT_TOP_LINE_Y + cursor.y * MENU_LINE_HEIGHT, TFT_WIDTH - 64, MENU_ITEM_HEIGHT);
  tft.set_background(color);
}

void menu_item(const uint8_t row, bool sel ) {
  #if ENABLED(TOUCH_SCREEN)
    if (row == 0) {
      touch.clear();
      draw_menu_navigation = TERN(ADVANCED_PAUSE_FEATURE, ui.currentScreen != menu_pause_option, true);
    }
  #endif

  menu_line(row, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
  #if ENABLED(TOUCH_SCREEN)
    const TouchControlType tct = TERN(SINGLE_TOUCH_NAVIGATION, true, sel) ? MENU_CLICK : MENU_ITEM;
    touch.add_control(tct, 0, TFT_TOP_LINE_Y + row * MENU_LINE_HEIGHT, TFT_WIDTH - 64, MENU_ITEM_HEIGHT, encoderTopLine + row);
  #endif
}