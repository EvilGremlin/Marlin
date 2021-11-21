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

#include <stdint.h>

extern const uint8_t marlin_logo_112x38x1[];
extern const uint8_t marlin_logo_228x255x2[];
extern const uint8_t marlin_logo_228x255x4[];
extern const uint16_t marlin_logo_195x59x16[];
extern const uint16_t marlin_logo_320x240x16[];
extern const uint16_t marlin_logo_480x320x16[];
//starth1
extern const uint8_t back_v1_48x4[];
extern const uint8_t bed_leveling_64x4[];
extern const uint8_t bed_leveling_96x4[];
extern const uint8_t bed_temp_v1_64x4[];
extern const uint8_t chamber_light_96x4[];
extern const uint8_t chamber_temp_64x4[];
extern const uint8_t check_p_64x4[];
extern const uint8_t cogs_2_96x4[];
extern const uint8_t cross_p_64x4[];
extern const uint8_t down_48x4[];
extern const uint8_t fan_v1_64x4[];
extern const uint8_t home_all_64x4[];
extern const uint8_t hotend_temp_64x4[];
extern const uint8_t left_48x4[];
extern const uint8_t machine_prusa_48x4[];
extern const uint8_t machine_prusa_64x4[];
extern const uint8_t move_all_96x4[];
extern const uint8_t printing_flow_64x4[];
extern const uint8_t printing_speed_64x4[];
extern const uint8_t printing_tune_64x4[];
extern const uint8_t printing_tune_96x4[];
extern const uint8_t prism_64x4[];
extern const uint8_t redo_v1_48x4[];
extern const uint8_t right_48x4[];
extern const uint8_t SD_v1_64x4[];
extern const uint8_t SD_v1_96x4[];
extern const uint8_t spool_96x4[];
extern const uint8_t thermo_96x4[];
extern const uint8_t up_48x4[];
//endh1
extern const uint8_t slider_8x16x4[];

enum MarlinImage : uint8_t {
  imgBootScreen = 0x00,
//starth2
  imgBackv148x4,
  imgBedleveling64x4,
  imgBedleveling96x4,
  imgBedtempv164x4,
  imgChamberlight96x4,
  imgChambertemp64x4,
  imgCheckp64x4,
  imgCogs296x4,
  imgCrossp64x4,
  imgDown48x4,
  imgFanv164x4,
  imgHomeall64x4,
  imgHotendtemp64x4,
  imgLeft48x4,
  imgMachineprusa48x4,
  imgMachineprusa64x4,
  imgMoveall96x4,
  imgPrintingflow64x4,
  imgPrintingspeed64x4,
  imgPrintingtune64x4,
  imgPrintingtune96x4,
  imgPrism64x4,
  imgRedov148x4,
  imgRight48x4,
  imgSdv164x4,
  imgSdv196x4,
  imgSpool96x4,
  imgThermo96x4,
  imgUp48x4,
//endh2
  imgSlider,
  imgCount,
  noImage = imgCount,
  imgPageUp = imgLeft48x4,
  imgPageDown = imgRight48x4
};

enum colorMode_t : uint8_t {
  NOCOLORS = 0x00,
  MONOCHROME = 0x01,  // 1 bit per pixel
  GREYSCALE1 = 0x01,  // 1 bit per pixel
  GREYSCALE2,         // 2 bits per pixel
  GREYSCALE4,         // 4 bits per pixel
  HIGHCOLOR           // 16 bits per pixel
};

typedef colorMode_t ColorMode;

typedef struct __attribute__((__packed__)) {
  void *data;
  uint16_t width;
  uint16_t height;
  colorMode_t colorMode;
} tImage;

extern const tImage NoLogo;

#if ENABLED(SHOW_BOOTSCREEN)
  extern const tImage MarlinLogo112x38x1;
  extern const tImage MarlinLogo228x255x2;
  extern const tImage MarlinLogo228x255x4;
  extern const tImage MarlinLogo195x59x16;
  extern const tImage MarlinLogo320x240x16;
  extern const tImage MarlinLogo480x320x16;
#endif
//starth3
extern const tImage Back_v1_48x4;
extern const tImage Bed_leveling_64x4;
extern const tImage Bed_leveling_96x4;
extern const tImage Bed_temp_v1_64x4;
extern const tImage Chamber_light_96x4;
extern const tImage Chamber_temp_64x4;
extern const tImage Check_p_64x4;
extern const tImage Cogs_2_96x4;
extern const tImage Cross_p_64x4;
extern const tImage Down_48x4;
extern const tImage Fan_v1_64x4;
extern const tImage Home_all_64x4;
extern const tImage Hotend_temp_64x4;
extern const tImage Left_48x4;
extern const tImage Machine_prusa_48x4;
extern const tImage Machine_prusa_64x4;
extern const tImage Move_all_96x4;
extern const tImage Printing_flow_64x4;
extern const tImage Printing_speed_64x4;
extern const tImage Printing_tune_64x4;
extern const tImage Printing_tune_96x4;
extern const tImage Prism_64x4;
extern const tImage Redo_v1_48x4;
extern const tImage Right_48x4;
extern const tImage Sd_v1_64x4;
extern const tImage Sd_v1_96x4;
extern const tImage Spool_96x4;
extern const tImage Thermo_96x4;
extern const tImage Up_48x4;
//endh3
extern const tImage Slider8x16x4;

extern const tImage Images[imgCount];
