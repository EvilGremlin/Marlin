/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// SD Card Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_MARLINUI_MENU, HOST_FILE_SELECT)

#include "menu_item.h"
#include "../../feature/host_actions.h"

void menu_host_files(){

  static uint8_t id = 25;

  START_MENU();
  BACK_ITEM(MSG_MAIN);
  // ACTION_ITEM(MINIT,  hostui.request_pginit);
  ACTION_ITEM(MINIT,  hostui.request_pginit);
  ACTION_ITEM(MUP,    hostui.request_pgup);
  ACTION_ITEM(MDN,    hostui.request_pgdn);
  ACTION_ITEM(MSF,    []{hostui.select_file(ui8tostr3rj(id));});


  END_MENU();
}

#endif // HAS_MARLINUI_MENU && HOST_FILE_SELECT