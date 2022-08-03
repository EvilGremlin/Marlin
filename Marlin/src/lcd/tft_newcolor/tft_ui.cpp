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
#include "../../inc/MarlinConfigPre.h"

#include "ui_common.h"
#include "tft_ui.h"
#include "../marlinui.h"
#include "../../gcode/queue.h"
#include "../../module/planner.h"
#include "../../module/temperature.h"
#include "../menu/menu.h"
#include "../../libs/numtostr.h"

const bool blink = MarlinUI::get_blink();

void TFTui::quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if BOTH(HAS_MARLINUI_MENU, HAS_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_MARLINUI_MENU
      delay(10);
    #endif
  #endif
}

void TFTui::disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

void TFTui::drawBtn(int x, int y, const char *label, intptr_t data, MarlinImage img, uint16_t fgColor, uint16_t bgColor, bool enabled) {
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
void TFTui::drawAxisValue(const AxisEnum axis) {
  const float value = (
    TERN_(HAS_BED_PROBE, axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? probe.offset.z :)
    ui.manual_move.axis_value(axis)
  );
  xy_int_t pos;
  bool not_homed = axis_should_home(axis);
  switch (axis) {
    case X_AXIS: pos = motionAxisState.xValuePos; break;
    case Y_AXIS: pos = motionAxisState.yValuePos; break;
    case Z_AXIS: pos = motionAxisState.zValuePos; break;
    case E_AXIS: pos = motionAxisState.eValuePos; break;
    default: return;
  }
  tft.canvas(pos.x, pos.y, 80, FONT_LINE_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(blink && not_homed ? "?" : ftostr52sign(value));
  tft.add_text(0, 0, not_homed ? COLOR_AXIS_NOT_HOMED : COLOR_AXIS_HOMED, tft_string);

  if (!ui.manual_move.processing) {
  // Get motion limit from software endstops, if any
  float min, max;
  soft_endstop.get_manual_axis_limits(axis, min, max);
  }
}

// DONE
void TFTui::drawCurStepValue() {
  tft_string.set(ftostr52sp(motionAxisState.currentStepSize));
  tft_string.add(F("mm"));
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, 104, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(CUR_STEP_VALUE_WIDTH), 0, COLOR_AXIS_HOMED, tft_string);
}

// DONE
void TFTui::drawCurESelection() {
  tft_string.set("*", motionAxisState.e_selection);
  tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, 32, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_CONTROL_ENABLED , tft_string);
  tft.queue.sync();
}

// TODO: make it messagebox with timeout and close button ?
void TFTui::drawMessage(PGM_P const msg) {
  tft.canvas(64, 64, 416, TFT_HEIGHT - 128);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(240), (TFT_HEIGHT-128)/2-FONT_LINE_HEIGHT/2, COLOR_MENU_TEXT, msg);
}

// TODO: messages
void TFTui::moveAxis(const AxisEnum axis, const int8_t direction) {
  quick_feedback();

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(motionAxisState.e_selection)) {
      drawMessage(F("Too cold"));
      return;
    }
  #endif

  const float diff = motionAxisState.currentStepSize * direction;

  if (axis == Z_AXIS && motionAxisState.z_selection == Z_SELECTION_Z_PROBE) {
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      const int16_t babystep_increment = direction * BABYSTEP_SIZE_Z;
      const bool do_probe = DISABLED(BABYSTEP_HOTEND_Z_OFFSET) || active_extruder == 0;
      const float bsDiff = planner.mm_per_step[Z_AXIS] * babystep_increment,
                  new_probe_offset = probe.offset.z + bsDiff,
                  new_offs = TERN(BABYSTEP_HOTEND_Z_OFFSET
                    , do_probe ? new_probe_offset : hotend_offset[active_extruder].z - bsDiff
                    , new_probe_offset
                  );
      if (WITHIN(new_offs, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
        babystep.add_steps(Z_AXIS, babystep_increment);
        if (do_probe)
          probe.offset.z = new_offs;
        else
          TERN(BABYSTEP_HOTEND_Z_OFFSET, hotend_offset[active_extruder].z = new_offs, NOOP);
        drawMessage(NUL_STR); // clear the error
        drawAxisValue(axis);
      }
      else {
        drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
      }
    #elif HAS_BED_PROBE
      // only change probe.offset.z
      probe.offset.z += diff;
      if (direction < 0 && current_position[axis] < Z_PROBE_OFFSET_RANGE_MIN) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MIN;
        drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
      }
      else if (direction > 0 && current_position[axis] > Z_PROBE_OFFSET_RANGE_MAX) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MAX;
        drawMessage(GET_TEXT_F(MSG_LCD_SOFT_ENDSTOPS));
      }
      else {
        drawMessage(NUL_STR); // clear the error
      }
      drawAxisValue(axis);
    #endif
    return;
  }

  if (!ui.manual_move.processing) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits(axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z_AXIS && axis != E_AXIS) {
        max = SQRT(sq((float)(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y_AXIS - axis])); // (Y_AXIS - axis) == the other axis
        min = -max;
      }
    #endif

    // Get the new position
    const bool limited = ui.manual_move.apply_diff(axis, diff, min, max);
    #if IS_KINEMATIC
      UNUSED(limited);
    #else
      PGM_P const msg = limited ? GET_TEXT(MSG_LCD_SOFT_ENDSTOPS) : NUL_STR;
      drawMessage(msg);
    #endif

    ui.manual_move.soon(axis OPTARG(MULTI_E_MANUAL, motionAxisState.e_selection));
  }

  drawAxisValue(axis);
}

void TFTui::e_select() {
  motionAxisState.e_selection++;
  if (motionAxisState.e_selection >= EXTRUDERS) {
    motionAxisState.e_selection = 0;
  }
  quick_feedback();
  drawCurESelection();
  drawAxisValue(E_AXIS);
}

#if ANY(HAS_BED_PROBE, BABYSTEPPING)
  void TFTui::z_select() {
    motionAxisState.z_selection *= -1;
    quick_feedback();
    MarlinImage img = motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? imgBedzoffset64x64x4 : imgMovez64x64x4;
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, 64, 64);
    tft.add_image(0, 0, img, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, COLOR_BLACK);
    drawAxisValue(Z_AXIS);
    tft.queue.sync();
  }
#endif

void TFTui::do_home() {
  quick_feedback();
  drawMessage(GET_TEXT_F(MSG_LEVEL_BED_HOMING));
  queue.inject_P(G28_STR);
  // Disable touch until home is done
  TERN_(TOUCH_SCREEN, touch.disable());
}

void TFTui::step_size() {
  motionAxisState.currentStepSize = motionAxisState.currentStepSize / 10.0;
  if (motionAxisState.currentStepSize < 0.0015) motionAxisState.currentStepSize = 10.0;
  quick_feedback();
  drawCurStepValue();
}

void TFTui::menu_line(const uint8_t row, uint16_t color) {
  cursor.set(0, row);
  tft.canvas(0, TFT_TOP_LINE_Y + cursor.y * MENU_LINE_HEIGHT, TFT_WIDTH - 64, MENU_ITEM_HEIGHT);
  tft.set_background(color);
}

void TFTui::menu_item(const uint8_t row, bool sel ) {
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


#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE , 0, 0, TFT_WIDTH, TFT_HEIGHT);
    #endif

    tftui.menu_line(row);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.wholeDegHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(F(" / "));
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

#endif // ADVANCED_PAUSE_FEATURE