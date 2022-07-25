# G-Image
<p align="center">
  <img width="300" height="115" src="images/g-image-logo.png">
</p>
<p align="center">
A cross-platform LabVIEW library for image loading, saving, and processing.
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
    * png, jpg, bmp
* Save images to file or to memory in a range of formats, including:
    * png, jpg, bmp
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
PNG             | :heavy_check_mark:  | :heavy_check_mark:
JPEG            | :heavy_check_mark:  | :heavy_check_mark:
BMP             | :heavy_check_mark:  | :heavy_check_mark:

### Image Saving
In addition to saving the formats below to a file, G-Image supports saving images directly in memory.

Format          | G-Image             | LabVIEW Picture Functions
----------------|---------------------|--------------------
PNG             | :heavy_check_mark:  | :heavy_check_mark:
JPEG            | :heavy_check_mark:  | :heavy_check_mark:
BMP             | :heavy_check_mark:  | :heavy_check_mark:

## <a id="license"></a>License
This library is built using public domain image libraries. As such, this library is also made available in the public domain. See [LICENSE](LICENSE) for details.

## <a id="acknowledgments"></a>Acknowledgments
This library uses the following public domain libraries. Massive thanks to these authors.

Library | Author | Public Domain License
--------|--------|----------------------
[stb_image.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_write.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
[stb_image_resize.h](https://github.com/nothings/stb) | Sean Barrett | [Unlicense / MIT](https://github.com/nothings/stb/blob/master/LICENSE)
