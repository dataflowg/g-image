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
	int x, y, n, layers;
	int* delays = NULL;

	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));
	
	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	image->data = stbi_load_from_file_extended(file_name, &x, &y, &n, &layers, &delays);

	if (image->data == NULL)
	{
		stbi_failure_reason();

		return GI_E_GENERIC;
	}

	// If the image is less than 24-bit, assume it's 24-bit as we've requested 4 color components from stb_image
	if (n < 3)
	{
		n = 3;
	}

	image->data_size = sizeof(uint8_t) * x * y * layers * 4;
	image->width = x;
	image->height = y;
	image->depth = n * 8;
	image->layers = layers;
	image->delays = delays;

	*image_out = (intptr_t)image;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result load_image_from_memory(const uint8_t* encoded_image, int32_t encoded_image_count, intptr_t* image_out)
{
	int x, y, n, layers;
	int* delays = NULL;

	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));

	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	image->data = stbi_load_from_memory_extended(encoded_image, encoded_image_count, &x, &y, &n, &layers, &delays);

	if (image->data == NULL)
	{
		stbi_failure_reason();

		return GI_E_GENERIC;
	}

	// If the image is less than 24-bit, assume it's 24-bit as we've requested 4 color components from stb_image
	if (n < 3)
	{
		n = 3;
	}

	image->data_size = sizeof(uint8_t) * x * y * layers * 4;
	image->width = x;
	image->height = y;
	image->depth = n * 8;
	image->layers = layers;
	image->delays = delays;

	*image_out = (intptr_t)image;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result free_image(intptr_t image_ptr)
{
	return free_image((gi_image_t*)image_ptr);
}

