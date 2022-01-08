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

#if HAS_UI_480x320 || HAS_UI_480x272

#include "ui_common.h"

#include "../marlinui.h"
#include "../menu/menu.h"
// #include "../../libs/numtostr.h"

// #include "../../sd/cardreader.h"
// #include "../../module/temperature.h"
// #include "../../module/printcounter.h"
// #include "../../module/planner.h"
// #include "../../module/motion.h"

// #if DISABLED(LCD_PROGRESS_BAR) && BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
//   #include "../../feature/filwidth.h"
//   #include "../../gcode/parser.h"
// #endif

// #if ENABLED(AUTO_BED_LEVELING_UBL)
//   #include "../../feature/bedlevel/bedlevel.h"
// #endif

MotionAxisState motionAxisState;

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    if (TERN0(HAS_TOUCH_SLEEP, lcd_sleep_task())) return;
    if (draw_menu_navigation) {
      add_control(424, 8, BACK, imgBackv148x4);
      add_control(424, 96, PAGE_UP, imgUp48x4, encoderTopLine > 0);
      tft.canvas(447, 160, 2, TFT_HEIGHT - 224);
      tft.set_background(COLOR_LINES);
      add_control(424, TFT_HEIGHT - 56, PAGE_DOWN, imgDown48x4, encoderTopLine + LCD_HEIGHT < screen_items);
      draw_menu_navigation = false;
    }
  #endif

  tft.queue.async();
  TERN_(TOUCH_SCREEN, touch.idle());
}

#if ENABLED(SHOW_BOOTSCREEN)

  void MarlinUI::show_bootscreen() {
    tft.queue.reset();

    tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
    #if ENABLED(BOOT_MARLIN_LOGO_SMALL)
      #define BOOT_LOGO_W 195   // MarlinLogo195x59x16
      #define BOOT_LOGO_H  59
      #define SITE_URL_Y (TFT_HEIGHT - 70)
      tft.set_background(COLOR_BACKGROUND);
    #else
      #define BOOT_LOGO_W TFT_WIDTH   // MarlinLogo480x320x16
      #define BOOT_LOGO_H TFT_HEIGHT
      #define SITE_URL_Y (TFT_HEIGHT - 90)
    #endif
    tft.add_image((TFT_WIDTH - BOOT_LOGO_W) / 2, (TFT_HEIGHT - BOOT_LOGO_H) / 2, imgBootScreen);
    #ifdef WEBSITE_URL
      tft_string.set(WEBSITE_URL);
      tft.add_text(tft_string.center(TFT_WIDTH), SITE_URL_Y, COLOR_WEBSITE_URL, tft_string);
    #endif

    tft.queue.sync();
  }

  void MarlinUI::bootscreen_completion(const millis_t sofar) {
    if ((BOOTSCREEN_TIMEOUT) > sofar) safe_delay((BOOTSCREEN_TIMEOUT) - sofar);
    clear_lcd();
  }

#endif

void MarlinUI::draw_kill_screen() {
  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_KILL_SCREEN_BG);

  uint16_t line = 2;

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  line++;
  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_HALTED));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  tft.queue.sync();
}

