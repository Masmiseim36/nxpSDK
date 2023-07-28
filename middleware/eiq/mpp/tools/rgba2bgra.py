# Copyright 2022-2023 NXP
# All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

""" 
This script can be used to reorder channels from
an image.
This script will reorder a 4 channels input image
with format RGBA. The output image channels will be
reordered as BGRA.
""" 

import  cv2
from PIL import Image
import numpy as np
import io
import sys
import getopt

arg_input = ""
arg_output = ""
arg_width = 348
arg_height = 421

print('cmd entry:', sys.argv)

arg_help = "{0} -i <input> -o <output> -w <width> -h <height>".format(sys.argv[0])
try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:o:W:H:", ["help", "input=", "output=", "width=", "height="])

except:
    print(arg_help)
    sys.exit(2)

for opt, arg in opts:
    if opt in ("-h", "--help"):
        print(arg_help)  # print the help message
        sys.exit(2)
    elif opt in ("-i", "--input"):
        arg_input = arg
    elif opt in ("-o", "--output"):
        arg_output = arg
    elif opt in ("-W", "--width"):
        arg_width = int(arg)
    elif opt in ("-H", "--height"):
        arg_height = int(arg)

print('input:', arg_input)
print('output:', arg_output)
print('width:', arg_width)
print('height:', arg_height)

na = np.fromfile(arg_input, dtype=np.uint8).reshape((arg_width,arg_height,4))
r, g, b, a = na[:, :, 0], na[:, :, 1], na[:, :, 2], na[:, :, 3] 
bgra = np.dstack((b,g,r,a))

bgra.tofile(arg_output, '')

