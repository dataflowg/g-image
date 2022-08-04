#ifndef _G_IMAGE_H_
#define _G_IMAGE_H_

#include <stdint.h>
#include <string.h>

#include "miniz.h"

#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char* compress_for_stbiw(unsigned char *data, int data_len, int *out_len, int quality)
{
	uLongf bufSize = compressBound(data_len);
	// note that buf will be free'd by stb_image_write.h
	// with STBIW_FREE() (plain free() by default)
	unsigned char* buf = (unsigned char*)malloc(bufSize);
	if (buf == NULL)
	{
		return NULL;
	}

	if (compress2(buf, &bufSize, data, data_len, quality) != Z_OK)
	{
		free(buf);
		return NULL;
	}
	*out_len = bufSize;

	return buf;
}
#define STBIW_ZLIB_COMPRESS compress_for_stbiw
#if defined( _WIN32 )
#define __STDC_LIB_EXT1__
#endif
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "gif.h"

#if defined(_WIN32)
#define LV_DLL_IMPORT  __declspec(dllimport)
#define LV_DLL_EXPORT  __declspec(dllexport)
#define LV_DLL_PRIVATE static
#else
#if defined(__GNUC__) && __GNUC__ >= 4
#define LV_DLL_IMPORT  __attribute__((visibility("default")))
#define LV_DLL_EXPORT  __attribute__((visibility("default")))
#define LV_DLL_PRIVATE __attribute__((visibility("hidden")))
#else
#define LV_DLL_IMPORT
#define LV_DLL_EXPORT
#define LV_DLL_PRIVATE static
#endif
#endif

typedef int32_t gi_result;

#define GI_SUCCESS				0
// ERRORS
#define GI_E_GENERIC			-1		// Generic error
#define GI_E_MEMORY				-2		// Memory allocation / deallocation error
#define GI_E_UNSUPPORTED		-3		// Unsupported image type
#define GI_E_FILE				-4		// Generic file IO error

// NOTE: This struct is replicated as a cluster in LabVIEW.
// Be careful of alignment issues - LV 32-bit is byte aligned, LV 64-bit is naturally aligned.
typedef struct
{
	uint8_t* data;
	uint32_t* colors;
	int32_t* delays;
	int32_t data_size;
	int32_t color_size;
	int32_t width;
	int32_t height;
	int32_t depth;
	int32_t layers;
} gi_image_t;

typedef enum
{
	format_save_png,
	format_save_jpg,
	format_save_bmp,
	format_save_gif,
	format_save_tga,
	format_save_count
} image_format_save_t;

typedef enum
{
	dither_none,
	dither_floyd_steinberg
} dither_type_t;

typedef struct
{
	uint8_t* image_data;
	int32_t image_data_count;
	int32_t image_data_size;
} save_callback_data_t;

////////////////////////////
// LabVIEW CLFN Callbacks //
////////////////////////////
extern "C" LV_DLL_EXPORT int32_t clfn_abort(void* data);

///////////////////////
// LabVIEW image API //
///////////////////////
extern "C" LV_DLL_EXPORT gi_result load_image_from_file(const char* file_name, intptr_t* image);
extern "C" LV_DLL_EXPORT gi_result load_image_from_memory(const uint8_t* encoded_image, int32_t encoded_image_size, intptr_t* image_out);
extern "C" LV_DLL_EXPORT gi_result free_image(intptr_t image_ptr);
gi_result free_image(gi_image_t* image);

extern "C" LV_DLL_EXPORT gi_result save_image_to_file(const char* file_name, int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option);
extern "C" LV_DLL_EXPORT gi_result save_image_to_memory(int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option, intptr_t* image_data_out, int32_t* image_data_count_out);
extern "C" LV_DLL_EXPORT gi_result free_data(intptr_t data_ptr);

extern "C" LV_DLL_EXPORT gi_result resize_image(const uint8_t * image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in,
												int32_t width_resize, int32_t height_resize, uint8_t* image_data_out, int32_t filter);

