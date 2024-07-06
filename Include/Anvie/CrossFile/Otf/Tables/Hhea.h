/**
 * @file Hhea.h
 * @date Sun, 26th May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_HHEA_H
#define ANVIE_CROSSFILE_OTF_TABLES_HHEA_H

#include <Anvie/Types.h>

/**
 * @b Horizontal Header Table.
 *
 * REF :  https://learn.microsoft.com/en-us/typography/opentype/spec/hhea
 * */
typedef struct OtfHhea {
    Uint16 major_version;
    Uint16 minor_version;
    Int16  ascender;
    Int16  descender;
    Int16  line_gap;
    Uint16 advance_width_max;
    Int16  min_left_side_bearing;
    Int16  min_right_side_bearing;
    Int16  x_max_extent;
    Int16  caret_slope_rise;
    Int16  caret_slope_run;
    Int16  caret_offset;
    Int16  reserved[4];
    Int16  metric_data_format;
    Uint16 number_of_h_metrics;
} OtfHhea;

OtfHhea *otf_hhea_init (OtfHhea *hhea, Uint8 *data, Size size);
OtfHhea *otf_hhea_pprint (OtfHhea *hhea, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_HHEA_H
