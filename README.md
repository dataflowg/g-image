# G-Image
<p align="center">
  <img width="300" height="115" src="images/g-image-logo.png">
</p>
<p align="center">
A cross-platform LabVIEW library for loading, saving, and processing images.
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
* Initial release! See the features section for details.

## <a id="features"></a>Features
* Load images from file or from memory in a range of formats, including:
    * PNG, JPEG, BMP, GIF, Animated GIF, TGA, PSD, HDR, PIC, PNM
* Save images to file or to memory in a range of formats, including:
    * PNG, JPEG, BMP, TGA
* Basic image processing functions:
    * Resize
    * Rotate

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
BMP             | :heavy_check_mark:¹ | :heavy_check_mark:²
GIF             | :heavy_check_mark:¹ | :x:
Animated GIF    | :heavy_check_mark:¹ | :x:
TGA             | :heavy_check_mark:  | :x:
PSD             | :heavy_check_mark:  | :x:
HDR             | :heavy_check_mark:  | :x:
PIC             | :heavy_check_mark:  | :x:
PNM             | :heavy_check_mark:  | :x:

¹ *Palettized images (8-bit and below) are supported, but are converted to 24-bit RGB or 32-bit RGBA images. This may change in future updates.*

² *32-bit images are unsupported.*

### Image Saving
In addition to saving the formats below to a file, G-Image supports saving images directly in memory.

Format          | G-Image             | LabVIEW Picture Functions
----------------|---------------------|--------------------
PNG             | :heavy_check_mark:¹ | :heavy_check_mark:
JPEG            | :heavy_check_mark:  | :heavy_check_mark:
BMP             | :heavy_check_mark:  | :heavy_check_mark:
GIF             | :x:                 | :heavy_check_mark:³
TGA             | :heavy_check_mark:² | :x:

¹ *Images less than 8-bit are converted to 8-bit when saved.*

² *Images are saved with Run Length Encoding (RLE) compression.*

³ *The GIF save function isn't in the palette, but can be found in `<vi.lib>\picture\gif.llb\Write GIF File.vi`*

## <a id="license"></a>License
This library is built using public domain image libraries. As such, this library is also made available in the public domain. See [LICENSE](LICENSE) for details.

## <a id="acknowledgments"></a>Acknowledgments
This library uses the following public domain libraries. Massive thanks to these authors.

Library | Author | Public Domain License
--------|--------|----------------------
[stb_image.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_write.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_resize.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[miniz](https://code.google.com/archive/p/miniz)* | Rich Geldreich | [Unlicense](https://code.google.com/archive/p/miniz)

\*The linked version is public domain. Newer versions of miniz are MIT licensed.