gi_result free_image(gi_image_t* image)
{
	if (image != NULL)
	{
		STBI_FREE(image->data);
		image->data = NULL;
		STBI_FREE(image->colors);
		image->colors = NULL;
		STBI_FREE(image->delays);
		image->delays = NULL;
		free(image);
		image = NULL;
	}

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result load_svg_from_file(const char* file_name, float scale, intptr_t* image_out)
{
	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));

	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	// Just use px/96 to load. The rasterizer will control the final image size
	NSVGimage* svg = nsvgParseFromFileUtf8(file_name, "px", 96.0);

	if (svg == NULL)
	{
		return GI_E_MEMORY;
	}

	// Create rasterizer (can be used to render multiple images).
	struct NSVGrasterizer* rast = nsvgCreateRasterizer();

	if (rast == NULL)
	{
		return GI_E_MEMORY;
	}

	int width = svg->width * scale;
	int height = svg->height * scale;

	// Allocate memory for image
	uint8_t* image_data = (uint8_t*)malloc(width * height * 4);
	// Rasterize
	nsvgRasterize(rast, svg, 0, 0, scale, image_data, width, height, width * 4);

	image->data = image_data;
	image->data_size = width * height * 4;
	image->width = width;
	image->height = height;
	image->depth = 32;
	image->layers = 1;
	image->delays = (int32_t*)calloc(1, sizeof(int32_t));
	*image_out = (intptr_t)image;

	nsvgDeleteRasterizer(rast);
	nsvgDelete(svg);

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result load_svg_from_memory(const uint8_t* encoded_image, int32_t encoded_image_count, float scale, intptr_t* image_out)
{
	gi_image_t* image = (gi_image_t*)calloc(1, sizeof(gi_image_t));

	if (image == NULL)
	{
		return GI_E_MEMORY;
	}

	// Just use px/96 to load. The rasterizer will control the final image size
	NSVGimage* svg = nsvgParseFromMemory(encoded_image, encoded_image_count, "px", 96.0);

	if (svg == NULL)
	{
		return GI_E_MEMORY;
	}

	// Create rasterizer (can be used to render multiple images).
	struct NSVGrasterizer* rast = nsvgCreateRasterizer();

	if (rast == NULL)
	{
		return GI_E_MEMORY;
	}

	int width = svg->width * scale;
	int height = svg->height * scale;

	// Allocate memory for image
	uint8_t* image_data = (uint8_t*)malloc(width * height * 4);
	// Rasterize
	nsvgRasterize(rast, svg, 0, 0, scale, image_data, width, height, width * 4);

	image->data = image_data;
	image->data_size = width * height * 4;
	image->width = width;
	image->height = height;
	image->depth = 32;
	image->layers = 1;
	image->delays = (int32_t*)calloc(1, sizeof(int32_t));
	*image_out = (intptr_t)image;

	nsvgDeleteRasterizer(rast);
	nsvgDelete(svg);

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result save_image_to_file(const char* file_name, int32_t format, int32_t width, int32_t height, int32_t channels, const uint8_t* image_data, void* option)
{
	int result;

	image_format_save_t format_save = (image_format_save_t)format;
	switch (format_save)
	{
		case format_save_png: stbi_write_png_compression_level = *(int32_t*)option;
			                  result = stbi_write_png(file_name, width, height, channels, image_data, 0); break;
		case format_save_jpg: result = stbi_write_jpg(file_name, width, height, channels, image_data, *(int32_t*)option); break;
		case format_save_bmp: result = stbi_write_bmp(file_name, width, height, channels, image_data); break;
		case format_save_gif: result = gi_write_gif(file_name, width, height, image_data, (dither_type_t)*(uint32_t*)option); break;
		case format_save_tga: stbi_write_tga_with_rle = *(int32_t*)option;
							  result = stbi_write_tga(file_name, width, height, channels, image_data); break;
		default: return GI_E_UNSUPPORTED; break;
	}

	if (result == 0)
	{
		return GI_E_GENERIC;
	}

	return GI_SUCCESS;
}

gi_result gi_write_gif(const char* file_name, int32_t width, int32_t height, const uint8_t* image_data, dither_type_t dither)
{
	GifWriter writer = {};
	bool gif_result = true;

	gif_result &= GifBeginUtf8(&writer, file_name, width, height, 0, 8, dither == dither_floyd_steinberg);
	gif_result &= GifWriteFrame(&writer, image_data, width, height, 0, 8, dither == dither_floyd_steinberg);
	gif_result &= GifEnd(&writer);

	if (!gif_result)
	{
		return GI_E_FILE;
	}

	return GI_SUCCESS;
}


extern "C" LV_DLL_EXPORT gi_result open_write_gif(const char* file_name, int32_t width, int32_t height, int32_t depth, intptr_t* writer_ptr)
{
	bool result;

	GifWriter* writer = (GifWriter*)malloc(sizeof(GifWriter));
	if (writer == NULL)
	{
		return GI_E_MEMORY;
	}

	result = GifBeginUtf8(writer, file_name, width, height, 1, depth);

	if (!result)
	{
		free(writer);
		return GI_E_FILE;
	}

	*writer_ptr = (intptr_t)writer;

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result write_gif_frame(intptr_t writer_ptr, const uint8_t* image_data, int32_t width, int32_t height, int32_t delay, uint32_t dither)
{
	bool result;

	result = GifWriteFrame((GifWriter*)writer_ptr, image_data, width, height, delay, 8, (dither_type_t)dither == dither_floyd_steinberg);

	if (!result)
	{
		return GI_E_FILE;
	}

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result close_gif(intptr_t writer_ptr)
{
	bool result;
	GifWriter* writer = (GifWriter*)writer_ptr;

	result = GifEnd(writer);
	free(writer);
	writer = NULL;

	if (!result)
	{
		return GI_E_FILE;
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
		size_t resize = (size_t)(callback_data->image_data_size * 1.5);
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
		case format_save_png: stbi_write_png_compression_level = *(int32_t*)option;
							  result = stbi_write_png_to_func(save_callback, &callback_data, width, height, channels, image_data_in, 0); break;
		case format_save_jpg: result = stbi_write_jpg_to_func(save_callback, &callback_data, width, height, channels, image_data_in, *(int32_t*)option); break;
		case format_save_bmp: result = stbi_write_bmp_to_func(save_callback, &callback_data, width, height, channels, image_data_in); break;
		case format_save_tga: stbi_write_tga_with_rle = *(int32_t*)option;
							  result = stbi_write_tga_to_func(save_callback, &callback_data, width, height, channels, image_data_in); break;
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
												int32_t width_resize, int32_t height_resize, uint8_t* image_data_out, int32_t filter)
{
	int result;

	//result = stbir_resize_uint8(image_data_in, width_in, height_in, 0, image_data_out, width_resize, height_resize, 0, channels_in);

	// Note: image_data_in will be in ARGB format when channels_in == 4, so the alpha index is 0.
	result = stbir_resize_uint8_generic(image_data_in, width_in, height_in, 0, image_data_out, width_resize, height_resize, 0, channels_in,
										channels_in == 4 ? 0 : STBIR_ALPHA_CHANNEL_NONE, 0, STBIR_EDGE_CLAMP, (stbir_filter)filter, STBIR_COLORSPACE_LINEAR, NULL);

	if (result != 1)
	{
		return GI_E_GENERIC;
	}

	return GI_SUCCESS;
}

extern "C" LV_DLL_EXPORT gi_result rotate_image(const uint8_t* image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in, int32_t row_stride, double angle, uint8_t* image_data_out)
{
	double rotation_matrix[2][2];
	double translate_x;
	double translate_y;
	double rotated_x;
	double rotated_y;
	int32_t pixel_x;
	int32_t pixel_y;
	int32_t row_offset;
	double center_x = width_in / 2.0;
	double center_y = height_in / 2.0;

	double angle_rad = angle * GI_DEG_TO_RAD;
	rotation_matrix[0][0] = cos(angle_rad);
	rotation_matrix[0][1] = -sin(angle_rad);
	rotation_matrix[1][0] = -rotation_matrix[0][1]; // sin(angle_rad)
	rotation_matrix[1][1] = rotation_matrix[0][0];  // cos(angle_rad)

	for (int y = 0; y < height_in; y++)
	{
		row_offset = row_stride * y;
		for (int x = 0; x < width_in; x++)
		{
			// Translate the pixel location by the center of rotation
			translate_x = x - center_x;
			translate_y = y - center_y;
			// Calculate the rotated pixel location
			rotated_x = translate_x * rotation_matrix[0][0] + translate_y * rotation_matrix[0][1];
			rotated_y = translate_x * rotation_matrix[1][0] + translate_y * rotation_matrix[1][1];
			// Translate the rotated pixel back from the center of rotation
			pixel_x = (int)(rotated_x + center_x);
			pixel_y = (int)(rotated_y + center_y);
			// Check the new pixel location is within the bounds of the original image
			if (pixel_x >= 0 && pixel_x < width_in && pixel_y >= 0 && pixel_y < height_in)
			{
				memcpy(image_data_out + row_offset + (x * channels_in), image_data_in + pixel_y * row_stride + (pixel_x * channels_in), channels_in * sizeof(uint8_t));
			}
		}
	}

	return GI_SUCCESS;
}


extern "C" LV_DLL_EXPORT gi_result true_color_to_indexed(const uint8_t* image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in, int32_t depth, uint32_t dither, uint8_t* image_data_out, uint32_t* colors_out)
{
	if (channels_in < 4)
	{
		return GI_E_UNSUPPORTED;
	}

	GifPalette pal;
	GifMakePalette(NULL, image_data_in, width_in, height_in, depth, (dither_type_t)dither == dither_floyd_steinberg, &pal);

	if (dither)
		GifDitherImage(NULL, image_data_in, image_data_out, width_in, height_in, &pal);
	else
		GifThresholdImage(NULL, image_data_in, image_data_out, width_in, height_in, &pal);

	int32_t num_colors = 1 << depth;
	for (int i = 0; i < num_colors; i++)
	{
		colors_out[i] = pal.r[i] << 16 | pal.g[i] << 8 | pal.b[i];
	}

	return GI_SUCCESS;
}




/*void get_palette_image(uint8_t* rgba, int32_t width, int32_t height, uint8_t* indexedPixels, uint8_t* palette, int32_t colors)
{
	int32_t size = width * height;
	unsigned char *ditheredPixels = (unsigned char*)malloc(size * 4);
	memcpy(ditheredPixels, rgba, size * 4);
	for (int k = 0; k < size * 4; k += 4) {
		int rgb[3] = { ditheredPixels[k + 0], ditheredPixels[k + 1], ditheredPixels[k + 2] };
		//int rgb[3] = { rgba[k + 0], rgba[k + 1], rgba[k + 2] };
		int bestd = 0x7FFFFFFF, best = -1;
		// TODO: exhaustive search. do something better.
		for (int i = 0; i < colors; ++i) {
			int bb = palette[i * 3 + 0] - rgb[0];
			int gg = palette[i * 3 + 1] - rgb[1];
			int rr = palette[i * 3 + 2] - rgb[2];
			int d = bb * bb + gg * gg + rr * rr;
			if (d < bestd) {
				bestd = d;
				best = i;
			}
		}
		indexedPixels[k / 4] = best;
		int diff[3] = { ditheredPixels[k + 0] - palette[indexedPixels[k / 4] * 3 + 0], ditheredPixels[k + 1] - palette[indexedPixels[k / 4] * 3 + 1], ditheredPixels[k + 2] - palette[indexedPixels[k / 4] * 3 + 2] };
		// Floyd-Steinberg Error Diffusion
		// TODO: Use something better -- http://caca.zoy.org/study/part3.html
		if (k + 4 < size * 4) {
			ditheredPixels[k + 4 + 0] = (unsigned char)jo_gif_clamp(ditheredPixels[k + 4 + 0] + (diff[0] * 7 / 16), 0, 255);
			ditheredPixels[k + 4 + 1] = (unsigned char)jo_gif_clamp(ditheredPixels[k + 4 + 1] + (diff[1] * 7 / 16), 0, 255);
			ditheredPixels[k + 4 + 2] = (unsigned char)jo_gif_clamp(ditheredPixels[k + 4 + 2] + (diff[2] * 7 / 16), 0, 255);
		}
		if (k + width * 4 + 4 < size * 4) {
			for (int i = 0; i < 3; ++i) {
				ditheredPixels[k - 4 + width * 4 + i] = (unsigned char)jo_gif_clamp(ditheredPixels[k - 4 + width * 4 + i] + (diff[i] * 3 / 16), 0, 255);
				ditheredPixels[k + width * 4 + i] = (unsigned char)jo_gif_clamp(ditheredPixels[k + width * 4 + i] + (diff[i] * 5 / 16), 0, 255);
				ditheredPixels[k + width * 4 + 4 + i] = (unsigned char)jo_gif_clamp(ditheredPixels[k + width * 4 + 4 + i] + (diff[i] * 1 / 16), 0, 255);
			}
		}
	}
	free(ditheredPixels);
}

extern "C" LV_DLL_EXPORT gi_result true_color_to_indexed(const uint8_t* image_data_in, int32_t width_in, int32_t height_in, int32_t channels_in, int32_t depth, int32_t dither, uint8_t* image_data_out, uint8_t* colors_out)
{
	if (channels_in < 4)
	{
		return GI_E_UNSUPPORTED;
	}

	jo_gif_quantize((uint8_t*)image_data_in, width_in * height_in * channels_in, 1, colors_out, depth);
	get_palette_image((uint8_t*)image_data_in, width_in, height_in, image_data_out, colors_out, depth);

	return GI_SUCCESS;
}
*/
