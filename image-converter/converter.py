#!/usr/bin/env python
import cv2
import sys
from os.path import splitext

if len(sys.argv) != 2:
    print( "USAGE: " + sys.argv[0] + " <filename>" )
    sys.exit(1)

filename = sys.argv[1]
varname = splitext(filename)[0]

# Read input file and validate that it is 16x16
img = cv2.imread('/convert/' + filename)
height, width, depth = img.shape

if height != 16 or width != 16:
    print( "ERROR: Image must be 16x16")
    sys.exit(1)

# Create header for C header file
macro = "__" + varname.upper() + "_H__"

print("#ifndef " + macro)
print("#define " + macro)
print("")
print('const unsigned char ' + varname + '[Num_Of_Word][16] = {')

# Convert to HSV so we can decide on output values based on V
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

for i in range(0, width):
    row = ""
    for j in range(0, height):
        if hsv[j,i][2] > 128:
            row += "0"
        else:
            row += "1"

# Append next row to C header file
    rowHex = "{0:0>4X}".format(int(row,2))
    print ("    0x" + rowHex[:2] + ", 0x" + rowHex[2:] + ", // " + row)

# Create footer
print("};");
print("#endif // " + macro)

# cv2.imshow('image', img)
# cv2.waitKey(0)


