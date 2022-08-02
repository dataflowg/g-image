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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

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
extern "C" LV_DLL_EXPORT gi_result load_image_from_memory(const uint8_t * encoded_image, int32_t encoded_image_size, intptr_t* image_out);
extern "C" LV_DLL_EXPORT gi_result free_image(intptr_t image_ptr);

extern "C" LV_DLL_EXPORT gi_result save_image_to_file(const char* file_name, int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option);
extern "C" LV_DLL_EXPORT gi_result save_image_to_memory(int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option, intptr_t * image_data_out, int32_t * image_data_count_out);
extern "C" LV_DLL_EXPORT gi_result free_data(intptr_t data_ptr);

extern "C" LV_DLL_EXPORT gi_result resize_image(const uint8_t * image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in,
												int32_t width_resize, int32_t height_resize, uint8_t* image_data_out, int32_t filter);

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

#endif
