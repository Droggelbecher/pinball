
import std.stdio;

import pngconf;
import png;

import buffer_canvas;
import coordinate;
import switchable;
import task;

BufferCanvas read_png(string filename) {
	//enum PNG_BYTES_TO_CHECK = 4;

	// https://gist.github.com/niw/5963798

	auto file = File(filename, "rb");
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, null, null, null);
	if(!png) {
		throw new Exception("PNG: Error loading file");
	}

	png_infop info = png_create_info_struct(png);
	if(!info) {
		throw new Exception("PNG: Error creating info struct");
	}

	png_init_io(png, file.getFP);
	png_read_info(png, info);

	auto width = png_get_image_width(png, info);
	auto height = png_get_image_height(png, info);
	auto color_type = png_get_color_type(png, info);
	auto bits = png_get_bit_depth(png, info);

	png_read_update_info(png, info);

	// TODO: Include filename in error messages
	// TODO: Ensure correct width & height
	if(color_type != PNG_COLOR_TYPE_PALETTE || bits != 4) {
		writefln("w=%d h=%d c=%d bits=%d", width, height, color_type, bits);
		throw new Exception("PNG: Expected 4-Bit Paletted PNGs!");
	}


	auto row_pointers = new png_bytep[](height);
	auto row_bytes = png_get_rowbytes(png, info);

	for(int i=0; i< row_pointers.length; i++) {
		row_pointers[i] = (new png_byte[](row_bytes)).ptr;
	}

	png_read_image(png, row_pointers.ptr);

	auto r = new BufferCanvas(Coord(height, width));

	foreach(row, rp; row_pointers) {
		for(int i = 0; i < row_bytes; i++) {
			auto b = rp[i];
			//writef("%1d %1d ", (b >> 4), (b & 0x0f));
			r[row, i * 2] = (b >> 4);
			r[row, i * 2 + 1] = (b & 0x0f);
		}
		//writeln;
	}

	return r;
}


