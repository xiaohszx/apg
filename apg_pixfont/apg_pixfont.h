/* apg_pixfont - C Pixel Font Utility v0.0.1
LICENCE: see bottom of this file
==============================================================
Authors and Contributors:
- Anton Gerdelan - @capnramses - <antongdl@protonmail.com>

What Does It Do?
==============================================================
- Tells you how much memory to allocate is required for image to fit a given string.
- Given a string of text, renders it into an image, in pre-allocated memory, using an embedded pixel font.
- Useful for quick & dirty text output in graphics software. eg FPS counters or debug-on-screen text, or pixel-art games.

What Doesn't It Do?
==============================================================
- Contain a file output API. If you want to save to a file use stb_image_write or libpng etc.
- Characters rendered are limited to those in the font - 1-byte-per-codepoint ASCII, and a limited selection of other Latin alphabet glyphs (basic accents,
umlauts).

Instructions:
==============================================================
1. Copy apg_pixfont.c and apg_pixfont.h into your project's source code files.
   You don't need to copy the image file for the font - it's embedded in the C file.

2. To get the required memory size call:

int w, h;
apg_pixfont_image_size_for_str( "my_string", &w, &h );

3. Then allocate memory

unsigned char* img_mem = (unsigned char*)malloc( w * h );

4. Then paint the string into the memory

apg_pixfont_str_into_image( "my_string", img_mem, w, h, 0 );

5. You can then free the memory

free( img_mem );

==============================================================

Technical Details:
==============================================================
- Implementation is C99, interface is C89.

TODO:
==============================================================
- Can add pixel art outlines to the text -- needs RGBA. may as well add colour here too.
- Testing building in a C++ program
- Support more glyphs

History:
==============================================================
2018 - First version by Anton, as part of voxel game project
2019 Aug 9 - Split into stand-alone library code.
*/
#ifndef _APG_PIXFONT_H_
#define _APG_PIXFONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define APG_PIXFONT_FAILURE 0
#define APG_PIXFONT_SUCCESS 1
#define APG_PIXFONT_MAX_STRLEN 2048

/* use your image loader, eg stb_image, to load a pixel font image into memory, and store a pointer to it, width, height, and number of channels here. */
struct apg_pixfont_t {
  unsigned char* img;
  int w, h;
  int n_colour_channels;
};

/* Get image dimensions required for writing full string into with str_into_image()

ARGUMENTS:
* ascii_str - null-terminated string to render. ASCII plus a few basic UTF-8 encoded Unicode Latin characters are supported.
* w,h - minimum dimensions of image require to fix the whole string.

RETURNS:
* APG_PIXFONT_FAILURE on error (zero-length strings, NULL pointer args), otherwise success

CAVEATS: image widths will always tightly fit text size
* Very short strings, eg '|' will produce images that are smaller than 4-byte-aligned memory expected by eg OpenGL textures.
  To address this you could pad the image, or set OpenGL byte alignment to 1
* If you want power-of-two sized images then allocate the next power-of-two size up.
*/
int apg_pixfont_image_size_for_str( const char* ascii_str, int* w, int* h );

/* Writes an ASCII string into a 1-channel image using the pixel font.
Allocate image memory first and clear as desired. font writes over the top in 1-channel white.

ARGUMENTS:
* ascii_str - null-terminated string to render. ASCII plus a few basic UTF-8 encoded Unicode Latin characters are supported.
* image - a pre-allocated block of memory to draw the image into. Use image_size_for_str() to find the size required for this.
* image_w, image_h - dimensions of image to write into. If text pixels extend out of these bounds it won't attempt to write anything there.
* vertically_flip - Image can be vertically flipped (0=no,1=yes) for eg OpenGL textures.

RETURNS:
* returns APG_PIXFONT_FAILURE on error, otherwise success
*/
int apg_pixfont_str_into_image( const char* ascii_str, unsigned char* image, int image_w, int image_h, int vertically_flip );

#ifdef __cplusplus
}
#endif /* extern C */

#endif

/*
-------------------------------------------------------------------------------------
This software is available under two licences - you may use it under either licence.
-------------------------------------------------------------------------------------
FIRST LICENCE OPTION

>                                  Apache License
>                            Version 2.0, January 2004
>                         http://www.apache.org/licenses/
>    Copyright 2019 Anton Gerdelan.
>    Licensed under the Apache License, Version 2.0 (the "License");
>    you may not use this file except in compliance with the License.
>    You may obtain a copy of the License at
>        http://www.apache.org/licenses/LICENSE-2.0
>    Unless required by applicable law or agreed to in writing, software
>    distributed under the License is distributed on an "AS IS" BASIS,
>    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
>    See the License for the specific language governing permissions and
>    limitations under the License.
-------------------------------------------------------------------------------------
SECOND LICENCE OPTION

> This is free and unencumbered software released into the public domain.
>
> Anyone is free to copy, modify, publish, use, compile, sell, or
> distribute this software, either in source code form or as a compiled
> binary, for any purpose, commercial or non-commercial, and by any
> means.
>
> In jurisdictions that recognize copyright laws, the author or authors
> of this software dedicate any and all copyright interest in the
> software to the public domain. We make this dedication for the benefit
> of the public at large and to the detriment of our heirs and
> successors. We intend this dedication to be an overt act of
> relinquishment in perpetuity of all present and future rights to this
> software under copyright law.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
> EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
> MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
> IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
> OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
> ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
> OTHER DEALINGS IN THE SOFTWARE.
>
> For more information, please refer to <http://unlicense.org>
-------------------------------------------------------------------------------------
*/