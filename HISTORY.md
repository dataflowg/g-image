# G-Image Release History
### v0.4.0 - 2023-05-xx
* Support for reading and writing QOI ('Quite OK Image Format') images
    * Lossless 24-bit and 32-bit image type with fast encode + decode
* Support for loading and processing 16-bit per channel images
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
