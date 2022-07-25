#ifndef _G_IMAGE_H_
#define _G_IMAGE_H_

#include <stdint.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if defined( _WIN32 )
#define __STDC_LIB_EXT1__
#endif
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STBIR_DEFAULT_FILTER_UPSAMPLE     STBIR_FILTER_BOX
#define STBIR_DEFAULT_FILTER_DOWNSAMPLE   STBIR_FILTER_BOX
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
	uint32_t data_size;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} gi_image_t;

typedef enum
{
	format_load_png,
	format_load_jpg,
	format_load_bmp,
	format_load_gif,
	format_load_tga,
	format_load_count
} image_format_load_t;

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

extern "C" LV_DLL_EXPORT gi_result save_image_to_file(const char* file_name, int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data);
extern "C" LV_DLL_EXPORT gi_result save_image_to_memory(int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, intptr_t * image_data_out, int32_t * image_data_count_out);
extern "C" LV_DLL_EXPORT gi_result free_data(intptr_t data_ptr);

extern "C" LV_DLL_EXPORT gi_result resize_image(const uint8_t * image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in,
												int32_t width_resize, int32_t height_resize, uint8_t* image_data_out);

#endif
