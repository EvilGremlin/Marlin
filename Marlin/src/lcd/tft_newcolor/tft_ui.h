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
#include "../marlinui.h"
#include "../../gcode/queue.h"
#include "../menu/menu.h"
#include "tft.h"
#include "tft_image.h"
#include "../../MarlinCore.h"

#if HAS_BED_PROBE
  #include "../../module/probe.h"
#endif
#if ENABLED(TOUCH_SCREEN)
  #include "touch.h"
  extern bool draw_menu_navigation;
#endif

#define Z_SELECTION_Z 1
#define Z_SELECTION_Z_PROBE -1

typedef struct MotionAxisState {
        xy_int_t xValuePos, yValuePos, zValuePos, eValuePos, stepValuePos, zTypePos, eNamePos;
        float currentStepSize = 10.0;
        int z_selection = Z_SELECTION_Z;
        uint8_t e_selection = 0;
        bool blocked = false;
        char message[32];};

class TFTui;
extern TFTui tftui;

class TFTui {
public:
    MotionAxisState motionAxisState;

    static void screen_movement();

    static void drawBtn(int x, int y, const char *label, intptr_t data, MarlinImage img, uint16_t fgColor, uint16_t bgColor, bool enabled = true);
    static void drawAxisValue(const AxisEnum axis);
    static void drawMessage(PGM_P const msg);
    static void drawMessage(FSTR_P const fmsg);

    static void draw_heater_status(uint16_t x, uint16_t y, const int8_t Heater);
    static void draw_fan_status(uint16_t x, uint16_t y, const bool blink);

    void menu_line(const uint8_t row, uint16_t color=COLOR_BACKGROUND);
    void menu_item(const uint8_t row, bool sel = false);

    static void quick_feedback();
    static void disable_steppers();
    static void drawMessage(PGM_P const msg);
    static void drawMessage(FSTR_P const fmsg) { drawMessage(FTOP(fmsg)); };

};

void menu_pause_option();
const bool blink = MarlinUI::get_blink();
const bool busy = printingIsActive();

