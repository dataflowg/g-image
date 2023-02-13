#!/bin/bash
OUTPUT_PATH=../LabVIEW/G-Image/lib

g++ -shared -fPIC -o g_image_64.so *.cpp -O3
mkdir -p $OUTPUT_PATH
mv g_image_64.so $OUTPUT_PATH
