/**
 * @file Os2.h
 * @date Thu, 30th May 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2024 Siddharth Mishra
 * @copyright Copyright 2024 Anvie Labs
 *
 * Copyright 2024 Siddharth Mishra, Anvie Labs
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 *    and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse
 *    or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * */

#ifndef ANVIE_CROSSFILE_OTF_TABLES_OS2_H
#define ANVIE_CROSSFILE_OTF_TABLES_OS2_H

#include <Anvie/Types.h>

/**
 * @b Existence of some of the fields depend on the version of table.
 *
 * Version 5 covers all fields but other lower versions leave out
 * last few. Version 2, 3 and 4 have same fields with probably different
 * semantics.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/os2#ur
 * */
typedef struct XfOtfOs2 {
    /* v0 */
    Uint16 version;
    Int16  x_avg_char_width;
    Uint16 weight_class;
    Uint16 width_class;
    Uint16 type;
    Int16  y_subscript_x_size;
    Int16  y_subscript_y_size;
    Int16  y_subscript_x_offset;
    Int16  y_subscript_y_offset;
    Int16  y_superscript_x_size;
    Int16  y_superscript_y_size;
    Int16  y_superscript_x_offset;
    Int16  y_superscript_y_offset;
    Int16  y_strikeout_size;
    Int16  y_strikeout_position;
    Int16  family_class;
    Uint8  panose[10];
    Uint32 unicode_range[4];
    Char   vend_id[4];
    Uint16 selection;
    Uint16 first_char_index;
    Uint16 last_char_index;
    Int16  typo_ascender;
    Int16  typo_descender;
    Int16  typo_line_gap;
    Uint16 win_ascent;
    Uint16 win_descent;

    /* v1 */
    Uint32 code_page_range[2];

    /* v2, v3, v4*/
    Int16  x_height;
    Int16  cap_height;
    Uint16 default_char;
    Uint16 break_char;
    Uint16 max_context;

    /* v5 */
    Uint16 lower_optical_point_size;
    Uint16 upper_optical_point_size;
} XfOtfOs2;

XfOtfOs2 *xf_otf_os2_init (XfOtfOs2 *os2, Uint8 *data, Size size);
XfOtfOs2 *xf_otf_os2_pprint (XfOtfOs2 *os2, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_OS2_H
