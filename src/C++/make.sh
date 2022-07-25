#!/bin/bash
OUTPUT_PATH=../LabVIEW/G-Image/lib

mkdir -p $OUTPUT_PATH
g++ -shared -fPIC -o g_image_64.so *.cpp -O3
mv g_image_64.so $OUTPUT_PATH
