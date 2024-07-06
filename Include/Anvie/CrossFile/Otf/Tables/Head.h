/**
 * @file Head.h
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_HEAD_H
#define ANVIE_CROSSFILE_OTF_TABLES_HEAD_H

#include <Anvie/Types.h>

#define OTF_MAGIC_NUMBER ((Uint32)0x5F0F3CF5)

typedef Uint16 OtfMacStyleFlags;
typedef enum OtfMacSyleFlagBits : OtfMacStyleFlags {
    OTF_MAC_STYLE_FLAG_BOLD      = 1 << 0,
    OTF_MAC_STYLE_FLAG_ITALIC    = 1 << 1,
    OTF_MAC_STYLE_FLAG_UNDERLINE = 1 << 2,
    OTF_MAC_STYLE_FLAG_OUTLINE   = 1 << 3,
    OTF_MAC_STYLE_FLAG_SHADOW    = 1 << 4,
    OTF_MAC_STYLE_FLAG_CONDENSED = 1 << 5,
    OTF_MAC_STYLE_FLAG_EXTENDED  = 1 << 6,
    OTF_MAC_STYLE_FLAG_RESERVED  = 0xf0,
} OtfMacStyleFlagBits;

typedef Uint16 OtfHeadFlags;
typedef enum OtfHeadFlagBits : OtfHeadFlags {
    OTF_HEAD_FLAG_FONT_BASELINE_Y_EQ_0          = 1 << 0,
    OTF_HEAD_FLAG_FONT_LEFT_SIDEBAR_X_EQ_0      = 1 << 1,
    OTF_HEAD_FLAG_INSNS_DEPEND_ON_POINT_SIZE    = 1 << 2,
    OTF_HEAD_FLAG_FORCE_PPEM_TO_INT             = 1 << 3,
    OTF_HEAD_FLAG_INSNS_ALTER_ADVANCE_WIDTH     = 1 << 4,
    OTF_HEAD_FLAG_IGNORE0                       = 0x07e0, /* bits 5 to 10 are ignored in OTF */
    OTF_HEAD_FLAG_LOSSLESS                      = 1 << 11,
    OTF_HEAD_FLAG_CONVERTED                     = 1 << 12,
    OTF_HEAD_FLAG_FONT_OPTIMIZED_FOR_CLEAR_TYPE = 1 << 13,
    OTF_HEAD_FLAG_LAST_RESORT_FONT              = 1 << 14,
    OTF_HEAD_FLAG_RESERVED                      = 1 << 15
} OtfHeadFlagBits;

typedef enum OtfFontDirectionHint : Int16 {
    OTF_FONT_DIRECTION_HINT_LEFT_TO_RIGHT        = 2,
    OTF_FONT_DIRECTION_HINT_LEFT_TO_RIGHT_STRONG = 1,
    OTF_FONT_DIRECTION_HINT_FULLY_MIXED          = 0,
    OTF_FONT_DIRECTION_HINT_RIGHT_TO_LEFT_STRONG = -1,
    OTF_FONT_DIRECTION_HINT_RIGHT_TO_LEFT        = -2
} OtfFontDirectionHint;

/**
 * @b Font Header Table
 * 
 * This table gives global information about the font. The bounding box values should be
 * computed using only glyphs that have contours. Glyphs with no contours should be ignored
 * for the purposes of these calculations.
 * 
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/head
 * */
typedef struct OtfHead {
    Uint16               major_version; /**< @b Always set to 1 */
    Uint16               minor_version; /**< @b Always set to 0 */
    Uint32               font_revision; /**< @b Set by font manufacturer. */
    Uint32               checksum_adjustment;
    Uint32               magic_number;  /**< @b Always set to @c OTF_MAGIC_NUMBER */
    OtfHeadFlags         flags;
    Uint16               units_per_em;  /**< @b Some info for rasterizer */
    Uint64               created;       /**< @b Number of seconds since Jan 1st 1904 Midnight */
    Uint64               modified;      /**< @b Number of seconds since Jan 1st 1904 Midnight */
    Int16                x_min; /**< Minimum x coordinate across all glyph bounding boxes. */
    Int16                y_min; /**< Minimum y coordinate across all glyph bounding boxes. */
    Int16                x_max; /**< Maximum x coordinate across all glyph bounding boxes. */
    Int16                y_max; /**< Maximum y coordinate across all glyph bounding boxes. */
    OtfMacStyleFlags     mac_style;
    Uint16               lowest_rec_ppem;     /**< @b Smallest readable size in pixels */
    OtfFontDirectionHint font_direction_hint; /**< @b Deprecated (always set to 2). */
    Int16 index_to_loc_format; /**< @b 0 for short offsets, 1 for long offsets in "loca" */
    Int16 glyph_data_format;   /**< @b 0 for current format. */
} OtfHead;

#define OTF_HEAD_DATA_SIZE                                                                         \
    (sizeof (Uint16) * 2 + sizeof (Uint32) * 3 + sizeof (OtfHeadFlags) + sizeof (Uint16) +         \
     sizeof (Uint64) * 2 + sizeof (Int16) * 4 + sizeof (OtfMacStyleFlags) + sizeof (Uint16) +      \
     sizeof (OtfFontDirectionHint) + sizeof (Int16) * 2)

OtfHead* otf_head_init (OtfHead* head, Uint8* data, Size size);
OtfHead* otf_head_pprint (OtfHead* head, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_HEAD_H
