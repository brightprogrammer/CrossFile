/**
 * @file Common.h
 * @date Mon, 27th May 2024
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

/*
 * This file exists here to define common types, enums, defines, etc... that are shared
 * by all tables and is defined in the spec.
 * */

#ifndef ANVIE_CROSSFILE_OTF_TABLES_COMMON_H
#define ANVIE_CROSSFILE_OTF_TABLES_COMMON_H

#include <Anvie/Types.h>

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#platform-ids
 * */
typedef enum XfOtfPlatformId : Uint16 {
    XF_OTF_PLATFORM_ID_MIN = 0,

    XF_OTF_PLATFORM_ID_VARIOUS = 0,
    XF_OTF_PLATFORM_ID_MAC     = 1,
    XF_OTF_PLATFORM_ID_ISO     = 2,
    XF_OTF_PLATFORM_ID_WIN     = 3,
    XF_OTF_PLATFORM_ID_CUSTOM  = 4,

    XF_OTF_PLATFORM_ID_MAX = 4
} XfOtfPlatformId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#unicode-platform-platform-id--0 
 * 
 * - Use of encoding IDs 0, 1 or 2 is deprecated.
 * - Encoding ID 3 should be used in conjunction with 'cmap' subtable formats 4 or 6.
 * - Encoding ID 4 should be used in conjunction with subtable formats 10 or 12.
 * 
 * - Unicode Variation Sequences supported by the font should be specified in the 'cmap'
 *   table using a format 14 subtable.
 *
 * - A format 14 subtable must only be used under platform ID 0 and encoding ID 5; 
 * - Encoding ID 5 should only be used with a format 14 subtable.
 * 
 * - Encoding ID 6 should only be used in conjunction with 'cmap' subtable format 13;
 * - Subtable format 13 should only be used under platform ID 0 and encoding ID 6.
 * */
typedef enum XfOtfVariousEncodingId : Uint16 {
    XF_OTF_VARIOUS_ENCODING_ID_MIN = 0,

    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_10                 = 0,
    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_11                 = 1,
    XF_OTF_VARIOUS_ENCODING_ID_ISO_IEC_10646              = 2,
    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_20_BMP_ONLY        = 3,
    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_20_FULL_REPERTOIRE = 4,
    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_VARIATION_SEQ      = 5,
    XF_OTF_VARIOUS_ENCODING_ID_UNICODE_FULL_REPERTOIRE    = 6,

    XF_OTF_VARIOUS_ENCODING_ID_MAX = 6
} XfOtfVariousEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/name#macintosh-encoding-ids-script-manager-codes
 * */
