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

#if HAS_GRAPHICAL_TFT

#include "tft_image.h"
#include "ui_common.h"

const tImage NoLogo                 = { nullptr, 0, 0, NOCOLORS };

#if ENABLED(SHOW_BOOTSCREEN)
  const tImage MarlinLogo112x38x1   = { (void *)marlin_logo_112x38x1, 112, 38, GREYSCALE1 };
  const tImage MarlinLogo228x255x2  = { (void *)marlin_logo_228x255x2, 228, 255, GREYSCALE2 };
  const tImage MarlinLogo228x255x4  = { (void *)marlin_logo_228x255x4, 228, 255, GREYSCALE4 };
  const tImage MarlinLogo195x59x16  = { (void *)marlin_logo_195x59x16,  195,  59, HIGHCOLOR };
  const tImage MarlinLogo320x240x16 = { (void *)marlin_logo_320x240x16, 320, 240, HIGHCOLOR };
  const tImage MarlinLogo480x320x16 = { (void *)marlin_logo_480x320x16, 480, 320, HIGHCOLOR };
#endif
//startcpp1
const tImage Back_v1_48x4         = { (void *)back_v1_48x4, 48, 48, GREYSCALE4 };
const tImage Bed_leveling_64x4         = { (void *)bed_leveling_64x4, 64, 64, GREYSCALE4 };
const tImage Bed_leveling_96x4         = { (void *)bed_leveling_96x4, 96, 96, GREYSCALE4 };
const tImage Bed_temp_v1_64x4         = { (void *)bed_temp_v1_64x4, 64, 64, GREYSCALE4 };
const tImage Bed_z_offset_64x64x4         = { (void *)bed_z_offset_64x64x4, 64, 64, GREYSCALE4 };
const tImage Chamber_light_96x4         = { (void *)chamber_light_96x4, 96, 96, GREYSCALE4 };
const tImage Chamber_temp_64x4         = { (void *)chamber_temp_64x4, 64, 64, GREYSCALE4 };
const tImage Check_p_64x4         = { (void *)check_p_64x4, 64, 64, GREYSCALE4 };
const tImage Cogs_2_96x4         = { (void *)cogs_2_96x4, 96, 96, GREYSCALE4 };
const tImage Cross_p_64x4         = { (void *)cross_p_64x4, 64, 64, GREYSCALE4 };
const tImage Down_48x4         = { (void *)down_48x4, 48, 48, GREYSCALE4 };
const tImage Fan_v1_64x4         = { (void *)fan_v1_64x4, 64, 64, GREYSCALE4 };
const tImage Home_all_64x4         = { (void *)home_all_64x4, 64, 64, GREYSCALE4 };
const tImage Hotend_64x64x4         = { (void *)hotend_64x64x4, 64, 64, GREYSCALE4 };
const tImage Hotend_temp_64x4         = { (void *)hotend_temp_64x4, 64, 64, GREYSCALE4 };
const tImage Left_48x4         = { (void *)left_48x4, 48, 48, GREYSCALE4 };
const tImage Machine_prusa_48x4         = { (void *)machine_prusa_48x4, 48, 48, GREYSCALE4 };
const tImage Machine_prusa_64x4         = { (void *)machine_prusa_64x4, 64, 64, GREYSCALE4 };
const tImage Minus_p_48x48x4         = { (void *)minus_p_48x48x4, 48, 48, GREYSCALE4 };
const tImage Move_all_96x4         = { (void *)move_all_96x4, 96, 96, GREYSCALE4 };
const tImage Move_e_down_64x64x4         = { (void *)move_E_down_64x64x4, 64, 64, GREYSCALE4 };
const tImage Move_e_up_64x64x4         = { (void *)move_E_up_64x64x4, 64, 64, GREYSCALE4 };
const tImage Move_z_64x64x4         = { (void *)move_Z_64x64x4, 64, 64, GREYSCALE4 };
const tImage Ok_p_48x48x4         = { (void *)ok_p_48x48x4, 48, 48, GREYSCALE4 };
const tImage Plus_p_48x48x4         = { (void *)plus_p_48x48x4, 48, 48, GREYSCALE4 };
const tImage Printing_flow_64x4         = { (void *)printing_flow_64x4, 64, 64, GREYSCALE4 };
const tImage Printing_speed_64x4         = { (void *)printing_speed_64x4, 64, 64, GREYSCALE4 };
const tImage Printing_tune_64x4         = { (void *)printing_tune_64x4, 64, 64, GREYSCALE4 };
const tImage Printing_tune_96x4         = { (void *)printing_tune_96x4, 96, 96, GREYSCALE4 };
const tImage Prism_128x64x4         = { (void *)prism_128x64x4, 128, 64, GREYSCALE4 };
const tImage Prism_64x4         = { (void *)prism_64x4, 64, 64, GREYSCALE4 };
const tImage Redo_v1_48x4         = { (void *)redo_v1_48x4, 48, 48, GREYSCALE4 };
const tImage Right_48x4         = { (void *)right_48x4, 48, 48, GREYSCALE4 };
const tImage Sd_v1_64x4         = { (void *)SD_v1_64x4, 64, 64, GREYSCALE4 };
const tImage Sd_v1_96x4         = { (void *)SD_v1_96x4, 96, 96, GREYSCALE4 };
const tImage Spool_96x4         = { (void *)spool_96x4, 96, 96, GREYSCALE4 };
const tImage Stepper_off_64x64x4         = { (void *)stepper_off_64x64x4, 64, 64, GREYSCALE4 };
const tImage Thermo_96x4         = { (void *)thermo_96x4, 96, 96, GREYSCALE4 };
const tImage Up_48x4         = { (void *)up_48x4, 48, 48, GREYSCALE4 };
//endcpp1
const tImage Slider8x16x4           = { (void *)slider_8x16x4, 8, 16, GREYSCALE4 };

const tImage Images[imgCount] = {
  TERN(SHOW_BOOTSCREEN, TERN(BOOT_MARLIN_LOGO_SMALL, MarlinLogo195x59x16, MARLIN_LOGO_FULL_SIZE), NoLogo),
//startcpp2
  Back_v1_48x4,
  Bed_leveling_64x4,
  Bed_leveling_96x4,
  Bed_temp_v1_64x4,
  Bed_z_offset_64x64x4,
  Chamber_light_96x4,
  Chamber_temp_64x4,
  Check_p_64x4,
  Cogs_2_96x4,
  Cross_p_64x4,
  Down_48x4,
  Fan_v1_64x4,
  Home_all_64x4,
  Hotend_64x64x4,
  Hotend_temp_64x4,
  Left_48x4,
  Machine_prusa_48x4,
  Machine_prusa_64x4,
  Minus_p_48x48x4,
  Move_all_96x4,
  Move_e_down_64x64x4,
  Move_e_up_64x64x4,
  Move_z_64x64x4,
  Ok_p_48x48x4,
  Plus_p_48x48x4,
  Printing_flow_64x4,
  Printing_speed_64x4,
  Printing_tune_64x4,
  Printing_tune_96x4,
  Prism_128x64x4,
  Prism_64x4,
  Redo_v1_48x4,
  Right_48x4,
  Sd_v1_64x4,
  Sd_v1_96x4,
  Spool_96x4,
  Stepper_off_64x64x4,
  Thermo_96x4,
  Up_48x4,
//endcpp2
  Slider8x16x4,
};

#endif // HAS_GRAPHICAL_TFT
