#!/usr/bin/env python

import cv2
import numpy as np
from pylibseek_ext import *

def process_frame(inframe, scale=1.0, colormap=-1, rotate=0):
	frame_g16 = cv2.normalize(inframe, alpha=0, beta=65535, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_16UC1)
	frame_g8  = cv2.convertScaleAbs(src=frame_g16, alpha=1.0/256.0, beta=0.0)

    #Rotate the image, if desired
    if( rotate == 90 ):
        frame_g8 = cv2.transpose(src=frame_g8)
        frame_g8 = cv2.flip(src=frame_g8, flipMode=1)
    else if( rotate == 180 ):
        frame_g8 = cv2.flip(src=frame_g8, flipMode=-1)
    else if( rotate == 270 ):
        frame_g8 = cv2.transpose(src=frame_g8)
        frame_g8 = cv2.flip(src=frame_g8, flipMode=0)

    #Scale the image up/down
    if ( scale != 1.0 ):
        frame_g8 = cv2.resize(src=frame_g8, dsize=Size(), fx=scale, fy=scale, interpolation=cv2.INTER_LINEAR)

    #Change the colormap
	if( colormap != -1 ):
		frame_g8_color  = cv2.applyColorMap(frame_g8, colormap=colormap)
	else:
		frame_g8_color  = cv2.cvtColor(src=frame_g8, code=cv2.COLOR_GRAY2BGR)

	return(frame_g8_color)


st = SeekThermal()
st.open()
frame = st.read()
frame_color = process_frame(frame)
cv2.imwrite("test_color.jpg", frame_color)
st.close()

