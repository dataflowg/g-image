#!/bin/bash
OUTPUT_PATH=/usr/lib

g++ -shared -fPIC -o g_image_32.so *.cpp -std=c++11 -mfpu=neon -mfloat-abi=softfp -O3
mv g_image_32.so $OUTPUT_PATH
