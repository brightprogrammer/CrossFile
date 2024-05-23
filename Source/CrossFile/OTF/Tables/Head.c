/**
 * @file Head.c
 * @date Wed, 22nd May 2024
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

#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Head.h>

/* libc */
#include <time.h>

#include "Anvie/Common.h"

static inline Char* mac_style_flag_pprint (XfOtfMacStyleFlags mac_style, Char* buf, Size size);
static inline Char* head_flags_pprint (XfOtfHeadFlags flags, Char* buf, Size size);
static inline Char* font_direction_hint_pprint (XfOtfFontDirectionHint hint, Char* buf, Size size);

/**************************************************************************************************/
/*********************************** PUBLIC METHOD DEFINITIONS ************************************/
/**************************************************************************************************/

/**
 * @b Initialize @c XfOtfHead structure with proper endianness.
 *
 * @param head Reference to @c XfOtfHead structure where initialization
 *        will take place.
 * @param data Reference to raw data that needs to be adjusted before loading.
 *
 * @return @c head on success.
 * @return @c Null otherwise.
 * */
XfOtfHead* xf_otf_head_init (XfOtfHead* head, Uint8* data) {
    RETURN_VALUE_IF (!head || !data, Null, ERR_INVALID_ARGUMENTS);

    /* load and check validity of version numbers */
    head->major_version = GET_AND_ADV_U2 (data);
    head->minor_version = GET_AND_ADV_U2 (data);
    RETURN_VALUE_IF (
        head->major_version != 1 && head->minor_version != 0,
        Null,
        "Invalid version of font header table \"head\".\n"
    );

    head->font_revision       = GET_AND_ADV_U4 (data);
    head->checksum_adjustment = GET_AND_ADV_U4 (data);

    /* load and check magic number */
    head->magic_number = GET_AND_ADV_U4 (data);
    RETURN_VALUE_IF (
        head->magic_number != OTF_MAGIC_NUMBER,
        Null,
        "Invalid magic number in font header table \"head\".\n"
    );

    head->flags = GET_AND_ADV_U2 (data);

    head->units_per_em = GET_AND_ADV_U2 (data);
    RETURN_VALUE_IF (
        head->units_per_em < 16 || head->units_per_em > 16384,
        Null,
        "Invalid value of unitsPerEm in font header table \"head\". \n"
    );

    head->created             = GET_AND_ADV_U8 (data);
    head->modified            = GET_AND_ADV_U8 (data);
    head->x_min               = GET_AND_ADV_I2 (data);
    head->y_min               = GET_AND_ADV_I2 (data);
    head->x_max               = GET_AND_ADV_I2 (data);
    head->y_max               = GET_AND_ADV_I2 (data);
    head->mac_style           = GET_AND_ADV_U2 (data);
    head->lowest_rec_ppem     = GET_AND_ADV_U2 (data);
    head->font_direction_hint = GET_AND_ADV_I2 (data);
    head->index_to_loc_format = GET_AND_ADV_I2 (data);
    head->glyph_data_format   = GET_AND_ADV_I2 (data);

    return head;
}

/**
 * @b Pretty print given @c XfOtfHead structure.
 *
 * @param head
 *
 * @return @c head on success.
 * @return @c Null otherwise.
 * */