extern "C" LV_DLL_EXPORT gi_result true_color_to_indexed(const uint8_t* image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in, int32_t depth, int32_t dither, uint8_t* image_data_out, uint32_t* colors_out);

gi_result gi_write_gif(const char* file_name, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, dither_type_t dither);
//////////////////////////
// Ancilliary Functions //
//////////////////////////
STBIDEF unsigned char *stbi_load_extended(stbi__context* s, int* x, int* y, int* n, int* layers, int** delays)
{
	unsigned char *result = 0;

	if (stbi__gif_test(s))
		return (uint8_t*)stbi__load_gif_main(s, delays, x, y, layers, n, 4);

	stbi__result_info ri;
	result = (uint8_t*)stbi__load_main(s, x, y, n, 4, &ri, 8);
	*layers = !!result;
	*delays = (int*)calloc(1, sizeof(int));

	if (ri.bits_per_channel != 8) {
		STBI_ASSERT(ri.bits_per_channel == 16);
		result = stbi__convert_16_to_8((stbi__uint16 *)result, *x, *y, 4);
		ri.bits_per_channel = 8;
	}

	return result;
}

STBIDEF unsigned char *stbi_load_from_memory_extended(const unsigned char* buffer, int len, int* x, int* y, int* n, int* layers, int** delays)
{
	stbi__context s;
	stbi__start_mem(&s, buffer, len);
	return stbi_load_extended(&s, x, y, n, layers, delays);
}

STBIDEF unsigned char *stbi_load_from_file_extended(const char* filename, int* x, int* y, int* n, int* layers, int** delays)
{
	FILE *f;
	stbi__context s;
	unsigned char *result = 0;

	if (!(f = stbi__fopen(filename, "rb")))
		return stbi__errpuc("can't fopen", "Unable to open file");

	stbi__start_file(&s, f);
	result = stbi_load_extended(&s, x, y, n, layers, delays);
	fclose(f);

	return result;
}

// Creates a gif file.
// The input GIFWriter is assumed to be uninitialized.
// The delay value is the time between frames in hundredths of a second - note that not all viewers pay much attention to this value.
bool GifBeginUtf8(GifWriter* writer, const char* filename, uint32_t width, uint32_t height, uint32_t delay, int32_t bitDepth = 8, bool dither = false)
{
	(void)bitDepth; (void)dither; // Mute "Unused argument" warnings

	writer->f = stbiw__fopen(filename, "wb");
	if (!writer->f) return false;

	writer->firstFrame = true;

	// allocate
	writer->oldImage = (uint8_t*)GIF_MALLOC(width * height * 4);

	fputs("GIF89a", writer->f);

	// screen descriptor
	fputc(width & 0xff, writer->f);
	fputc((width >> 8) & 0xff, writer->f);
	fputc(height & 0xff, writer->f);
	fputc((height >> 8) & 0xff, writer->f);

	fputc(0xf0, writer->f);  // there is an unsorted global color table of 2 entries
	fputc(0, writer->f);     // background color
	fputc(0, writer->f);     // pixels are square (we need to specify this because it's 1989)

	// now the "global" palette (really just a dummy palette)
	// color 0: black
	fputc(0, writer->f);
	fputc(0, writer->f);
	fputc(0, writer->f);
	// color 1: also black
	fputc(0, writer->f);
	fputc(0, writer->f);
	fputc(0, writer->f);

	if (delay != 0)
	{
		// animation header
		fputc(0x21, writer->f); // extension
		fputc(0xff, writer->f); // application specific
		fputc(11, writer->f); // length 11
		fputs("NETSCAPE2.0", writer->f); // yes, really
		fputc(3, writer->f); // 3 bytes of NETSCAPE2.0 data

		fputc(1, writer->f); // JUST BECAUSE
		fputc(0, writer->f); // loop infinitely (byte 0)
		fputc(0, writer->f); // loop infinitely (byte 1)

		fputc(0, writer->f); // block terminator
	}

	return true;
}

#endif
