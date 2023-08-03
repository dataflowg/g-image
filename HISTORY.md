# G-Image Release History
### v0.5.0 - 2023-08-03
* Support for reading PCX images
* Save GIF and animated GIF to memory (issue #3)
* Support GIF and animated GIF transparency (issue #6)
    * Transparency taken from LabVIEW image mask or alpha channel
* Set animated GIF loop count (issue #7)

##### Changed VIs
* _Indexed Color To True Color.vi_ renamed to _To True Color.vi_
    * VI supports converting 24-bit image + mask input to 32-bit ARGB output
* _True Color To Indexed Color.vi_ renamed to _To Indexed Color.vi_
* Removed `Dither` option from _Save GIF Image.vi_ and _Save Animated GIF Image.vi_
    * Image will always be dithered

### v0.4.0 - 2023-05-05
* Support for reading and writing QOI ('Quite OK Image Format') images
    * Lossless 24-bit and 32-bit image type with fast encode + decode
* Support for loading 16-bit per channel images
* Fix incorrect error output when saving GIF files
* Fix incorrect alpha channel when converting certain 8-bit images to 32-bit
* Fix incorrect default scale when loading SVG from UTF-8 path
* Misc. VI doc updates
* Update stb_image.h to v2.28

##### New VIs
* _Save QOI Image.vi_
* _Load Image (16-bit).vi_
* _Convert Image To 8-bit Per Channel.vim_
* _Convert Image To 16-bit Per Channel.vim_
* _Create Mask By Index.vi_

##### Changed VIs
* _Resize Image.vi_ is now polymorphic, accepting either width x height or a scale multiplier

### v0.3.0 - 2023-02-13
* Basic support for loading Scalable Vector Graphics (SVG)
    * Path based vectors only
* Image rotation performance improvement
* Fix when loading certain 8-bit images
* Fix crash when saving images to memory

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
