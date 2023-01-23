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

#include "../../inc/MarlinConfig.h"

#if ENABLED(HOST_FILE_SELECT)

#include "../gcode.h"
#include "../../feature/host_actions.h"
#include "../../lcd/menu/menu.h"

/*
 * M472: Send host filename with index
 *  
 *  I<integer> - 8-bit integer index
 * 
 *  Usage: M472 I222 benchy.gcode
 * 
 * For memory efficiency sake, we use uint8_t indexes for everything
 *        1  - previous directory
 *  64..127  - directory indexes
 * 128..254  - file indexes
 *      255  - end of page
 * 
 * Action commands:
 * //action:host_file <char>
 *    1..63  - number of entries in page (LCD_HEIGHT-1), this serve as first (top) page request
 *        P  - go to parent directory (if any)
 *        U  - page down (show next group of files/directories) 
 *        D  - page up
 *        +  - next entry 
 *  64..127  - open directory
 * 128..254  - open file and report back full filename for confirmation dialog
 */

static uint8_t i, j = 0;
static uint16_t idx = 0;

void GcodeSuite::M472() {

  if (parser.seenval('I')) {
    i = parser.value_byte();
    if(i != j && hostui.pending) {

      hostui.page_data[idx] = parser.value_byte();

      if (parser.value_byte() == 255 || idx >= PGSIZE-1) {
        j = 0, idx = 0, hostui.pending = false;
        ui.refresh();
      }
      else if (parser.string_arg[0]) {
        strncpy(&hostui.page_data[idx+1], parser.string_arg, PGCOLS);  // not scrolling filename
      }
      else return;
      
      SERIAL_ECHOLNPGM("idx=", idx, "  page_data@idx=", &hostui.page_data[idx]);
      
      j = i, idx += PGCOLS+1;
    } 
  }

  // SERIAL_ECHOLNPGM("page_data0: ", F(&hostui.page_data));
  // SERIAL_ECHOLNPGM("page_data1: ", F(&hostui.page_data[PGCOLS+1]));
  // SERIAL_ECHOLNPGM("page_data2: ", F(&hostui.page_data[(PGCOLS+1)*2]));
  // SERIAL_ECHOLNPGM("page_data3: ", F(&hostui.page_data[(PGCOLS+1)*3]));
  // SERIAL_ECHOLNPGM("page_data4: ", F(&hostui.page_data[(PGCOLS+1)*4]));
}

#endif // HOST_FILE_SELECT
