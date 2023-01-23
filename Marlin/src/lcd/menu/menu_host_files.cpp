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
// Host files menu (M472)
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_MARLINUI_MENU, HOST_FILE_SELECT)

#include "menu_item.h"
#include "../../feature/host_actions.h"

// PGMSTR(SUBS, "$");

// #define HFS_DEBUG
#if ENABLED(HFS_DEBUG)
  PGMSTR(DINIT, "get init page");
  PGMSTR(DSF, "select file 128");
  PGMSTR(DSD, "select directory 64");
  PGMSTR(DPD, "previous directory");
  PGMSTR(DNI, "next item");
  PGMSTR(DUP, "page up");
  PGMSTR(DDN, "page down");
#endif
 
void menu_host_files(){
  // SERIAL_ECHOLNPGM("sizeof hostui.page_data = ", sizeof(hostui.page_data));
  char hfName[PGCOLS];
  if (!hostui.pending) {
    hostui.request_pginit();
    hostui.pending = true;
  }
  
  ui.encoder_direction_menus();
  START_MENU();
  BACK_ITEM(MSG_MAIN); 
  if (ui.should_draw()) for (uint16_t idx=0; idx<PGSIZE; idx += PGCOLS+1) {
    hostui.hfIdx = hostui.page_data[idx];
    strcpy(hfName, &hostui.page_data[idx+1]);

    // if (hostui.hfIdx == 0) SKIP_ITEM(); // if empty
    if (hostui.hfIdx == 1)            // if inside dir
      ACTION_ITEM_F(F(LCD_STR_FOLDER " .."), hostui.request_parent);
    else if WITHIN(hostui.hfIdx, 64, 127)        // if dir
      ACTION_ITEM_S_F(hfName, F(LCD_STR_FOLDER " $"), []{hostui.select_file(ui8tostr3rj(hostui.hfIdx));});
    else if WITHIN(hostui.hfIdx, 128, 254)        // if file
      ACTION_ITEM_S_F(hfName, F("  $"), []{hostui.select_file(ui8tostr3rj(hostui.hfIdx));});
    else SKIP_ITEM();                   // if page end 255
  }
  
  #if ENABLED(HFS_DEBUG)
    ACTION_ITEM_F(F(DINIT),  hostui.request_pginit);
    ACTION_ITEM_F(F(DSF),    []{hostui.select_file(ui8tostr3rj(128));});
    ACTION_ITEM_F(F(DSD),    []{hostui.select_file(ui8tostr3rj(64));});
    ACTION_ITEM_F(F(DDN),    hostui.request_pgdn);
    ACTION_ITEM_F(F(DUP),    hostui.request_pgup);
    ACTION_ITEM_F(F(DPD),    hostui.request_parent);
    ACTION_ITEM_F(F(DNI),    hostui.request_next);
  #endif

  END_MENU();

  if (_menuLineNr == _thisItemNr) hostui.request_pgdn();
  if (ui.encoderPosition == 0) hostui.request_pgup();

  SERIAL_ECHOLNPGM("_menuLineNr = ", _menuLineNr);
  SERIAL_ECHOLNPGM("_thisItemNr = ", _thisItemNr);
  SERIAL_ECHOLNPGM("ui.encoderPosition = ", ui.encoderPosition);
  
}


#endif // BOTH(HAS_MARLINUI_MENU, HOST_FILE_SELECT)


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