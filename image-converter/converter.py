#!/usr/bin/env python

import cv2
import sys

if sys.argc != 2:
    print( "USAGE: " + sys.argv[0] + " <filename>" )
    sys.exit(1)

img = cv2.imread('/convert/smiley.png')
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

height, width, depth = img.shape

print ( '''
const unsigned char Word[Num_Of_Word][32] =
{ ''')

for i in range(0, width):
    row = ""
    for j in range(0, height):
        if hsv[j,i][2] > 128:
            row += "0"
        else:
            row += "1"

    # rowHex = hex(int(row,2))
    rowHex = "{0:0>4X}".format(int(row,2))

    cDeclaration = "    0x" + rowHex[:2] + ", 0x" + rowHex[2:] + ", // " + row

    print (cDeclaration)

print("};");

# cv2.imshow('image', img)
# cv2.waitKey(0)


