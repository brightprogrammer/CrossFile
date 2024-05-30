/**
 * @file Os2.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Os2.h>

/* libc */
#include <memory.h>

#define OS2_V0_DATA_SIZE                                                                           \
    (sizeof (Uint16) * 16 + sizeof (Uint8) * 10 + sizeof (Uint32) * 4 + sizeof (Char) * 4 +        \
     sizeof (Uint16) * 8)

#define OS2_V1_DATA_SIZE (OS2_V0_DATA_SIZE + sizeof (Uint32) * 2)

#define OS2_V2_DATA_SIZE (OS2_V1_DATA_SIZE + sizeof (Uint16) * 5)
#define OS2_V3_DATA_SIZE OS2_V2_DATA_SIZE
#define OS2_V4_DATA_SIZE OS2_V2_DATA_SIZE

#define OS2_V5_DATA_SIZE (OS2_V4_DATA_SIZE + sizeof (Uint16) * 2)

XfOtfOs2 *xf_otf_os2_init (XfOtfOs2 *os2, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!os2 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < sizeof (Uint16),
        Null,
        "Data buffer size not sufficient to initialize os/2 (windows metrics) table\n"
    );

    os2->version = GET_AND_ADV_U2 (data);

    switch (os2->version) {
        case 0 :
            RETURN_VALUE_IF (
                size < OS2_V0_DATA_SIZE,
                Null,
                "Data buffer size not sufficient to initialize OS/2 (windows metrics) table\n"
            );
            break;

        case 1 :
            RETURN_VALUE_IF (
                size < OS2_V1_DATA_SIZE,
                Null,
                "Data buffer size not sufficient to initialize OS/2 (windows metrics) table\n"
            );
            break;

        case 2 :
        case 3 :
        case 4 :
            RETURN_VALUE_IF (
                size < OS2_V4_DATA_SIZE,
                Null,
                "Data buffer size not sufficient to initialize OS/2 (windows metrics) table\n"
            );
            break;

        case 5 :
            RETURN_VALUE_IF (
                size < OS2_V5_DATA_SIZE,
                Null,
                "Data buffer size not sufficient to initialize OS/2 (windows metrics) table\n"
            );
            break;

        default :
            PRINT_ERR ("Invalid OS/2 table version\n");
            return Null;
    }

    os2->x_avg_char_width       = GET_AND_ADV_I2 (data);
    os2->us_weight_class        = GET_AND_ADV_U2 (data);
    os2->us_width_class         = GET_AND_ADV_U2 (data);
    os2->type                   = GET_AND_ADV_U2 (data);
    os2->y_subscript_x_size     = GET_AND_ADV_I2 (data);
    os2->y_subscript_y_size     = GET_AND_ADV_I2 (data);
    os2->y_subscript_x_offset   = GET_AND_ADV_I2 (data);
    os2->y_subscript_y_offset   = GET_AND_ADV_I2 (data);
    os2->y_superscript_x_size   = GET_AND_ADV_I2 (data);
    os2->y_superscript_y_size   = GET_AND_ADV_I2 (data);
    os2->y_superscript_x_offset = GET_AND_ADV_I2 (data);
    os2->y_superscript_y_offset = GET_AND_ADV_I2 (data);
    os2->y_strikeout_size       = GET_AND_ADV_I2 (data);
    os2->y_strikeout_position   = GET_AND_ADV_I2 (data);
    os2->family_class           = GET_AND_ADV_I2 (data);

    memcpy (os2->panose, data, 10);
    data += 10;

    /* TODO: work from here */
    
    os2->unicode_range[4]         = GET_ARR_AND_ADV_U2 (data, 0, 4);
    os2->vend_id[4]               = GET_AND_ADV_U2 (data);
    os2->selection                = GET_AND_ADV_U2 (data);
    os2->first_char_index         = GET_AND_ADV_U2 (data);
    os2->last_char_index          = GET_AND_ADV_U2 (data);
    os2->typo_ascender            = GET_AND_ADV_U2 (data);
    os2->typo_descender           = GET_AND_ADV_U2 (data);
    os2->typo_line_gap            = GET_AND_ADV_U2 (data);
    os2->win_ascent               = GET_AND_ADV_U2 (data);
    os2->win_descent              = GET_AND_ADV_U2 (data);
    os2->code_page_range[2]       = GET_AND_ADV_U2 (data);
    os2->x_height                 = GET_AND_ADV_U2 (data);
    os2->cap_height               = GET_AND_ADV_U2 (data);
    os2->default_char             = GET_AND_ADV_U2 (data);
    os2->break_char               = GET_AND_ADV_U2 (data);
    os2->max_context              = GET_AND_ADV_U2 (data);
    os2->lower_optical_point_size = GET_AND_ADV_U2 (data);
    os2->upper_optical_point_size = GET_AND_ADV_U2 (data);

    return os2;
}