typedef enum XfOtfMacEncodingId : Uint16 {
    XF_OTF_MAC_ENCODING_ID_MIN = 0,

    XF_OTF_MAC_ENCODING_ID_ROMAN               = 0,
    XF_OTF_MAC_ENCODING_ID_JAPANESE            = 1,
    XF_OTF_MAC_ENCODING_ID_CHINESE_TRADITIONAL = 2,
    XF_OTF_MAC_ENCODING_ID_KOREAN              = 3,
    XF_OTF_MAC_ENCODING_ID_ARABIC              = 4,
    XF_OTF_MAC_ENCODING_ID_HEBREW              = 5,
    XF_OTF_MAC_ENCODING_ID_GREEK               = 6,
    XF_OTF_MAC_ENCODING_ID_RUSSIAN             = 7,
    XF_OTF_MAC_ENCODING_ID_RSYMBOL             = 8,
    XF_OTF_MAC_ENCODING_ID_DEVANAGARI          = 9,
    XF_OTF_MAC_ENCODING_ID_GURMUKHI            = 10,
    XF_OTF_MAC_ENCODING_ID_GUJARATI            = 11,
    XF_OTF_MAC_ENCODING_ID_ODIA                = 12,
    XF_OTF_MAC_ENCODING_ID_BANGLA              = 13,
    XF_OTF_MAC_ENCODING_ID_TAMIL               = 14,
    XF_OTF_MAC_ENCODING_ID_TELUGU              = 15,
    XF_OTF_MAC_ENCODING_ID_KANNADA             = 16,
    XF_OTF_MAC_ENCODING_ID_MALAYALAM           = 17,
    XF_OTF_MAC_ENCODING_ID_SINHALESE           = 18,
    XF_OTF_MAC_ENCODING_ID_BURMESE             = 19,
    XF_OTF_MAC_ENCODING_ID_KHMER               = 20,
    XF_OTF_MAC_ENCODING_ID_THAI                = 21,
    XF_OTF_MAC_ENCODING_ID_LAOTIAN             = 22,
    XF_OTF_MAC_ENCODING_ID_GEORGIAN            = 23,
    XF_OTF_MAC_ENCODING_ID_ARMENIAN            = 24,
    XF_OTF_MAC_ENCODING_ID_CHINESE_SIMPLIFIED  = 25,
    XF_OTF_MAC_ENCODING_ID_TIBETAN             = 26,
    XF_OTF_MAC_ENCODING_ID_MONGOLIAN           = 27,
    XF_OTF_MAC_ENCODING_ID_GEEZ                = 28,
    XF_OTF_MAC_ENCODING_ID_SLAVIC              = 29,
    XF_OTF_MAC_ENCODING_ID_VIETNAMESE          = 30,
    XF_OTF_MAC_ENCODING_ID_SINDHI              = 31,
    XF_OTF_MAC_ENCODING_ID_UNINTERPRETED       = 32,

    XF_OTF_MAC_ENCODING_ID_MAX = 32
} XfOtfMacEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#iso-platform-platform-id--2
 * */
typedef enum XfOtfIsoEncodingId : Uint16 {
    XF_OTF_ISO_ENCODING_ID_MIN = 0,

    XF_OTF_ISO_ENCODING_ID_7_BIT_ASCII = 0,
    XF_OTF_ISO_ENCODING_ID_ISO_10646   = 1,
    XF_OTF_ISO_ENCODING_ID_ISO_8859_1  = 2,

    XF_OTF_ISO_ENCODING_ID_MAX = 2,
} XfOtfIsoEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#windows-platform-platform-id--3
 * */
typedef enum XfOtfWinEncodingId : Uint16 {
    XF_OTF_WIN_ENCODING_ID_MIN = 0,

    XF_OTF_WIN_ENCODING_ID_SYMBOL                  = 0,
    XF_OTF_WIN_ENCODING_ID_UNICODE_BMP             = 1,
    XF_OTF_WIN_ENCODING_ID_SHIFTJIS                = 2,
    XF_OTF_WIN_ENCODING_ID_PRC                     = 3,
    XF_OTF_WIN_ENCODING_ID_BIG5                    = 4,
    XF_OTF_WIN_ENCODING_ID_WANSUNG                 = 5,
    XF_OTF_WIN_ENCODING_ID_JOHAB                   = 6,
    XF_OTF_WIN_ENCODING_ID_RESERVED_7              = 7,
    XF_OTF_WIN_ENCODING_ID_RESERVED_8              = 8,
    XF_OTF_WIN_ENCODING_ID_RESERVED_9              = 9,
    XF_OTF_WIN_ENCODING_ID_UNICODE_FULL_REPERTOIRE = 10,

    XF_OTF_WIN_ENCODING_ID_MAX = 10,
} XfOtfWinEncodingId;

#define XF_OTF_CUSTOM_ENCODING_ID_MIN 0
#define XF_OTF_CUSTOM_ENCODING_ID_MAX 255

typedef struct XfOtfPlatformEncoding {
    XfOtfPlatformId platform_id;
    union {
        XfOtfVariousEncodingId various;
        XfOtfMacEncodingId     mac;
        XfOtfIsoEncodingId     iso;
        XfOtfWinEncodingId     win;
        Uint16                 custom;
    } encoding_id;
} XfOtfPlatformEncoding;

CString xf_otf_platform_encoding_get_platform_str (XfOtfPlatformEncoding platform_encoding);
CString xf_otf_platform_encoding_get_encoding_str (XfOtfPlatformEncoding platform_encoding);

#endif // ANVIE_CROSSFILE_OTF_TABLES_COMMON_H