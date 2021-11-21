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

#define RED(color)            ((color >> 8) & 0xF8)
#define GREEN(color)          ((color >> 3) & 0xFC)
#define BLUE(color)           ((color << 3) & 0xF8)
#define RGB(red, green, blue) (((red << 8) & 0xF800) | ((green << 3) & 0x07E0) | ((blue >> 3) & 0x001F))
#define COLOR(color)          RGB(((color >> 16) & 0xFF), ((color >> 8) & 0xFF), (color & 0xFF))
#define HALF(color)           RGB(RED(color) >> 1, GREEN(color) >> 1, BLUE(color) >> 1)

// 16 bit color generator: https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
// RGB565 color picker:  https://trolsoft.ru/en/articles/rgb565-color-picker

#define COLOR_GREEN1          0x1B09  // #1E6251
#define COLOR_GREEN2          0x2A88  // #2F5149
#define COLOR_BLUE1           0x2A6F  // #314F83
#define COLOR_BLUE2           0x428D  // #45546F
#define COLOR_PURPLE1         0x916E  // #9C3079
#define COLOR_PURPLE2         0x7A4D  // #814B6F
#define COLOR_RED1            0xB9C8  // #52459A
#define COLOR_RED2            0x9ACB  // #A25D61
#define COLOR_ORANGE1         0xBCC7  // #C49B3B
#define COLOR_ORANGE2         0x9C4B  // #A28D5D
#define COLOR_YELLOW1         0xBD87  // #C4B53B
#define COLOR_YELLOW2         0x9CCB  // #A29B5D
#define COLOR_GREY1           0x39E7  // #404040
#define COLOR_GREY2           0x52CA  // #5A5A5A
#define COLOR_GREY3           0x7BEF  // #808080
#define COLOR_WHITE           0xFFFF  // #FFFFFF
#define COLOR_BLACK           0x0000  // #000000


#define COLOR_BACKGROUND        COLOR_GREEN1
#define COLOR_SHADOW            COLOR_BLACK
#define COLOR_LINES             COLOR_GREY2
#define COLOR_SELECTION_BG      COLOR_BLUE1
#define COLOR_WEBSITE_URL       COLOR_YELLOW1

#define COLOR_ACTIVE            COLOR_WHITE
#define COLOR_INACTIVE          COLOR_GREY2
#define COLOR_COLD              COLOR_WHITE
#define COLOR_HOTEND            COLOR_RED1
#define COLOR_HEATED_BED        COLOR_RED1
#define COLOR_CHAMBER           COLOR_WHITE
#define COLOR_COOLER            COLOR_WHITE
#define COLOR_FAN               COLOR_WHITE

#define COLOR_AXIS_HOMED        COLOR_WHITE
#define COLOR_AXIS_NOT_HOMED    COLOR_PURPLE2
#define COLOR_RATE_100          COLOR_ORANGE1
#define COLOR_RATE_ALTERED      COLOR_PURPLE1

#define COLOR_PRINT_TIME        COLOR_ORANGE1
#define COLOR_PROGRESS_FRAME    COLOR_WHITE
#define COLOR_PROGRESS_BAR      COLOR_ORANGE1
#define COLOR_PROGRESS_BG       COLOR_BLUE1
#define COLOR_STATUS_MESSAGE    COLOR_ORANGE1

#define COLOR_CONTROL_ENABLED   COLOR_WHITE
#define COLOR_CONTROL_DISABLED  COLOR_GREY2
#define COLOR_CONTROL_CANCEL    COLOR_RED1
#define COLOR_CONTROL_CONFIRM   COLOR_ORANGE1

#define COLOR_BUSY              COLOR_GREY2
#define COLOR_MENU_TEXT         COLOR_WHITE
#define COLOR_MENU_VALUE        COLOR_YELLOW2
#define COLOR_SLIDER            COLOR_ORANGE1
#define COLOR_SLIDER_INACTIVE   COLOR_ORANGE2

#define COLOR_UBL               COLOR_WHITE

#define COLOR_TOUCH_CALIBRATION COLOR_WHITE

#define COLOR_KILL_SCREEN_BG    COLOR_RED2
#define COLOR_KILL_SCREEN_TEXT  COLOR_GREY1