XfOtfHead* xf_otf_head_pprint (XfOtfHead* head) {
    RETURN_VALUE_IF (!head, Null, ERR_INVALID_ARGUMENTS);

    // Create a tm structure for January 1, 1904
    struct tm epoch_start = {0};
    epoch_start.tm_year   = 4; // Years since 1900 (1904)
    epoch_start.tm_mon    = 0; // January
    epoch_start.tm_mday   = 1; // 1st
    epoch_start.tm_hour   = 0;
    epoch_start.tm_min    = 0;
    epoch_start.tm_sec    = 0;

    time_t epoch_offset = mktime (&epoch_start);

    Char       created_time_buf[20] = {0};
    time_t     created_time         = ((time_t)head->created) + epoch_offset;
    struct tm* created_tm           = localtime (&created_time);
    strftime (created_time_buf, sizeof (created_time_buf), "%Y-%m-%d %H:%M:%S", created_tm);

    Char       modified_time_buf[20] = {0};
    time_t     modified_time         = ((time_t)head->modified) + epoch_offset;
    struct tm* modified_tm           = localtime (&modified_time);
    strftime (modified_time_buf, sizeof (modified_time_buf), "%Y-%m-%d %H:%M:%S", modified_tm);

    Char mac_style_pprinted_buf[80] = {0};
    mac_style_flag_pprint (head->mac_style, mac_style_pprinted_buf, sizeof mac_style_pprinted_buf);

    Char head_flags_pprinted_buf[300] = {0};
    head_flags_pprint (head->flags, head_flags_pprinted_buf, sizeof head_flags_pprinted_buf);

    Char font_direction_hint_pprinted_buf[21] = {0};
    font_direction_hint_pprint (
        head->font_direction_hint,
        font_direction_hint_pprinted_buf,
        sizeof font_direction_hint_pprinted_buf
    );

    printf (
        "OTF Font Header Table : \n"
        "\tmajor_version = %u\n"
        "\tminor_version = %u\n"
        "\tfont_revision = %u\n"
        "\tchecksum_adjustment = 0x%08x\n"
        "\tmagic_number = 0x%08x\n"
        "\tflags = %s\n"
        "\tunits_per_em = %u\n"
        "\tcreated = %s\n"
        "\tmodified = %s\n"
        "\tx_min = %d\n"
        "\ty_min = %d\n"
        "\tx_max = %d\n"
        "\ty_max = %d\n"
        "\tmac_style = %s\n"
        "\tlowest_rec_ppem = %u\n"
        "\tfont_direction_hint = %s\n"
        "\tindex_to_loc_format = %s\n"
        "\tglyph_data_format = %s\n",
        head->major_version,
        head->minor_version,
        head->font_revision,
        head->checksum_adjustment,
        head->magic_number,
        head_flags_pprinted_buf,
        head->units_per_em,
        created_time_buf,
        modified_time_buf,
        head->x_min,
        head->y_min,
        head->x_max,
        head->y_max,
        mac_style_pprinted_buf,
        head->lowest_rec_ppem,
        font_direction_hint_pprinted_buf,
        head->index_to_loc_format ? "LONG" : "SHORT",
        head->glyph_data_format ? "UNKNOWN" : "CURRENT"
    );

    return head;
}

/**************************************************************************************************/
/*********************************** PRIVATE METHOD DEFINITIONS ***********************************/
/**************************************************************************************************/

/**
 * @b Pretty print given mac style flag bits to given buffer of given size.
 *
 * @param mac_style Mac style flag bits to be printed.
 * @param buf Buffer where mac style will be pretty printed.
 * @param size Size of buffer.
 *
 * @return @c buf on success.
 * @return @c Null otherwise.
 * */
