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
#include "tft_ui.h"
#include "../marlinui.h"
#include "../../MarlinCore.h"
#include "../menu/menu.h"

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    if (TERN0(HAS_TOUCH_SLEEP, lcd_sleep_task())) return;
    if (draw_menu_navigation) {
      add_control(424, 8, BACK, imgBackv148x4);
      add_control(424, 96, PAGE_UP, imgUp48x4, encoderTopLine > 0);
      tft.canvas(447, 160, 2, TFT_HEIGHT - 224);
      tft.set_background(COLOR_SLIDER);
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

  tftui.menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  line++;
  tftui.menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_HALTED));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  tftui.menu_line(line++, COLOR_KILL_SCREEN_BG);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET));
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  tft.queue.sync();
}

// INPROGRESS
void MarlinUI::draw_status_screen() {
  uint16_t x = 24, y = 80;

  TERN_(TOUCH_SCREEN, touch.clear());

  // status message
  tft.canvas(8, 16, TFT_WIDTH - 16, 32);
  tft.set_background(COLOR_BACKGROUND);
  tft_string.set(status_message);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_STATUS_MESSAGE, tft_string);

  if (IsStopped) {
    // TODO: will open temperature screen
    // tftui.drawBtn(x, y, "", HEATER, imgThermo96x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, true);
    tft.canvas(x, y, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgThermo96x4, COLOR_CONTROL_ENABLED);
    TERN_(TOUCH_SCREEN, touch.add_control(HEATER, x, 80, 96, 96));
    x += 112;

    // TODO: will open feed/fan/etc tuning screen
    // tftui.drawBtn(x, y, "", FEEDRATE, imgPrintingtune96x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
    tft.canvas(x, y, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgPrintingtune96x4, COLOR_CONTROL_ENABLED);
    TERN_(TOUCH_SCREEN, touch.add_control(FEEDRATE, x, 80, 96, 96));
    x += 112;

    // REDO: open settings (standard marlin menu)
    // tftui.drawBtn(x, y, "", (intptr_t)ui.goto_screen(menu_main), imgCogs296x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
    tft.canvas(x, y, 96, 96);
    tft.set_background(COLOR_BACKGROUND);
    tft.add_image(0, 0, imgCogs296x4, COLOR_CONTROL_ENABLED);
    TERN_(TOUCH_SCREEN, touch.add_control(MENU_MAIN, x, 80, 96, 96));
    x += 112;

    //DONE Open movement screen
    tftui.drawBtn(x, y, "", (intptr_t)tftui.screen_movement, imgMoveall96x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
    // tft.canvas(x, y, 96, 96);
    // tft.set_background(COLOR_BACKGROUND);
    // tft.add_image(0, 0, imgMoveall96x4, COLOR_CONTROL_ENABLED);
    // TERN_(TOUCH_SCREEN, touch.add_control(SCREEN_MOVE_AXIS, x, 80, 96, 96));

    x = 24;
    y = TERN(HAS_UI_480x272, 192, 208);

    // Open SD card menu
    #if ENABLED(SDSUPPORT)
      // drawBtn(x, y, "", (intptr_t)e_plus, imgMoveedown64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
      tft.canvas(x, y, 96, 96);
      tft.set_background(COLOR_BACKGROUND);
      tft.add_image(0, 0, imgSdv196x4, COLOR_CONTROL_ENABLED);
      TERN_(TOUCH_SCREEN, touch.add_control(MENU_MEDIA, x, 208, 96, 96)); 
      x += 112;
    #endif

      //TODO: leveling options screen & leveling screens
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      // drawBtn(x, y, "", (intptr_t)e_plus, imgMoveedown64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
      tft.canvas(x, y, 96, 96);
      tft.set_background(COLOR_BACKGROUND);
      tft.add_image(0, 0, imgBedleveling96x4, COLOR_CONTROL_ENABLED);
      TERN_(TOUCH_SCREEN, touch.add_control(SCREEN_UBL, x, 208, 96, 96)); 
      x += 112;
    #endif

    // TODO: LED controls screen
    #if ANY(LED_CONTROL_MENU, CASE_LIGHT_MENU)
      // drawBtn(x, y, "", (intptr_t)e_plus, imgMoveedown64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
      // tft.canvas(x, y, 96, 96);
      // tft.set_background(COLOR_BACKGROUND);
      // tft.add_image(0, 0, imgChamberlight96x4, COLOR_CONTROL_ENABLED);
      // TERN_(TOUCH_SCREEN, touch.add_control(SCREEN_LED, x, 208, 96, 96)); 
      x += 112;
    #endif

    // TODO: filament change screen
    #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      // drawBtn(x, y, "", (intptr_t)e_plus, imgMoveedown64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
      // tft.canvas(x, y, 96, 96);
      // tft.set_background(COLOR_BACKGROUND);
      // tft.add_image(0, 0, imgSpool96x4, COLOR_CONTROL_ENABLED);
      // TERN_(TOUCH_SCREEN, touch.add_control(SCREEN_FILCHANGE, x, 208, 96, 96)); 
      x += 112;
    #endif
  }
  // TODO
  // else if (IsRunning) {}
}

