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
 *  Usage: M472 I1
 *         M472 I64 this_is_directory
 *         M472 I128 benchy.gcode
 *         M472 I255
 * 
 *        1  - previous directory
 *  64..127  - directory indexes
 * 128..254  - file indexes
 *      255  - end of page
 * 
 * Action commands:
 * //action:host_file <char>
 *    1..63  - number of entries in page, this serve as first (top) page request
 *        P  - go to parent directory (if any)
 *        U  - page down (show next group of files/directories) 
 *        D  - page up
 *        +  - next entry 
 *  64..127  - open directory
 * 128..254  - open file and report back full filename for confirmation dialog, max filname length is 254 symbols
 */

uint8_t i, j = 0;
uint16_t idx = 0;


void GcodeSuite::M472() {

  if (parser.seenval('I')) {
    i = parser.value_byte();    
    if(i != j && hostui.pending) {    // get menu items array
      hostui.page_data[idx] = parser.value_byte();
      if (parser.value_byte() == 1) return;   // avoid crash on empty string_arg
      else if (parser.value_byte() == 255 || idx >= (PGSIZE-1)) {
        if (parser.value_byte() == 255) DEBUG_ECHO_MSG(">>>caught 255");
        else if (idx > (PGSIZE-2)) DEBUG_ECHO_MSG(">>>caught overflow");
        j = 0, idx = 0;
        // hostui.pending = false;
        ui.refresh();
        // DEBUG_ECHO_MSG(">>>refresh called");
      }
      else if (parser.string_arg[0]) {
        strncpy(&hostui.page_data[idx+1], parser.string_arg, PGCOLS); // for menu, name is cut down to screen size
      }
      
      DEBUG_ECHOLNPGM("idx=", ui16tostr3rj(idx), "  page_data@idx=", ui8tostr3rj(hostui.page_data[idx]), " ", &hostui.page_data[idx+1]);
      j = i, idx += (PGCOLS+1);
    }
  #if ENABLED(HOST_FILE_CONFIRM_START)
    else if (!hostui.pending) {       // get full filename for confirmation dialog
      strncpy(&hostui.hfFullName[0], parser.string_arg, HFFLEN); 
    }
  #endif
  }

  // DEBUG_ECHOLNPGM("", );
  // DEBUG_ECHO_MSG(">>>");

}

#endif // HOST_FILE_SELECT
