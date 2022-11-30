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

#if ENABLED(HOST_FILE_SELECT)

#include "menu_item.h"
#include "../../feature/host_actions.h"

PGMSTR(SUBS, "$");
PGMSTR(MINIT, "get init page");
PGMSTR(MUP, "page up");
PGMSTR(MDN, "page down");
PGMSTR(MSF, "select file");

// static void hfStartPrint(const uint8_t hfidx) {
//   #if ENABLED(HOST_FILE_CONFIRM_START)
// //     MenuItem_submenu::action(fstr, []{
// //       char * const longest = card.longest_filename();
// //       char buffer[strlen(longest) + 2];
// //       buffer[0] = ' ';
// //       strcpy(buffer + 1, longest);
// //       MenuItem_confirm::select_screen(
// //         GET_TEXT_F(MSG_BUTTON_PRINT), GET_TEXT_F(MSG_BUTTON_CANCEL),
// //         hostui.start, nullptr,
// //         GET_TEXT_F(MSG_START_PRINT), buffer, F("?")
// //       );
// //     });
//   #else
//     hostui.start();
//     UNUSED(fstr);
//   #endif
// }

void menu_host_files(){
  // ui.encoder_direction_menus();
  START_MENU();
  // #if (PGROWS) > 5
    BACK_ITEM(MSG_MAIN);  // save screen space
  // #endif 

  bool last_blink = false;
  // memset(&hostui.page_data, 0x20, PGSIZE);
  hostui.request_pginit();

  if (last_blink != ui.get_blink()) {
    last_blink = ui.get_blink();
    if (hostui.page_full){
      if (ui.should_draw()){
        for (int idx=0; idx<=PGSIZE; idx += PGCOLS+1){
          int hfIdx = hostui.page_data[idx];
          char hfName = hostui.page_data[idx+1];
          ACTION_ITEM_S_F(FTOP(hfName), F(SUBS), []{hostui.select_file(ui8tostr3rj(25));});
        }
      }
    }
    // else SKIP_ITEM();

  }

  // if (ui.should_draw()) 
    // for (uint8_t i = 0; i < PGLEN; i++) {
    
    // if (_menuLineNr == _thisItemNr) {
      
      // hostui.request_next();

      // if (hfIdx == 0)
      //   ACTION_ITEM_F(F(LCD_STR_FOLDER " .."), hostui.request_parent);   // return to previous dir
      // else if ((PGLEN) < 6)
      //   BACK_ITEM(MSG_MAIN);

      // if (hfIdx < 128)
        // ACTION_ITEM_S_F(hfName, F(LCD_STR_FOLDER Language_en::MSG_PID_P), []{hostui.select_file(ui8tostr3rj(hfIdx));});
      // else if (hfIdx < 255)
      // ACTION_ITEM_S_F(hostui.hfName, F(SUBS), []{hostui.select_file(ui8tostr3rj(hostui.hfIdx));});

      // hostui.host_item_received = false;
    
    // }
  // }

  // ACTION_ITEM(MSF,    []{hostui.select_file(ui8tostr3rj(hfIdx));});
  // ACTION_ITEM(MINIT,  hostui.request_pginit);
  // ACTION_ITEM(MUP,    hostui.request_pgup);
  // ACTION_ITEM(MDN,    hostui.request_pgdn);

  END_MENU();
}




#endif // HAS_MARLINUI_MENU && HOST_FILE_SELECT