// TODO
// Low-level draw_edit_screen can be used to draw an edit screen from anyplace
void MenuEditItemBase::draw_edit_screen(FSTR_P const fstr, const char * const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

  uint16_t line = 1;

  tftui.menu_line(line++);
  tft_string.set(fstr, itemIndex, itemStringC, itemStringF);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);

  TERN_(AUTO_BED_LEVELING_UBL, if (ui.external_control) line++);  // ftostr52() will overwrite *value so *value has to be displayed first

  tftui.menu_line(line);
  tft_string.set(value);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    if (ui.external_control) {
      tftui.menu_line(line - 1);

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

  if (ui.can_show_slider()) {

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

// TODO
void TFT::draw_edit_screen_buttons() {
  #if ENABLED(TOUCH_SCREEN)
    add_control(64, TFT_HEIGHT - 64, DECREASE, imgDown48x4);
    add_control(352, TFT_HEIGHT - 64, INCREASE, imgUp48x4);
    add_control(208, TFT_HEIGHT - 64, CLICK, imgCheckp64x4);
  #endif
}

// TODO
// The Select Screen presents a prompt and two "buttons"
void MenuItem_confirm::draw_select_screen(FSTR_P const yes, FSTR_P const no, const bool yesno, FSTR_P const pref, const char * const string/*=nullptr*/, FSTR_P const suff/*=nullptr*/) {
  uint16_t line = 1;

  if (!string) line++;

  tftui.menu_line(line++);
  tft_string.set(pref);
  tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);

  if (string) {
    tftui.menu_line(line++);
    tft_string.set(string);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

  if (suff) {
    tftui.menu_line(line);
    tft_string.set(suff);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }
  #if ENABLED(TOUCH_SCREEN)
    if (no)  add_control( 88, TFT_HEIGHT - 64, CANCEL,  imgCrossp64x4,  true, yesno ? HALF(COLOR_CONTROL_CANCEL) : COLOR_CONTROL_CANCEL);
    if (yes) add_control(328, TFT_HEIGHT - 64, CONFIRM, imgCheckp64x4, true, yesno ? COLOR_CONTROL_CONFIRM : HALF(COLOR_CONTROL_CONFIRM));
  #endif
}

// TODO
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
        if (position_is_reachable({ bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y) }))
          tft.add_bar(1 + (x * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2, GRID_HEIGHT - 3 - ((y * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 2, 2, COLOR_UBL);

    tft.add_rectangle((x_plot * 2 + 1) * (GRID_WIDTH - 4) / (GRID_MAX_POINTS_X) / 2 - 1, GRID_HEIGHT - 5 - ((y_plot * 2 + 1) * (GRID_HEIGHT - 4) / (GRID_MAX_POINTS_Y) / 2), 6, 6, COLOR_UBL);

    const xy_pos_t pos = { bedlevel.get_mesh_x(x_plot), bedlevel.get_mesh_y(y_plot) },
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
    tft_string.set(isnan(bedlevel.z_values[x_plot][y_plot]) ? "-----" : ftostr43sign(bedlevel.z_values[x_plot][y_plot]));
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
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + CONTROL_OFFSET,                    SCREEN_UBL,  (ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgUp48x4);
      add_control(GRID_OFFSET_X + GRID_WIDTH + CONTROL_OFFSET,      GRID_OFFSET_Y + GRID_HEIGHT - CONTROL_OFFSET - 32, SCREEN_UBL, -(ENCODER_STEPS_PER_MENU_ITEM) * (GRID_MAX_POINTS_X), imgDown48x4);
      add_control(GRID_OFFSET_X + CONTROL_OFFSET,                   GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      SCREEN_UBL, -(ENCODER_STEPS_PER_MENU_ITEM), imgLeft48x4);
      add_control(GRID_OFFSET_X + GRID_WIDTH - CONTROL_OFFSET - 32, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET,      SCREEN_UBL,   ENCODER_STEPS_PER_MENU_ITEM, imgRight48x4);
      add_control(320, GRID_OFFSET_Y + GRID_HEIGHT + CONTROL_OFFSET, CLICK, imgBedleveling64x4);
      add_control(224, TFT_HEIGHT - 34, BACK, imgBackv148x4);
    #endif
  }
#endif // AUTO_BED_LEVELING_UBL

// DONE
void TFTui::screen_movement() {
  // Reset
  ui.defer_status_screen(true);
  motionAxisState.blocked = false;
  TERN_(TOUCH_SCREEN, touch.enable());
  ui.clear_lcd();
  TERN_(TOUCH_SCREEN, touch.clear());

  // Draw controls layout
  // ROW 1: Z+    Y+    E+   Back
  // ROW 2: | X- Home X+ | [    ]
  // ROW 3: Z-    Y-    E- [    ]
  // ROW 4: Z#   Step   E#   Moff

  int x = 8, y = 8;
  tftui.drawBtn(x, y, "Z+", (intptr_t)tftui.z_plus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET)); //only enabled when not busy or have baby step
  x += 128;
  tftui.drawBtn(x, y, "Y+", (intptr_t)tftui.y_plus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  x += 128;
  tftui.drawBtn(x, y, "", (intptr_t)tftui.e_minus, imgMoveeup64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  tftui.drawBtn(424, y, "", (intptr_t)ui.goto_previous_screen, imgBackv148x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, true);

  tft.canvas(39, 80, 2, 80);
  tft.set_background(COLOR_WHITE);
  x = 56; y += 80;
  tftui.drawBtn(x, y, "X-", (intptr_t)tftui.x_minus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  x += 80;
  tftui.drawBtn(x, y, "", (intptr_t)do_home, imgHomeall64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  x += 80;
  tftui.drawBtn(x, y, "X+", (intptr_t)tftui.x_plus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  tft.canvas(295, 80, 2, 80);
  tft.set_background(COLOR_WHITE);
  
  x = 8, y += 80;
  tftui.drawBtn(x, y, "Z-", (intptr_t)tftui.z_minus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy || ENABLED(BABYSTEP_ZPROBE_OFFSET));
  x += 128;
  tftui.drawBtn(x, y, "Y-", (intptr_t)tftui.y_minus, imgPrism64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  x += 128;
  tftui.drawBtn(x, y, "", (intptr_t)tftui.e_plus, imgMoveedown64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
  
  x = 8, y += 80;
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
    tftui.drawBtn(x, y, "", (intptr_t)step_size, imgPrism128x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
    drawCurStepValue(); 
    }
  
  x += 160;
  #if EXTRUDERS > 1
    motionAxisState.eNamePos.x = x+8;
    motionAxisState.eNamePos.y = y+8;
    if (!busy) {
      tftui.drawBtn(x, y, "", (intptr_t)e_select, imgHotend64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);
      drawCurESelection(); 
      }
  #endif
  
  x += 136;
  tftui.drawBtn(x, y, "", (intptr_t)tftui.disable_steppers, imgStepperoff64x64x4, COLOR_CONTROL_ENABLED, COLOR_BACKGROUND, !busy);

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
  tftui.drawAxisValue(X_AXIS);
  
  y += FONT_LINE_HEIGHT;
  motionAxisState.yValuePos.x = x;
  motionAxisState.yValuePos.y = y;
  tftui.drawAxisValue(Y_AXIS);

  y += FONT_LINE_HEIGHT;
  motionAxisState.zValuePos.x = x;
  motionAxisState.zValuePos.y = y;
  tftui.drawAxisValue(Z_AXIS);
  
  y += FONT_LINE_HEIGHT;
  motionAxisState.eValuePos.x = x;
  motionAxisState.eValuePos.y = y;
  tftui.drawAxisValue(E_AXIS);
}

#endif // HAS_UI_480x320
