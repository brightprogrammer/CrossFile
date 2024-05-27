/**
 * @file Hhea.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Hhea.h>

/* libc */
#include <memory.h>

#define HHEA_DATA_SIZE (sizeof (Uint16) * 18)

XfOtfHhea *xf_otf_hhea_init (XfOtfHhea *hhea, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!hhea || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < HHEA_DATA_SIZE,
        Null,
        "Data buffer size no sufficient to initialize hhea table\n"
    );

    hhea->major_version          = GET_AND_ADV_U2 (data);
    hhea->minor_version          = GET_AND_ADV_U2 (data);
    hhea->ascender               = GET_AND_ADV_I2 (data);
    hhea->descender              = GET_AND_ADV_I2 (data);
    hhea->line_gap               = GET_AND_ADV_I2 (data);
    hhea->advance_width_max      = GET_AND_ADV_U2 (data);
    hhea->min_left_side_bearing  = GET_AND_ADV_I2 (data);
    hhea->min_right_side_bearing = GET_AND_ADV_I2 (data);
    hhea->x_max_extent           = GET_AND_ADV_I2 (data);
    hhea->caret_slope_rise       = GET_AND_ADV_I2 (data);
    hhea->caret_slope_run        = GET_AND_ADV_I2 (data);
    hhea->caret_offset           = GET_AND_ADV_I2 (data);

    /* skip reserved fields */
    data += sizeof (Uint16) * 4;

    hhea->metric_data_format  = GET_AND_ADV_I2 (data);
    hhea->number_of_h_metrics = GET_AND_ADV_U2 (data);

    return hhea;
}
XfOtfHhea *xf_otf_hhea_pprint (XfOtfHhea *hhea, Uint8 indent_level) {
    RETURN_VALUE_IF (!hhea, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Horizontal Header Table\n"
        "|%s|major_version = %u\n"
        "|%s|minor_version = %u\n"
        "|%s|ascender = %d\n"
        "|%s|descender = %d\n"
        "|%s|line_gap = %d\n"
        "|%s|advance_width_max = %u\n"
        "|%s|min_left_side_bearing = %d\n"
        "|%s|min_right_side_bearing = %d\n"
        "|%s|x_max_extent = %d\n"
        "|%s|caret_slope_rise = %d\n"
        "|%s|caret_slope_run = %d\n"
        "|%s|caret_offset = %d\n"
        "|%s|metric_data_format = %d\n"
        "|%s|number_of_h_metrics = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        hhea->major_version,
        indent,
        hhea->minor_version,
        indent,
        hhea->ascender,
        indent,
        hhea->descender,
        indent,
        hhea->line_gap,
        indent,
        hhea->advance_width_max,
        indent,
        hhea->min_left_side_bearing,
        indent,
        hhea->min_right_side_bearing,
        indent,
        hhea->x_max_extent,
        indent,
        hhea->caret_slope_rise,
        indent,
        hhea->caret_slope_run,
        indent,
        hhea->caret_offset,
        indent,
        hhea->metric_data_format,
        indent,
        hhea->number_of_h_metrics
    );

    return hhea;
}