void MarlinUI::draw_status_screen() {
  uint16_t x = 24;

  TERN_(TOUCH_SCREEN, touch.clear());

  // status message
  tft.canvas(8, 16, TFT_WIDTH - 16, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_STATUS_MESSAGE, tft_string);

  if (IsStopped) {
    // TODO: will open temperature screen
    tft.canvas(x, 80, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgThermo96x4, COLOR_WHITE);
    TERN_(TOUCH_SCREEN, touch.add_control(HEATER, x, 80, 96, 96));
    x += 112;

    // TODO: will open feeed/fan/etc/tuning screen
    tft.canvas(x, 80, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgPrintingtune96x4, COLOR_ORANGE1);
    TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, x, 80, 96, 96));
    x += 112;

    // REDO: open settings (standard marlin menu)
    tft.canvas(x, 80, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgCogs296x4, COLOR_PURPLE1);
    TERN_(TOUCH_SCREEN, touch.add_control(MENU_MAIN, x, 80, 96, 96));
    x += 112;

    // TODO: open movement screen
    tft.canvas(x, 80, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgMoveall96x4, COLOR_HOTEND);
    TERN_(TOUCH_SCREEN, touch.add_control(MOVE_AXIS, x, 80, 96, 96));

    x = 24;
    // y += TERN(HAS_UI_480x272, 28, 36);

      // open SD card menu
      #if ENABLED(SDSUPPORT)
        tft.canvas(x, 208, 96, 96);
        tft.set_background(COLOR_BACKGROUND);
        tft.add_image(0, 0, imgSdv196x4, COLOR_HOTEND);
        TERN_(TOUCH_SCREEN, touch.add_control(MENU_MEDIA, x, 208, 96, 96)); 
        x += 112;
      #endif

      //TODO: leveling options screen & leveling screens
    #if ENABLED(AUTO_BED_LEVELING_UBL)
        tft.canvas(x, 208, 96, 96);
        tft.set_background(COLOR_BACKGROUND);
        tft.add_image(0, 0, imgBedleveling96x4, COLOR_HOTEND);
        TERN_(TOUCH_SCREEN, touch.add_control(UBL, x, 208, 96, 96)); 
        x += 112;
      #endif

      // TODO: light controls screen
      #if ANY(NEOPIXEL_LED, CASE_LIGHT_ENABLE)
        tft.canvas(x, 208, 96, 96);
        tft.set_background(COLOR_BACKGROUND);
        tft.add_image(0, 0, imgChamberlight96x4, COLOR_HOTEND);
        TERN_(TOUCH_SCREEN, touch.add_control(FAN, x, 208, 96, 96)); 
        x += 112;
      #endif

      // TODO: filament change screen
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        tft.canvas(x, 208, 96, 96);
        tft.set_background(COLOR_BACKGROUND);
        tft.add_image(0, 0, imgSpool96x4, COLOR_HOTEND);
        TERN_(TOUCH_SCREEN, touch.add_control(FLOWRATE, x, 208, 96, 96)); 
        x += 112;
      #endif
  }
  // TODO
  // else if (IsRunning) {}
}

