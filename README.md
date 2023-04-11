# G-Image
<p align="center">
  <img width="300" height="115" src="images/g-image-logo.png">
</p>
<p align="center">
A cross-platform LabVIEW library for loading, saving, and processing images.
</p>
<p align="center">
<a href="https://www.vipm.io/package/dataflow_g_lib_g_image/"> <img src="https://www.vipm.io/package/dataflow_g_lib_g_image/badge.svg?metric=installs"></a> <a href="https://www.vipm.io/package/dataflow_g_lib_g_image/"><img src="https://www.vipm.io/package/dataflow_g_lib_g_image/badge.svg?metric=stars"></a>
</p>

<p align="center">
    <a href="#whats-new">What's New?</a> -
    <a href="#features">Features</a> -
    <a href="#installation">Installation</a> -
    <a href="#usage">Usage</a> -
	<a href="#building">Building</a> -
	<a href="#comparison">Comparison</a> - 
	<a href="#license">License</a> - 
	<a href="#acknowledgments">Acknowledgments</a>
</p>

## <a id="whats-new"></a>What's New?
* Support for reading and writing QOI ('Quite OK Image Format') images
    * Lossless 24-bit and 32-bit image type with fast encode + decode
* Fix incorrect error output when saving GIF files
* Misc. VI doc updates
* Update stb_image.h to v2.28

## <a id="features"></a>Features
* Load images from file or from memory in a range of formats, including:
    * PNG, JPEG, BMP, GIF, Animated GIF, TGA, SVG, PSD, HDR, PIC, PNM, QOI
* Save images to file or to memory in a range of formats, including:
    * PNG, JPEG, BMP, GIF, Animated GIF, TGA, QOI
* Basic image processing functions:
    * Resize
    * Rotate
    * Flip
    * Quantize (True Color to Indexed Color)

## <a id="installation"></a>Installation
TBD

## <a id="usage"></a>Usage
See the example VIs in [Examples](src/LabVIEW/G-Image/Examples) to load, save, and process images.

Unit tests are included and can be run individually, or with the [AST Unit Tester](https://www.autosofttech.net/documents/ast-unit-tester).

## <a id="building"></a>Building
TBD

## <a id="comparison"></a>Comparison
### Image Loading
In addition to loading the formats below from a file, G-Image supports loading images directly from memory.

Format          | G-Image             | LabVIEW Picture Functions
----------------|---------------------|--------------------
PNG             | :heavy_check_mark:¹ | :heavy_check_mark:
JPEG            | :heavy_check_mark:  | :heavy_check_mark:
BMP             | :heavy_check_mark:¹ | :heavy_check_mark:³
GIF             | :heavy_check_mark:¹ | :x:
Animated GIF    | :heavy_check_mark:¹ | :x:
TGA             | :heavy_check_mark:  | :x:
SVG             | :heavy_check_mark:² | :x:
PSD             | :heavy_check_mark:  | :x:
HDR             | :heavy_check_mark:  | :x:
PIC             | :heavy_check_mark:  | :x:
PNM             | :heavy_check_mark:  | :x:
QOI             | :heavy_check_mark:  | :x:

¹ *Indexed color images (8-bit and below) are supported, but are converted to 24-bit or 32-bit images. This may change in future updates.*

² *Only paths are supported.*

³ *32-bit images are unsupported.*

### Image Saving
In addition to saving the formats below to a file, G-Image supports saving images directly in memory.

Format          | G-Image             | LabVIEW Picture Functions
----------------|---------------------|--------------------
PNG             | :heavy_check_mark:¹ | :heavy_check_mark:
JPEG            | :heavy_check_mark:  | :heavy_check_mark:
BMP             | :heavy_check_mark:¹ | :heavy_check_mark:
GIF             | :heavy_check_mark:  | :heavy_check_mark:²
Animated GIF    | :heavy_check_mark:  | :x:
TGA             | :heavy_check_mark:  | :x:
QOI             | :heavy_check_mark:  | :x:

¹ *Indexed color images (8-bit and below) are supported, but are converted to 24-bit or 32-bit images. This may change in future updates.*

² *The GIF save function isn't in the palette, but can be found in `<vi.lib>\picture\gif.llb\Write GIF File.vi`. Only 8-bit images are supported, and are not compressed with LZW compression.*

## <a id="license"></a>License
This library is built using public domain image libraries where possible. As such, this library is also made available in the public domain. See [LICENSE](LICENSE) for details.

Not all libraries used by G-Image are public domain, but are licensed using open source permissive licenses.

## <a id="acknowledgments"></a>Acknowledgments
This library uses the following open source libraries. Massive thanks to these authors.

Library | Author | License
--------|--------|----------------------
[stb_image.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_write.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_resize.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[miniz](https://code.google.com/archive/p/miniz)¹ | Rich Geldreich | [Unlicense](https://code.google.com/archive/p/miniz)
[gif-h](https://github.com/charlietangora/gif-h) | Charlie Tangora | [Public Domain](https://github.com/charlietangora/gif-h/blob/master/gif.h)
[nanosvg](https://github.com/memononen/nanosvg) | Mikko Mononen | [zlib](https://github.com/memononen/nanosvg/blob/master/LICENSE.txt)
[qoi](https://github.com/phoboslab/qoi) | Dominic Szablewski | [MIT](https://github.com/phoboslab/qoi/blob/master/LICENSE)

¹ *The linked version is public domain, and is the version used in G-Image. Newer versions of miniz are MIT licensed.*
