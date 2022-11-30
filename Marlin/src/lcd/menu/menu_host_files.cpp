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

#if ENABLED(HOST_FILE_SELECT)

#include "menu_item.h"
#include "../../feature/host_actions.h"

// PGMSTR(SUBS, "$");

#if ENABLED(DEBUG_HOST_FILE)
  PGMSTR(DINIT, "get init page");
  PGMSTR(DSF, "select file 128");
  PGMSTR(DSD, "select directory 64");
  PGMSTR(DPD, "previous directory");
  PGMSTR(DNI, "next item");
  PGMSTR(DUP, "page up");
  PGMSTR(DDN, "page down");
#endif
 
bool drawn = false;
bool can_scroll_up, can_scroll_down, can_scroll = false;
char hfName[PGCOLS];
uint8_t hfIdx, current_page = 0;

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

void clear_data(){
  memset(hostui.page_data, 0x00, PGSIZE);
}

void menu_host_files(){
  
  ui.encoder_direction_menus();
  START_MENU();
  if (current_page > 1){ 
    ACTION_ITEM_F(F(LCD_STR_UPLEVEL), hostui.request_pgup)
  }
  else BACK_ITEM(MSG_MAIN); 

  // if (!drawn) {
    if (!hostui.pending) {
      hostui.request_pginit();
      hostui.pending = true;
      current_page = 1;
    }
    else if (ui.should_draw()) for (uint16_t idx=0; idx<(PGSIZE-5); idx += (PGCOLS+1)) {
    
      hfIdx = hostui.page_data[idx];
      memcpy(hfName, &hostui.page_data[idx+1], PGCOLS);

      DEBUG_ECHOLNPGM("Drawing... idx=", ui8tostr3rj(hfIdx), "  name=", hfName);

      // if (hfIdx == 0) SKIP_ITEM(); // if empty
      if (hfIdx == 1)            // if inside dir
        ACTION_ITEM_F(F(LCD_STR_FOLDER " .."), hostui.request_parent);
      else if WITHIN(hfIdx, 64, 127)        // if dir
        ACTION_ITEM_S_F(hfName, F(LCD_STR_FOLDER " $"), []{hostui.select_file(ui8tostr3rj(hfIdx));});
      else if WITHIN(hfIdx, 128, 254)        // if file
        ACTION_ITEM_S_F(hfName, F("  $"), []{hostui.select_file(ui8tostr3rj(hfIdx));});
      else break;                   // if page end or null
    }

  // }

  
  #if ENABLED(DEBUG_HOST_FILE)
    // ACTION_ITEM_F(F(DINIT),  hostui.request_pginit);
    ACTION_ITEM_F(F(DSF),    []{hostui.select_file(ui8tostr3rj(128));});
    ACTION_ITEM_F(F(DSD),    []{hostui.select_file(ui8tostr3rj(64));});
    // ACTION_ITEM_F(F(DDN),    hostui.request_pgdn);
    // ACTION_ITEM_F(F(DUP),    hostui.request_pgup);
    // ACTION_ITEM_F(F(DPD),    hostui.request_parent);
    // ACTION_ITEM_F(F(DNI),    hostui.request_next);
  #endif

  END_MENU();

  // if (encoderLine > 0)
  // if (encoderLine == 0 && current_page > 1 ) current_page -=1, hostui.request_pgup();
  if (_menuLineNr == _thisItemNr) current_page += 1, hostui.request_pgdn();

  // if (encoderLine > 0) can_scroll_up, can_scroll_down = true;
  // if (encoderLine == 0 && can_scroll_up) {can_scroll_up = false; hostui.request_pgup();}
  // if (_menuLineNr == _thisItemNr && can_scroll_down) {can_scroll_down = false; hostui.request_pgdn();}
  DEBUG_ECHOLNPGM("menuLineNr=", _menuLineNr, "  thisItemNr=", _thisItemNr, "  encPos=", ui.encoderPosition);
  DEBUG_ECHOLNPGM("encTLine=", encoderTopLine, "  encLine=", encoderLine, " scr_itms=", screen_items);


  
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