static inline Char* mac_style_flag_pprint (XfOtfMacStyleFlags mac_style, Char* buf, Size size) {
    RETURN_VALUE_IF (!buf || !size, Null, ERR_INVALID_ARGUMENTS);

    Bool add_pipe = False;

#define XF_OTF_PPRINT_FLAG(name)                                                                   \
    if (mac_style & XF_OTF_MAC_STYLE_FLAG_##name) {                                                \
        Size printed_size = snprintf (buf, size, "%s%s", add_pipe ? " | " : "", #name);            \
        if (!printed_size || size == printed_size) {                                               \
            return buf;                                                                            \
        }                                                                                          \
        size     -= printed_size;                                                                  \
        buf      += printed_size;                                                                  \
        add_pipe  = True;                                                                          \
    }

    XF_OTF_PPRINT_FLAG (BOLD);
    XF_OTF_PPRINT_FLAG (ITALIC);
    XF_OTF_PPRINT_FLAG (UNDERLINE);
    XF_OTF_PPRINT_FLAG (OUTLINE);
    XF_OTF_PPRINT_FLAG (SHADOW);
    XF_OTF_PPRINT_FLAG (CONDENSED);
    XF_OTF_PPRINT_FLAG (EXTENDED);

#undef XF_OTF_PPRINT_FLAG

    return buf;
}

/**
 * @b Pretty print given head flags bits to given buffer of given size.
 *
 * @param flags Value of flags field in font header table. 
 * @param buf Buffer where flags will be pretty printed.
 * @param size Size of buffer.
 *
 * @return @c buf on success.
 * @return @c Null otherwise.
 * */
static inline Char* head_flags_pprint (XfOtfHeadFlags flags, Char* buf, Size size) {
    RETURN_VALUE_IF (!buf || !size, Null, ERR_INVALID_ARGUMENTS);

    Bool add_pipe = False;

#define XF_OTF_PPRINT_FLAG(name)                                                                   \
    if (flags & XF_OTF_HEAD_FLAG_##name) {                                                         \
        Size printed_size = snprintf (buf, size, "%s%s", add_pipe ? " | " : "", #name);            \
        if (!printed_size || size == printed_size) {                                               \
            return buf;                                                                            \
        }                                                                                          \
        size     -= printed_size;                                                                  \
        buf      += printed_size;                                                                  \
        add_pipe  = True;                                                                          \
    }


    XF_OTF_PPRINT_FLAG (FONT_BASELINE_Y_EQ_0);
    XF_OTF_PPRINT_FLAG (FONT_LEFT_SIDEBAR_X_EQ_0);
    XF_OTF_PPRINT_FLAG (INSNS_DEPEND_ON_POINT_SIZE);
    XF_OTF_PPRINT_FLAG (FORCE_PPEM_TO_INT);
    XF_OTF_PPRINT_FLAG (INSNS_ALTER_ADVANCE_WIDTH);
    XF_OTF_PPRINT_FLAG (LOSSLESS);
    XF_OTF_PPRINT_FLAG (CONVERTED);
    XF_OTF_PPRINT_FLAG (FONT_OPTIMIZED_FOR_CLEAR_TYPE);
    XF_OTF_PPRINT_FLAG (LAST_RESORT_FONT);

#undef XF_OTF_PPRINT_FLAG

    return buf;
}

/**
 * @b Pretty print given head flags bits to given buffer of given size.
 *
 * @param flags Value of flags field in font header table. 
 * @param buf Buffer where flags will be pretty printed.
 * @param size Size of buffer.
 *
 * @return @c buf on success.
 * @return @c Null otherwise.
 * */
static inline Char* font_direction_hint_pprint (XfOtfFontDirectionHint hint, Char* buf, Size size) {
    RETURN_VALUE_IF (!buf || !size, Null, ERR_INVALID_ARGUMENTS);

#define XF_OTF_PPRINT_FLAG(name)                                                                   \
    else if (hint == XF_OTF_FONT_DIRECTION_HINT_##name) {                                          \
        Size printed_size = snprintf (buf, size, #name);                                           \
        if (!printed_size || size == printed_size) {                                               \
            return buf;                                                                            \
        }                                                                                          \
        size -= printed_size;                                                                      \
        buf  += printed_size;                                                                      \
    }

    /* dummy check to activate the following macros */
    if (False) {}

    XF_OTF_PPRINT_FLAG (LEFT_TO_RIGHT)
    XF_OTF_PPRINT_FLAG (LEFT_TO_RIGHT_STRONG)
    XF_OTF_PPRINT_FLAG (FULLY_MIXED)
    XF_OTF_PPRINT_FLAG (RIGHT_TO_LEFT)
    XF_OTF_PPRINT_FLAG (RIGHT_TO_LEFT_STRONG)

    return buf;
}
