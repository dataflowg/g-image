/*
G-Image - An image library for LabVIEW.

See g_image.h for license details.
*/

#include "pch.h"
#include "g_image.h"

////////////////////////////
// LabVIEW CLFN Callbacks //
////////////////////////////

extern "C" LV_DLL_EXPORT int32_t clfn_abort(void* data)
{
	return 0;
}

extern "C" LV_DLL_EXPORT gi_result load_image_from_file(const char* file_name, intptr_t* image_out)
{
	int x, y, n;
	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));
	
	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	image->data = stbi_load(file_name, &x, &y, &n, 4);

	if (image->data == NULL)
	{
		stbi_failure_reason();

		return GI_E_GENERIC;
	}

	image->data_size = sizeof(uint8_t) * x * y * 4;
	image->width = x;
	image->height = y;
	image->depth = n * 8;

	*image_out = (intptr_t)image;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result load_image_from_memory(const uint8_t* encoded_image, int32_t encoded_image_count, intptr_t* image_out)
{
	int x, y, n;
	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));

	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	image->data = stbi_load_from_memory(encoded_image, encoded_image_count, &x, &y, &n, 4);

	if (image->data == NULL)
	{
		stbi_failure_reason();

		return GI_E_GENERIC;
	}

	image->data_size = sizeof(uint8_t) * x * y * 4;
	image->width = x;
	image->height = y;
	image->depth = n * 8;

	*image_out = (intptr_t)image;

	return GI_SUCCESS;
}

void load_gif_frame_callback(void* context, struct GIF_WHDR* data)
{
	gi_image_t* image = (gi_image_t*)context;

	if (image != NULL)
	{
		// frame X dim * frame Y dim
		image->data_size = data->frxd * data->fryd;
		image->data = (uint8_t*)malloc(image->data_size);
		if (image->data == NULL)
		{
			image->data_size = 0;
			return;
		}
		image->color_size = data->clrs;
		image->colors = (uint32_t*)malloc(image->color_size * sizeof(uint32_t));
		if (image->colors == NULL)
		{
			free(image->data);
			image->color_size = 0;
			return;
		}
		memcpy(image->data, data->bptr, image->data_size);
		for (int i = 0; i < image->color_size; i++)
		{
			image->colors[i] = data->cpal[i].R << 16 | data->cpal[i].G << 8 | data->cpal[i].B;
		}
		image->width = data->frxd;
		image->height = data->fryd;
		image->depth = 8;
	}
}

extern "C" LV_DLL_EXPORT gi_result load_gif_from_memory(const uint8_t * encoded_image, int32_t encoded_image_size, intptr_t* image_out)
{
	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));

	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	GIF_Load((void*)encoded_image, encoded_image_size, load_gif_frame_callback, NULL, image, 0);

	*image_out = (intptr_t)image;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result free_image(intptr_t image_ptr)
{
	gi_image_t* image = (gi_image_t*)image_ptr;

	STBI_FREE(image->data);
	image->data = NULL;
	STBI_FREE(image->colors);
	image->colors = NULL;
	free(image);
	image = NULL;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result save_image_to_file(const char* file_name, int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option)
{
	int result;

	image_format_save_t format_save = (image_format_save_t)format;
	switch (format_save)
	{
		case format_save_png: result = stbi_write_png(file_name, width, height, channels, image_data, 0); break;
		case format_save_jpg: result = stbi_write_jpg(file_name, width, height, channels, image_data, *(int32_t*)option); break;
		case format_save_bmp: result = stbi_write_bmp(file_name, width, height, channels, image_data); break;
		case format_save_tga: result = stbi_write_tga(file_name, width, height, channels, image_data); break;
		default: return GI_E_UNSUPPORTED; break;
	}

	if (result == 0)
	{
		return GI_E_GENERIC;
	}

	return GI_SUCCESS;
}

void save_callback(void *context, void *data, int size)
{
	save_callback_data_t* callback_data = (save_callback_data_t*)context;

	if (callback_data->image_data == NULL)
	{
		return;
	}

	if (callback_data->image_data_count + size > callback_data->image_data_size)
	{
		// Increase memory allocation by an additional 50%
		size_t resize = callback_data->image_data_size * 1.5;
		uint8_t* temp_ptr = (uint8_t*)realloc(callback_data->image_data, resize);

		if (temp_ptr == NULL)
		{
			free(callback_data->image_data);
			callback_data->image_data = NULL;
			callback_data->image_data_count = 0;
			callback_data->image_data_size = 0;
			return;
		}

		callback_data->image_data = temp_ptr;
		callback_data->image_data_size = resize;
	}

	memcpy(callback_data->image_data + callback_data->image_data_count, data, size);
	callback_data->image_data_count = callback_data->image_data_count + size;
}

extern "C" LV_DLL_EXPORT gi_result save_image_to_memory(int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data_in, void* option, intptr_t* encoded_image, int32_t* encoded_image_count)
{
	int result;
	save_callback_data_t callback_data;

	// Start with a small memory allocation, callback will reallocate as necessary.
	callback_data.image_data_size = width * height * channels / 10;
	callback_data.image_data = (uint8_t*)malloc(callback_data.image_data_size);
	callback_data.image_data_count = 0;
	
	if (callback_data.image_data == NULL)
	{
		return GI_E_MEMORY;
	}

	image_format_save_t format_save = (image_format_save_t)format;
	switch (format_save)
	{
		case format_save_png: result = stbi_write_png_to_func(save_callback, &callback_data, width, height, channels, image_data_in, 0); break;
		case format_save_jpg: result = stbi_write_jpg_to_func(save_callback, &callback_data, width, height, channels, image_data_in, *(int32_t*)option); break;
		case format_save_bmp: result = stbi_write_bmp_to_func(save_callback, &callback_data, width, height, channels, image_data_in); break;
		case format_save_tga: result = stbi_write_tga_to_func(save_callback, &callback_data, width, height, channels, image_data_in); break;
		default: return GI_E_UNSUPPORTED; break;
	}

	if (result == 0)
	{
		return GI_E_GENERIC;
	}

	if (callback_data.image_data == NULL)
	{
		return GI_E_MEMORY;
	}

	*encoded_image = (intptr_t)callback_data.image_data;
	*encoded_image_count = callback_data.image_data_count;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result free_data(intptr_t data_ptr)
{
	uint8_t* data = (uint8_t*)data_ptr;

	STBIW_FREE(data);
	data = NULL;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result resize_image(const uint8_t* image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in,
												int32_t width_resize, int32_t height_resize, uint8_t* image_data_out)
{
	int result;

	result = stbir_resize_uint8(image_data_in, width_in, height_in, 0, image_data_out, width_resize, height_resize, 0, channels_in);

	if (result != 1)
	{
		return GI_E_GENERIC;
	}

	return GI_SUCCESS;
}