#!/usr/bin/env python3

# Script to convert RGB(A) bitmaps into tricky 4-bit grayscale byte array with alpha mask
# 0x0 to 0x7 is antialiased shadows
# 0x8 is alpha mask
# 0x9 to 0xF is antialiased foreground
# transparent pixels in bitmap must be within 120...136 range (set to 128 in source)

import os
import fileinput

gpltext = '''/**\r\n \
* Marlin 3D Printer Firmware\r\n \
* Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]\r\n \
*\r\n \
* This program is free software: you can redistribute it and/or modify\r\n \
* it under the terms of the GNU General Public License as published by\r\n \
* the Free Software Foundation, either version 3 of the License, or\r\n \
* (at your option) any later version.\r\n \
*\r\n \
* This program is distributed in the hope that it will be useful,\r\n \
* but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n \
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\r\n \
* GNU General Public License for more details.\r\n \
*\r\n \
* You should have received a copy of the GNU General Public License\r\n \
* along with this program.  If not, see <https://www.gnu.org/licenses/>.\r\n \
**/\r\n\r\n'''

def getSize(filename):
    st = os.stat(filename)
    return st.st_size    

cpp1 = []
cpp2 = []    
h1 = []
h2 = []
h3 = []

with fileinput.input(mode='rb') as flist:
    for c in flist:
        # get every Nth byte of RGB(A) bitmap into array
        strip = bytearray()
        bit4 = []
        width = 0 
        height = 0
        cf = fileinput.filename()
        with open(cf, mode='rb') as f:
            f.seek(18)
            width = int.from_bytes(f.read(4), 'little') # read width from header
            f.seek(22)
            height = int.from_bytes(f.read(4), 'little') # read height from header
            f.seek(28)
            bpp = [4 if int.from_bytes(f.read(1), 'little')==32 else 3] # read bits per pixel from header
            bpp = int(''.join(str(i) for i in bpp))
            f.seek(10)
            dstart = int.from_bytes(f.read(4), 'little') # find bitmap data offset
            i = dstart-2                                
            while i <= getSize(cf):
                i += bpp           # read bytes, image must be monochrome for it all to work right
                f.seek(i)
                strip += bytearray(f.read(1))
        
        # floor divide every byte 16 to get 4-bit hex value
        j=0
        while j <= len(strip)-1:
            a = strip[j]
            # find transparent pixels
            bit4.append(hex(8 if a in range(120, 136) else a//16)) # snoop around 0x8 because why it would be easy?
            j += 1
        hexstrip = "".join(bit4).replace("0x","")

        # write .cpp with output byte array
        name = os.path.basename(cf)[0:-4] + 'x4' # construct name
        outfile = os.path.relpath(cf)[0:-4] + 'x4'  + '.cpp'
        with open(outfile, mode='w', encoding='utf-8') as of:
            of.write(gpltext)
            of.write('''#include "../../../inc/MarlinConfigPre.h"\r\n\r\n#if HAS_GRAPHICAL_TFT\r\n\r\n''')
            of.write('extern const uint8_t ' + name + '''[''' + str(len(hexstrip)//2) + '''] = {\r\n''')

            split1 = [hexstrip[i : i+width] for i in range(0, len(hexstrip), width)] # cut into lines
            split2 = [[0 for i in range(width)] for j in range(height)] # init empty array
            split2 = [[split1[i][j:j+2] for j in range(0, width, 2)] for i in range( 0, height)] # cut into pixels
            split2.reverse() # because bmp is backwards, remember images loading from bottom to top?

            for i in range(0, height):
                of.write('0x' + ', 0x'.join(split2[i]) +',\r\n') # actual byte array
            
            of.write('''};\r\n#endif // HAS_GRAPHICAL_TFT\r\n''')
            
        # constucting all required cpp definitions
        cpp1.append('const tImage ' + name.capitalize() + '         = { (void *)' + name + ', ' \
                    + str(width) + ', ' + str(height) + ', GREYSCALE4 };\n')
        cpp2.append('  ' + name.capitalize() + ',\n')
        h1.append('extern const uint8_t ' + name + '[];\n')
        h2.append('  img'+ name.capitalize().replace("_","") + ',\n')
        h3.append('extern const tImage ' + name.capitalize() + ';\n')
        fileinput.nextfile()
            
with open("./tft_image.cpp", "r+") as f:
    linesout = []
    list = enumerate(f, 0)
    
    # replace lines between tags with new defin
    for i, line in list: 
        if line.strip() != '//startcpp1': linesout.append(line) 
        else: linesout.append(line); break 
    linesout.extend(cpp1)
    for i, line in list:
        if line.strip() == '//endcpp1': linesout.append(line); break
    
    for i, line in list:
        if line.strip() != '//startcpp2': linesout.append(line) 
        else: linesout.append(line); break
    linesout.extend(cpp2)
    for i, line in list:
        if line.strip() == '//endcpp2': linesout.append(line); break
    for i, line in list:
        linesout.append(line)
    
    #overwriting file
    f.seek(0)
    f.writelines(linesout)
    f.truncate()
    

with open("./tft_image.h", "r+") as f:
    linesout = []
    list = enumerate(f, 0)
    
    for i, line in list:
        if line.strip() != '//starth1': linesout.append(line) 
        else: linesout.append(line); break
    linesout.extend(h1)
    for i, line in list:
        if line.strip() == '//endh1': linesout.append(line); break
    
    for i, line in list:
        if line.strip() != '//starth2': linesout.append(line) 
        else: linesout.append(line); break
    linesout.extend(h2)
    for i, line in list:
        if line.strip() == '//endh2': linesout.append(line); break
        
    for i, line in list:
        if line.strip() != '//starth3': linesout.append(line) 
        else: linesout.append(line); break
    linesout.extend(h3)
    for i, line in list:
        if line.strip() == '//endh3': linesout.append(line); break
    for i, line in list:
        linesout.append(line)
        
    f.seek(0)
    f.writelines(linesout)
    f.truncate()
    