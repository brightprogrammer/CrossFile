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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Head.h>

/* libc */
#include <memory.h>
#include <time.h>

static inline Char* mac_style_flag_to_str (XfOtfMacStyleFlags mac_style, Char* buf, Size size);
static inline Char* head_flags_to_str (XfOtfHeadFlags flags, Char* buf, Size size);
static inline Char* font_direction_into_to_str (XfOtfFontDirectionHint hint, Char* buf, Size size);

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
XfOtfHead* xf_otf_head_init (XfOtfHead* head, Uint8* data, Size size) {
    RETURN_VALUE_IF (!head || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < XF_OTF_HEAD_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize head table \"head\".\n"
    );

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
XfOtfHead* xf_otf_head_pprint (XfOtfHead* head, Uint8 indent_level) {
    RETURN_VALUE_IF (!head, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

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
    mac_style_flag_to_str (head->mac_style, mac_style_pprinted_buf, sizeof mac_style_pprinted_buf);

    Char head_flags_pprinted_buf[300] = {0};
    head_flags_to_str (head->flags, head_flags_pprinted_buf, sizeof head_flags_pprinted_buf);

    Char font_direction_hint_pprinted_buf[21] = {0};
    font_direction_into_to_str (
        head->font_direction_hint,
        font_direction_hint_pprinted_buf,
        sizeof font_direction_hint_pprinted_buf
    );

    printf (
        "|%.*s|OTF Font Header Table : \n"
        "|%s|major_version = %u\n"
        "|%s|minor_version = %u\n"
        "|%s|font_revision = %u\n"
        "|%s|checksum_adjustment = 0x%08x\n"
        "|%s|magic_number = 0x%08x\n"
        "|%s|flags = %s\n"
        "|%s|units_per_em = %u\n"
        "|%s|created = %s\n"
        "|%s|modified = %s\n"
        "|%s|x_min = %d\n"
        "|%s|y_min = %d\n"
        "|%s|x_max = %d\n"
        "|%s|y_max = %d\n"
        "|%s|mac_style = %s\n"
        "|%s|lowest_rec_ppem = %u\n"
        "|%s|font_direction_hint = %s\n"
        "|%s|index_to_loc_format = %s\n"
        "|%s|glyph_data_format = %s\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        head->major_version,
        indent,
        head->minor_version,
        indent,
        head->font_revision,
        indent,
        head->checksum_adjustment,
        indent,
        head->magic_number,
        indent,
        head_flags_pprinted_buf,
        indent,
        head->units_per_em,
        indent,
        created_time_buf,
        indent,
        modified_time_buf,
        indent,
        head->x_min,
        indent,
        head->y_min,
        indent,
        head->x_max,
        indent,
        head->y_max,
        indent,
        mac_style_pprinted_buf,
        indent,
        head->lowest_rec_ppem,
        indent,
        font_direction_hint_pprinted_buf,
        indent,
        head->index_to_loc_format ? "LONG" : "SHORT",
        indent,
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
static inline Char* mac_style_flag_to_str (XfOtfMacStyleFlags mac_style, Char* buf, Size size) {
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
static inline Char* head_flags_to_str (XfOtfHeadFlags flags, Char* buf, Size size) {
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
static inline Char* font_direction_into_to_str (XfOtfFontDirectionHint hint, Char* buf, Size size) {
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
