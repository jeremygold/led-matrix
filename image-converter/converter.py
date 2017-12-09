#!/usr/bin/env python

import cv2

img = cv2.imread('smiley.png')
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

height, width, depth = img.shape

for i in range(0, height):
    row = ""
    for j in range(0, width):
        if hsv[i,j][2] > 128:
            row += "*"
        else:
            row += " "
    print row

# cv2.imshow('image', img)
# cv2.waitKey(0)