// Low-level draw_edit_screen can be used to draw an edit screen from anyplace
void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char * const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

  uint16_t line = 1;

  menu_line(line++);
  tft_string.set(pstr, itemIndex, itemString);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  TERN_(AUTO_BED_LEVELING_UBL, if (ui.external_control) line++);  // ftostr52() will overwrite *value so *value has to be displayed first

  menu_line(line);
  tft_string.set(value);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    if (ui.external_control) {
      menu_line(line - 1);

      tft_string.set(X_LBL);
      tft.add_text((TFT_WIDTH / 2 - 120), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.x)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 - 16) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

      tft_string.set(Y_LBL);
      tft.add_text((TFT_WIDTH / 2 + 16), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
      tft_string.set(ftostr52(LOGICAL_X_POSITION(current_position.y)));
      tft_string.trim();
      tft.add_text((TFT_WIDTH / 2 + 120) - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
    }
  #endif

  extern screenFunc_t _manual_move_func_ptr;
  if (ui.currentScreen != _manual_move_func_ptr && !ui.external_control) {

    #define SLIDER_LENGTH 336
    #define SLIDER_Y_POSITION 186

    tft.canvas((TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION, SLIDER_LENGTH, 16);
    tft.set_background(COLOR_BACKGROUND);

    int16_t position = (SLIDER_LENGTH - 2) * ui.encoderPosition / maxEditValue;
    tft.add_bar(0, 7, 1, 2, ui.encoderPosition == 0 ? COLOR_SLIDER_INACTIVE : COLOR_SLIDER);
    tft.add_bar(1, 6, position, 4, COLOR_SLIDER);
    tft.add_bar(position + 1, 6, SLIDER_LENGTH - 2 - position, 4, COLOR_SLIDER_INACTIVE);
    tft.add_bar(SLIDER_LENGTH - 1, 7, 1, 2, int32_t(ui.encoderPosition) == maxEditValue ? COLOR_SLIDER : COLOR_SLIDER_INACTIVE);

    #if ENABLED(TOUCH_SCREEN)
      tft.add_image((SLIDER_LENGTH - 8) * ui.encoderPosition / maxEditValue, 0, imgSlider, COLOR_SLIDER);
      touch.add_control(SLIDER, (TFT_WIDTH - SLIDER_LENGTH) / 2, SLIDER_Y_POSITION - 8, SLIDER_LENGTH, 32, maxEditValue);
    #endif
  }

  tft.draw_edit_screen_buttons();
}

void TFT::draw_edit_screen_buttons() {
  #if ENABLED(TOUCH_SCREEN)
    add_control(64, TFT_HEIGHT - 64, DECREASE, imgDown48x4);
    add_control(352, TFT_HEIGHT - 64, INCREASE, imgUp48x4);
    add_control(208, TFT_HEIGHT - 64, CLICK, imgCheckp64x4);
  #endif
}

// The Select Screen presents a prompt and two "buttons"
void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string/*=nullptr*/, PGM_P const suff/*=nullptr*/) {
  uint16_t line = 1;

  if (!string) line++;

  menu_line(line++);
  tft_string.set(pref);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  if (string) {
    menu_line(line++);
    tft_string.set(string);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

  if (suff) {
    menu_line(line);
    tft_string.set(suff);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }
  #if ENABLED(TOUCH_SCREEN)
    add_control(88, TFT_HEIGHT - 64, CANCEL, imgCrossp64x4, true, yesno ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
    add_control(328, TFT_HEIGHT - 64, CONFIRM, imgCheckp64x4, true, yesno ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
  #endif
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE , 0, 0, TFT_WIDTH, TFT_HEIGHT);
    #endif

    menu_line(row);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.wholeDegHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(" / ");
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define GRID_OFFSET_X   8
  #define GRID_OFFSET_Y   8
  #define GRID_WIDTH      192
  #define GRID_HEIGHT     192
  #define CONTROL_OFFSET  16

  void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {

    tft.canvas(GRID_OFFSET_X, GRID_OFFSET_Y, GRID_WIDTH, GRID_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_rectangle(0, 0, GRID_WIDTH, GRID_HEIGHT, COLOR_WHITE);

    for (uint16_t x = 0; x < (GRID_MAX_POINTS_X); x++)
      for (uint16_t y = 0; y < (GRID_MAX_POINTS_Y); y++)
        if (position_is_reachable({ ubl.mesh_index_to_xpos(x), ubl.mesh_index_to_ypos(y) }))
          tft.add_bar(1 + (x * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2, GRID_HEIGHT - 3 - ((y * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 2, 2, COLOR_UBL);

    tft.add_rectangle((x_plot * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2 - 1, GRID_HEIGHT - 5 - ((y_plot * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 6, 6, COLOR_UBL);

    const xy_pos_t pos = { ubl.mesh_index_to_xpos(x_plot), ubl.mesh_index_to_ypos(y_plot) },
                   lpos = pos.asLogical();

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 - MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(X_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.x));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Y_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(ftostr52(lpos.y));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(320, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2 + MENU_ITEM_HEIGHT, 120, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(Z_LBL);
    tft.add_text(0, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
    tft_string.set(isnan(ubl.z_values[x_plot][y_plot]) ? "-----" : ftostr43sign(ubl.z_values[x_plot][y_plot]));
    tft_string.trim();
    tft.add_text(120 - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    constexpr uint8_t w = (TFT_WIDTH) / 10;
    tft.canvas(GRID_OFFSET_X + (GRID_WIDTH - w) / 2, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET - 5, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(x_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    tft.canvas(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET + 16 - 24, GRID_OFFSET_Y + (GRID_HEIGHT - MENU_ITEM_HEIGHT) / 2, w, MENU_ITEM_HEIGHT);
    tft.set_background(COLOR_BACKGROUND);
    tft_string.set(ui8tostr3rj(y_plot));
    tft_string.trim();
    tft.add_text(tft_string.center(w), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + CONTROL_OFFSET,                    UBL,  (ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgUp48x4);
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + GRID_HEIGHT - CONTROL_OFFSET - 32, UBL, -(ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgDown48x4);
      add_control(GRID_OFFSET_X + CONTROL_OFFSET,                   GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL, -(ENCODER_STEPS_PER_MENU_ITEM), imgLeft48x4);
      add_control(GRID_OFFSET_X + GRID_WIDTH - CONTROL_OFFSET - 32, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      UBL,   ENCODER_STEPS_PER_MENU_ITEM, imgRight48x4);
      add_control(320, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET, CLICK, imgBedleveling64x4);
      add_control(224, TFT_HEIGHT - 34, BACK, imgBackv148x4);
    #endif
  }
#endif // AUTO_BED_LEVELING_UBL

static void quick_feedback() {
  #if HAS_CHIRP
    ui.chirp(); // Buzz and wait. Is the delay needed for buttons to settle?
    #if BOTH(HAS_LCD_MENU, USE_BEEPER)
      for (int8_t i = 5; i--;) { buzzer.tick(); delay(2); }
    #elif HAS_LCD_MENU
      delay(10);
    #endif
  #endif
}

static void drawCurStepValue() {
  tft_string.set((uint8_t *)ftostr52sp(motionAxisState.currentStepSize));
  tft_string.add("mm");
  tft.canvas(motionAxisState.stepValuePos.x, motionAxisState.stepValuePos.y, CUR_STEP_VALUE_WIDTH, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(tft_string.center(CUR_STEP_VALUE_WIDTH), 0, COLOR_AXIS_HOMED, tft_string);
}

// TODO: fix this drawing
static void drawCurESelection() {
  tft_string.set("E");
  tft_string.add(ui8tostr3rj(motionAxisState.e_selection));
  tft.canvas(motionAxisState.eNamePos.x, motionAxisState.eNamePos.y, 32, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0, COLOR_ACTIVE , tft_string);
}

// TODO: make it messagebox with timeout and close button
static void drawMessage(uint16_t color, const char *msg) {
  tft.canvas(64, 64, 416, TFT_HEIGHT - 128);
  tft.set_background(color);
  tft.add_text(tft_string.center(240), (TFT_HEIGHT-128)/2-FONT_LINE_HEIGHT/2, COLOR_MENU_TEXT, msg);
}

static void drawAxisValue(const AxisEnum axis) {
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
}

static void moveAxis(const AxisEnum axis, const int8_t direction) {
  quick_feedback();

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(motionAxisState.e_selection)) {
      // drawMessage("Too cold");
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
        // drawMessage(""); // clear the error
        drawAxisValue(axis);
      }
      else {
        // drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
    #elif HAS_BED_PROBE
      // only change probe.offset.z
      probe.offset.z += diff;
      if (direction < 0 && current_position[axis] < Z_PROBE_OFFSET_RANGE_MIN) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MIN;
        // drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
      else if (direction > 0 && current_position[axis] > Z_PROBE_OFFSET_RANGE_MAX) {
        current_position[axis] = Z_PROBE_OFFSET_RANGE_MAX;
        // drawMessage(GET_TEXT(MSG_LCD_SOFT_ENDSTOPS));
      }
      else {
        // drawMessage(""); // clear the error
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
      // drawMessage(msg);
    #endif

    ui.manual_move.soon(axis OPTARG(MULTI_E_MANUAL, motionAxisState.e_selection));
  }

  drawAxisValue(axis);
}

static void e_plus()  { moveAxis(E_AXIS, 1);  }
static void e_minus() { moveAxis(E_AXIS, -1); }
static void x_minus() { moveAxis(X_AXIS, -1); }
static void x_plus()  { moveAxis(X_AXIS, 1);  }
static void y_plus()  { moveAxis(Y_AXIS, 1);  }
static void y_minus() { moveAxis(Y_AXIS, -1); }
static void z_plus()  { moveAxis(Z_AXIS, 1);  }
static void z_minus() { moveAxis(Z_AXIS, -1); }

#if ENABLED(TOUCH_SCREEN)
  static void e_select() {
    motionAxisState.e_selection++;
    if (motionAxisState.e_selection >= EXTRUDERS) {
      motionAxisState.e_selection = 0;
    }

    quick_feedback();
    drawCurESelection();
    drawAxisValue(E_AXIS);
  }

  static void do_home() {
    quick_feedback();
    drawMessage(COLOR_MESSAGEBG, GET_TEXT(MSG_LEVEL_BED_HOMING));
    queue.inject_P(G28_STR);
    // Disable touch until home is done
    TERN_(HAS_TFT_XPT2046, touch.disable());
  }

  static void step_size() {
    motionAxisState.currentStepSize = motionAxisState.currentStepSize / 10.0;
    if (motionAxisState.currentStepSize < 0.0015) motionAxisState.currentStepSize = 10.0;
    quick_feedback();
    drawCurStepValue();
  }
#endif

#if ANY(HAS_BED_PROBE, BABYSTEPPING)
  static void z_select() {
    motionAxisState.z_selection *= -1;
    quick_feedback();
    MarlinImage img = motionAxisState.z_selection == Z_SELECTION_Z_PROBE ? imgBedzoffset64x64x4 : imgMovez64x64x4;
    tft.canvas(motionAxisState.zTypePos.x, motionAxisState.zTypePos.y, 64, 64);
    tft.add_image(0, 0, img, COLOR_ACTIVE, COLOR_BACKGROUND, COLOR_SHADOW);
    drawAxisValue(Z_AXIS);
    tft.queue.sync();
  }
#endif

static void disable_steppers() {
  quick_feedback();
  queue.inject(F("M84"));
}

static void drawBtn(int x, int y, const char *label, intptr_t data, MarlinImage img, uint16_t fgColor, uint16_t bgColor, bool enabled = true) {
  uint16_t width = Images[img].width;
  uint16_t height = Images[img].height;

  if (!enabled) fgColor = COLOR_CONTROL_DISABLED;

  tft.canvas(x, y, width, height);
  tft.set_background(bgColor);
  tft.add_image(0, 0, img, fgColor, bgColor, COLOR_SHADOW);

  // TODO: Make an add_text() take a font arg
  if (label) {
    tft_string.set(label);
    tft_string.trim();
    tft.add_text(tft_string.center(width), height / 2 - tft_string.font_height() / 2, fgColor, tft_string);
  }
  else {
    tft.add_image(0, 0, img, fgColor, bgColor, COLOR_SHADOW);
  }

  TERN_(HAS_TFT_XPT2046, if (enabled) touch.add_control(BUTTON, x, y, width, height, data));
}

void MarlinUI::move_axis_screen() {
  // Reset
  defer_status_screen(true);
  motionAxisState.blocked = false;
  TERN_(HAS_TFT_XPT2046, touch.enable());

  ui.clear_lcd();

  TERN_(TOUCH_SCREEN, touch.clear());

  // Draw controls layout
  // ROW 1: Z+    Y+    E+   Back
  // ROW 2: | X- Home X+ | [    ]
  // ROW 3: Z-    Y-    E- [    ]
  // ROW 4: Z#   Step   E#   Moff

  int x = 8, y = 8;
  drawBtn(x, y, "Z+", (intptr_t)z_plus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step
  x += 128;
  drawBtn(x, y, "Y+", (intptr_t)y_plus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  x += 128;
  drawBtn(x, y, "", (intptr_t)e_minus, imgMoveeup64x64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  TERN_(HAS_TFT_XPT2046, add_control(424, y, BACK, imgBackv148x4));

  tft.canvas(39, 80, 2, 80);
  tft.set_background(COLOR_LINES);
  x = 56; y += 80;
  drawBtn(x, y, "X-", (intptr_t)x_minus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  TERN_(HAS_TFT_XPT2046, add_control(136, y, BUTTON, (intptr_t)do_home, imgHomeall64x4, !busy));
  x += 160;
  drawBtn(x, y, "X+", (intptr_t)x_plus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  tft.canvas(295, 80, 2, 80);
  tft.set_background(COLOR_LINES);
  
  x = 8; y += 80;
  drawBtn(x, y, "Z-", (intptr_t)z_minus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET));
  x += 128;
  drawBtn(x, y, "Y-", (intptr_t)y_minus, imgPrism64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  x += 128;
  drawBtn(x, y, "", (intptr_t)e_plus, imgMoveedown64x64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
  
  x = 8; y += 80;
  #if ANY(HAS_BED_PROBE, BABYSTEPPING)
    motionAxisState.z_selection = busy && ENABLED(BABYSTEPPING) ? Z_SELECTION_Z_PROBE : Z_SELECTION_Z;
    motionAxisState.zTypePos.x = x;
    motionAxisState.zTypePos.y = y;
    touch.add_control(BUTTON, x, y, 64, 64, (intptr_t)z_select);
    z_select();
  #endif
  
  x += 96;
  motionAxisState.stepValuePos.x = x+10;
  motionAxisState.stepValuePos.y = y+16;
  if (!busy) {
    drawBtn(x, y, "", (intptr_t)step_size, imgPrism128x64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
    drawCurStepValue(); 
    }
  
  x += 160;
  #if EXTRUDERS > 1
    motionAxisState.eNamePos.x = x+8;
    motionAxisState.eNamePos.y = y+8;
    if (!busy) {
      drawBtn(x, y, "", (intptr_t)e_select, imgHotend64x64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);
      drawCurESelection(); 
      }
  #endif
  
  x += 136;
  drawBtn(x, y, "", (intptr_t)disable_steppers, imgStepperoff64x64x4, COLOR_ACTIVE, COLOR_BACKGROUND, !busy);

  // Draw axes values display
  x = 344; y = 88;
  tft.canvas(x, y, 32, FONT_LINE_HEIGHT*4);
  tft.set_background(COLOR_BACKGROUND);
  tft.add_text(0, 0,                    COLOR_AXIS_HOMED, "X: ");
  tft.add_text(0, FONT_LINE_HEIGHT,     COLOR_AXIS_HOMED, "Y: ");
  tft.add_text(0, FONT_LINE_HEIGHT * 2, COLOR_AXIS_HOMED, "Z: ");
  tft.add_text(0, FONT_LINE_HEIGHT * 3, COLOR_AXIS_HOMED, "E: ");

  x += 32;
  motionAxisState.xValuePos.x = x;
  motionAxisState.xValuePos.y = y;
  drawAxisValue(X_AXIS);
  
  y += FONT_LINE_HEIGHT;
  motionAxisState.yValuePos.x = x;
  motionAxisState.yValuePos.y = y;
  drawAxisValue(Y_AXIS);

  y += FONT_LINE_HEIGHT;
  motionAxisState.zValuePos.x = x;
  motionAxisState.zValuePos.y = y;
  drawAxisValue(Z_AXIS);
  
  y += FONT_LINE_HEIGHT;
  motionAxisState.eValuePos.x = x;
  motionAxisState.eValuePos.y = y;
  drawAxisValue(E_AXIS);
}

#endif // HAS_UI_480x320
