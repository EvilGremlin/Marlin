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

#pragma once

#include "../../inc/MarlinConfigPre.h"
#include "../../MarlinCore.h"
#include "tft.h"


#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1
class TFTui {
public:
    // static void screen_movement();

    static void drawBtn(int x, int y, const char *label, intptr_t data, bool is_screen, MarlinImage img, uint16_t fgColor, uint16_t bgColor, bool enabled);
    static void drawMessage(PGM_P const msg);
    static void drawMessage(FSTR_P const fmsg) { drawMessage(FTOP(fmsg)); }

    static void drawAxisValue(const AxisEnum axis);
    static void drawCurStepValue();
    static void drawCurESelection();

    static void moveAxis(const AxisEnum axis, const int8_t direction);
    static void e_plus()  { moveAxis(E_AXIS,  1); }
    static void e_minus() { moveAxis(E_AXIS, -1); }
    static void x_minus() { moveAxis(X_AXIS, -1); }
    static void x_plus()  { moveAxis(X_AXIS,  1); }
    static void y_plus()  { moveAxis(Y_AXIS,  1); }
    static void y_minus() { moveAxis(Y_AXIS, -1); }
    static void z_plus()  { moveAxis(Z_AXIS,  1); }
    static void z_minus() { moveAxis(Z_AXIS, -1); }
    static void e_select();
    static void z_select();
    static void do_home();
    static void step_size();
    static void quick_feedback();
    static void disable_steppers();
    static void draw_fan_status(uint16_t x, uint16_t y, const bool blink);
    static void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
    static void menu_item(const uint8_t row, bool sel = false);
};

static TFTui tftui;

struct MotionAxisState {
      xy_int_t xValuePos, yValuePos, zValuePos, eValuePos, stepValuePos, zTypePos, eNamePos;
      float currentStepSize = 10.0;
      int z_selection = Z_SELECTION_Z;
      uint8_t e_selection = 0;
      bool blocked = false;
      char message[32];
      };
static MotionAxisState motionAxisState;

static xy_uint_t cursor;
void menu_pause_option();
const bool busy = printingIsActive();