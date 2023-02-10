# G-Image Release History
### v0.3.0 - 2022-10-xx
* Basic support for loading Scalable Vector Graphics (SVG)
    * Path based vectors only
* Image rotation implemented in C (~2x performance boost)
* Fix when loading certain 8-bit images

### v0.2.0 - 2022-08-08
* Support saving GIF and animated GIF (with optional dithering)
* Add PNG compression option when saving
* Add TGA compression option when saving
* Image resize filter option
    * Box, Triangle, Cubic B-Spline, Catmull-Rom, Mitchell
* Improved PNG compression
* New image processing functions
    * Flip Image
    * True Color <-> Indexed Color (with optional dithering)
* Fix image processing on certain 8-bit images

### v0.1.0 - 2022-07-28
* Initial release
