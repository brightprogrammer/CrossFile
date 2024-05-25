/**
 * @file Cmap.h
 * @date Fri, 24th May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_CMAP_H
#define ANVIE_CROSSFILE_OTF_TABLES_CMAP_H

#include <Anvie/Types.h>

typedef struct XfOtfCmapSubHeader {
    Uint16 first_code;
    Uint16 entry_count;
    Int16  id_delta;
    Uint16 id_range_offset;
} XfOtfCmapSubHeader;

/**
 * @b Same struct represents both SequentialMapGroup and ConstantMapGroup
 *  */
typedef struct XfOtfCmapMapGroup {
    Uint32 start_char_code;
    Uint32 end_char_code;
    union {
        Uint32 glyph_id;
        Uint32 start_glyph_id;
    };
} XfOtfCmapMapGroup;

typedef struct XfOtfCmapUnicodeRange {
    /* HACK : This union is a hack to emulate uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 start_unicode_value;
    };
    Uint8 additional_count;
} XfOtfCmapUnicodeRange;

typedef struct XfOtfCmapDefaultUVSTable {
    Uint32                 num_unicode_value_ranges;
    XfOtfCmapUnicodeRange* ranges;
} XfOtfCmapDefaultUVSTable;

typedef struct XfOtfCmapUVSMaping {
    /* HACK : This union is a hack to emulate uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 unicode_value;
    };
    Uint16 glyph_id;
} XfOtfCmapUVSMapping;

typedef struct XfOtfCmapNonDefaultUVSTable {
    Uint32               num_uvs_mappings;
    XfOtfCmapUVSMapping* uvs_mappings;
} XfOtfCmapNonDefaultUVSTable;

typedef struct XfOtfCmapVarSelector {
    /* HACK: This union is a hack to implement Uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 var_selector;
    };
    Uint32 default_uvs_offset;
    Uint32 non_default_uvs_offset;

    XfOtfCmapDefaultUVSTable    default_uvs_table;
    XfOtfCmapNonDefaultUVSTable non_default_uvs_table;
} XfOtfCmapVarSelector;

typedef struct XfOtfCmapSubTableFormat0 {
    Uint16 length;
    Uint16 language;
    Uint8  glyph_id_array[256];
} XfOtfCmapSubTableFormat0;

typedef struct XfOtfCmapSubTableFormat2 {
    Uint16              length;
    Uint16              language;
    Uint16              sub_header_keys[256];
    XfOtfCmapSubHeader* sub_headers;
    Uint16*             glyph_id_array;
} XfOtfCmapSubTableFormat2;

typedef struct XfOtfCmapSubTableFormat4 {
    Uint16  length;
    Uint16  language;
    Uint16  seg_count;
    Uint16  search_range;
    Uint16  entry_selector;
    Uint16  range_shift;
    Uint16* end_code;
    Uint16  reserved_pad;
    Uint16* start_code;
    Uint16* id_delta;
    Uint16* id_range_offsets;
    Uint16* glyph_id_array;
} XfOtfCmapSubTableFormat4;

typedef struct XfOtfCmapSubTableFormat6 {
    Uint16  length;
    Uint16  language;
    Uint16  first_code;
    Uint16  entry_count;
    Uint16* glyph_id_array;
} XfOtfCmapSubTableFormat6;

typedef struct XfOtfCmapSubTableFormat8 {
    Uint16             reserved;
    Uint32             length;
    Uint32             language;
    Uint8              is32[8192];
    Uint32             num_groups;
    XfOtfCmapMapGroup* groups;
} XfOtfCmapSubTableFormat8;

typedef struct XfOtfCmapSubTableFormat10 {
    Uint16  reserved;
    Uint32  length;
    Uint32  language;
    Uint32  start_char_code;
    Uint32  num_chars;
    Uint16* glyph_id_array;
} XfOtfCmapSubTableFormat10;

typedef struct XfOtfCmapSubTableFormat12 {
    Uint16             reserved;
    Uint32             length;
    Uint32             language;
    Uint32             num_groups;
    XfOtfCmapMapGroup* groups;
} XfOtfCmapSubTableFormat12, XfOtfCmapSubTableFormat13;

typedef struct XfOtfCmapSubTableFormat14 {
    Uint32                length;
    Uint32                num_var_selectors;
    XfOtfCmapVarSelector* var_selectors;
} XfOtfCmapSubTableFormat14;

/**
 * @b Definition of different sub tables in cmap table based on format.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#format-0-byte-encoding-table
 * */
typedef struct XfOtfCmapSubTable {
    Uint16 format;
    union {
        XfOtfCmapSubTableFormat0*  format0;
        XfOtfCmapSubTableFormat2*  format2;
        XfOtfCmapSubTableFormat4*  format4;
        XfOtfCmapSubTableFormat6*  format6;
        XfOtfCmapSubTableFormat8*  format8;
        XfOtfCmapSubTableFormat10* format10;
        XfOtfCmapSubTableFormat12* format12;
        XfOtfCmapSubTableFormat13* format13;
        XfOtfCmapSubTableFormat14* format14;
    };
} XfOtfCmapSubTable;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#platform-ids
 * */
typedef enum XfOtfCmapPlatformId : Uint16 {
    XF_OTF_CMAP_PLATFORM_ID_MIN = 0,

    XF_OTF_CMAP_PLATFORM_ID_VARIOUS = 0,
    XF_OTF_CMAP_PLATFORM_ID_MAC     = 1,
    XF_OTF_CMAP_PLATFORM_ID_ISO     = 2,
    XF_OTF_CMAP_PLATFORM_ID_WIN     = 3,
    XF_OTF_CMAP_PLATFORM_ID_CUSTOM  = 4,

    XF_OTF_CMAP_PLATFORM_ID_MAX = 4
} XfOtfCmapPlatformId;

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
typedef enum XfOtfCmapVariousEncodingId : Uint16 {
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_MIN = 0,

    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_10                 = 0,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_11                 = 1,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_ISO_IEC_10646              = 2,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_20_BMP_ONLY        = 3,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_20_FULL_REPERTOIRE = 4,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_VARIATION_SEQ      = 5,
    XF_OTF_CMAP_VARIOUS_ENCODING_ID_UNICODE_FULL_REPERTOIRE    = 6,

    XF_OTF_CMAP_VARIOUS_ENCODING_ID_MAX = 6
} XfOtfCmapVariousEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/name#macintosh-encoding-ids-script-manager-codes
 * */
typedef enum XfOtfCmapMacEncodingId : Uint16 {
    XF_OTF_CMAP_MAC_ENCODING_ID_MIN = 0,

    XF_OTF_CMAP_MAC_ENCODING_ID_ROMAN               = 0,
    XF_OTF_CMAP_MAC_ENCODING_ID_JAPANESE            = 1,
    XF_OTF_CMAP_MAC_ENCODING_ID_CHINESE_TRADITIONAL = 2,
    XF_OTF_CMAP_MAC_ENCODING_ID_KOREAN              = 3,
    XF_OTF_CMAP_MAC_ENCODING_ID_ARABIC              = 4,
    XF_OTF_CMAP_MAC_ENCODING_ID_HEBREW              = 5,
    XF_OTF_CMAP_MAC_ENCODING_ID_GREEK               = 6,
    XF_OTF_CMAP_MAC_ENCODING_ID_RUSSIAN             = 7,
    XF_OTF_CMAP_MAC_ENCODING_ID_RSYMBOL             = 8,
    XF_OTF_CMAP_MAC_ENCODING_ID_DEVANAGARI          = 9,
    XF_OTF_CMAP_MAC_ENCODING_ID_GURMUKHI            = 10,
    XF_OTF_CMAP_MAC_ENCODING_ID_GUJARATI            = 11,
    XF_OTF_CMAP_MAC_ENCODING_ID_ODIA                = 12,
    XF_OTF_CMAP_MAC_ENCODING_ID_BANGLA              = 13,
    XF_OTF_CMAP_MAC_ENCODING_ID_TAMIL               = 14,
    XF_OTF_CMAP_MAC_ENCODING_ID_TELUGU              = 15,
    XF_OTF_CMAP_MAC_ENCODING_ID_KANNADA             = 16,
    XF_OTF_CMAP_MAC_ENCODING_ID_MALAYALAM           = 17,
    XF_OTF_CMAP_MAC_ENCODING_ID_SINHALESE           = 18,
    XF_OTF_CMAP_MAC_ENCODING_ID_BURMESE             = 19,
    XF_OTF_CMAP_MAC_ENCODING_ID_KHMER               = 20,
    XF_OTF_CMAP_MAC_ENCODING_ID_THAI                = 21,
    XF_OTF_CMAP_MAC_ENCODING_ID_LAOTIAN             = 22,
    XF_OTF_CMAP_MAC_ENCODING_ID_GEORGIAN            = 23,
    XF_OTF_CMAP_MAC_ENCODING_ID_ARMENIAN            = 24,
    XF_OTF_CMAP_MAC_ENCODING_ID_CHINESE_SIMPLIFIED  = 25,
    XF_OTF_CMAP_MAC_ENCODING_ID_TIBETAN             = 26,
    XF_OTF_CMAP_MAC_ENCODING_ID_MONGOLIAN           = 27,
    XF_OTF_CMAP_MAC_ENCODING_ID_GEEZ                = 28,
    XF_OTF_CMAP_MAC_ENCODING_ID_SLAVIC              = 29,
    XF_OTF_CMAP_MAC_ENCODING_ID_VIETNAMESE          = 30,
    XF_OTF_CMAP_MAC_ENCODING_ID_SINDHI              = 31,
    XF_OTF_CMAP_MAC_ENCODING_ID_UNINTERPRETED       = 32,

    XF_OTF_CMAP_MAC_ENCODING_ID_MAX = 32
} XfOtfCmapMacEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#iso-platform-platform-id--2
 * */
typedef enum XfOtfCmapIsoEncodingId : Uint16 {
    XF_OTF_CMAP_ISO_ENCODING_ID_MIN = 0,

    XF_OTF_CMAP_ISO_ENCODING_ID_7_BIT_ASCII = 0,
    XF_OTF_CMAP_ISO_ENCODING_ID_ISO_10646   = 1,
    XF_OTF_CMAP_ISO_ENCODING_ID_ISO_8859_1  = 2,

    XF_OTF_CMAP_ISO_ENCODING_ID_MAX = 2,
} XfOtfCmapIsoEncodingId;

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#windows-platform-platform-id--3
 * */
typedef enum XfOtfCmapWinEncodingId : Uint16 {
    XF_OTF_CMAP_WIN_ENCODING_ID_MIN = 0,

    XF_OTF_CMAP_WIN_ENCODING_ID_SYMBOL                  = 0,
    XF_OTF_CMAP_WIN_ENCODING_ID_UNICODE_BMP             = 1,
    XF_OTF_CMAP_WIN_ENCODING_ID_SHIFTJIS                = 2,
    XF_OTF_CMAP_WIN_ENCODING_ID_PRC                     = 3,
    XF_OTF_CMAP_WIN_ENCODING_ID_BIG5                    = 4,
    XF_OTF_CMAP_WIN_ENCODING_ID_WANSUNG                 = 5,
    XF_OTF_CMAP_WIN_ENCODING_ID_JOHAB                   = 6,
    XF_OTF_CMAP_WIN_ENCODING_ID_RESERVED_7              = 7,
    XF_OTF_CMAP_WIN_ENCODING_ID_RESERVED_8              = 8,
    XF_OTF_CMAP_WIN_ENCODING_ID_RESERVED_9              = 9,
    XF_OTF_CMAP_WIN_ENCODING_ID_UNICODE_FULL_REPERTOIRE = 10,

    XF_OTF_CMAP_WIN_ENCODING_ID_MAX = 10,
} XfOtfCmapWinEncodingId;

#define XF_OTF_CMAP_CUSTOM_ENCODING_ID_MIN 0
#define XF_OTF_CMAP_CUSTOM_ENCODING_ID_MAX 255

/**
 * The array of encoding records specifies particular encodings and the offset to the
 * subtable for each encoding.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#encoding-records-and-encodings
 * */
typedef struct XfOtfCmapEncodingRecord {
    XfOtfCmapPlatformId platform_id;
    union {
        XfOtfCmapVariousEncodingId various;
        XfOtfCmapMacEncodingId     mac;
        XfOtfCmapIsoEncodingId     iso;
        XfOtfCmapWinEncodingId     win;
        Uint16 custom; /**< @b For legacy support, must be ignored in newer fonts. */
    } encoding_id;
    Uint32            sub_table_offset;
    XfOtfCmapSubTable sub_table;
} XfOtfCmapEncodingRecord;

/**
 * @b This table defines the mapping of character codes to the glyph index values used in the font.
 * It may contain more than one subtable, in order to support more than one character encoding scheme.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#cmap-header
 * */
typedef struct XfOtfCmap {
    Uint16                   version;
    Uint16                   num_tables;
    XfOtfCmapEncodingRecord* encoding_records;
} XfOtfCmap;

XfOtfCmap* xf_otf_cmap_init (XfOtfCmap* cmap, Uint8* data, Size size);
XfOtfCmap* xf_otf_cmap_deinit (XfOtfCmap* cmap);
XfOtfCmap* xf_otf_cmap_pprint (XfOtfCmap* cmap);

#endif // ANVIE_CROSSFILE_OTF_TABLES_CMAP_H
