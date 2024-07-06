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

/* crossfile */
#include <Anvie/CrossFile/Otf/Tables/Common.h>

typedef struct OtfCmapSubHeader {
    Uint16 first_code;
    Uint16 entry_count;
    Int16  id_delta;
    Uint16 id_range_offset;
} OtfCmapSubHeader;

/**
 * @b Same struct represents both SequentialMapGroup and ConstantMapGroup
 *  */
typedef struct OtfCmapMapGroup {
    Uint32 start_char_code;
    Uint32 end_char_code;
    union {
        Uint32 glyph_id;
        Uint32 start_glyph_id;
    };
} OtfCmapMapGroup;

typedef struct OtfCmapUnicodeRange {
    /* HACK : This union is a hack to emulate uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 start_unicode_value;
    };
    Uint8 additional_count;
} OtfCmapUnicodeRange;

typedef struct OtfCmapDefaultUVSTable {
    Uint32               num_unicode_value_ranges;
    OtfCmapUnicodeRange* ranges;
} OtfCmapDefaultUVSTable;

typedef struct OtfCmapUVSMaping {
    /* HACK : This union is a hack to emulate uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 unicode_value;
    };
    Uint16 glyph_id;
} OtfCmapUVSMapping;

typedef struct OtfCmapNonDefaultUVSTable {
    Uint32             num_uvs_mappings;
    OtfCmapUVSMapping* uvs_mappings;
} OtfCmapNonDefaultUVSTable;

typedef struct OtfCmapVarSelector {
    /* HACK: This union is a hack to implement Uint24 type in C. */
    union {
        Uint8  uint24[3];
        Uint32 var_selector;
    };
    Uint32 default_uvs_offset;
    Uint32 non_default_uvs_offset;

    OtfCmapDefaultUVSTable    default_uvs_table;
    OtfCmapNonDefaultUVSTable non_default_uvs_table;
} OtfCmapVarSelector;

typedef struct OtfCmapSubTableFormat0 {
    Uint16 length;
    Uint16 language;
    Uint8  glyph_id_array[256];
} OtfCmapSubTableFormat0;

typedef struct OtfCmapSubTableFormat2 {
    Uint16            length;
    Uint16            language;
    Uint16            sub_header_keys[256];
    Uint16            num_sub_headers; /**< Computed value, not present in binary. */
    OtfCmapSubHeader* sub_headers;     /**< This is an arbitrary sized array acc to spec. */
    Uint16            num_glyph_ids;   /**< Computed value, not present in binary. */
    Uint16*           glyph_id_array;  /**< This is an arbitrary sized array acc to spec. */
} OtfCmapSubTableFormat2;

typedef struct OtfCmapSubTableFormat4 {
    Uint16  length;
    Uint16  language;
    Uint16  seg_count;
    Uint16  search_range;
    Uint16  entry_selector;
    Uint16  range_shift;
    Uint16* end_code;
    Uint16  reserved_pad;
    Uint16* start_code;
    Int16*  id_delta;
    Uint16* id_range_offsets;
    Uint16  num_glyph_ids;  /**< @b Computed value, not present in binary */
    Uint16* glyph_id_array; /**< @b This is an arbitrary sized array acc to spec. */
} OtfCmapSubTableFormat4;

typedef struct OtfCmapSubTableFormat6 {
    Uint16  length;
    Uint16  language;
    Uint16  first_code;
    Uint16  entry_count;
    Uint16* glyph_id_array;
} OtfCmapSubTableFormat6;

typedef struct OtfCmapSubTableFormat8 {
    Uint16           reserved;
    Uint32           length;
    Uint32           language;
    Uint8            is32[8192];
    Uint32           num_groups;
    OtfCmapMapGroup* groups;
} OtfCmapSubTableFormat8;

typedef struct OtfCmapSubTableFormat10 {
    Uint16  reserved;
    Uint32  length;
    Uint32  language;
    Uint32  start_char_code;
    Uint32  num_chars;
    Uint16* glyph_id_array;
} OtfCmapSubTableFormat10;

typedef struct OtfCmapSubTableFormat12 {
    Uint16           reserved;
    Uint32           length;
    Uint32           language;
    Uint32           num_groups;
    OtfCmapMapGroup* groups;
} OtfCmapSubTableFormat12, OtfCmapSubTableFormat13;

typedef struct OtfCmapSubTableFormat14 {
    Uint32              length;
    Uint32              num_var_selectors;
    OtfCmapVarSelector* var_selectors;
} OtfCmapSubTableFormat14;

/**
 * @b Definition of different sub tables in cmap table based on format.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#format-0-byte-encoding-table
 * */
typedef struct OtfCmapSubTable {
    Uint16 format;
    union {
        OtfCmapSubTableFormat0*  format0;
        OtfCmapSubTableFormat2*  format2;
        OtfCmapSubTableFormat4*  format4;
        OtfCmapSubTableFormat6*  format6;
        OtfCmapSubTableFormat8*  format8;
        OtfCmapSubTableFormat10* format10;
        OtfCmapSubTableFormat12* format12;
        OtfCmapSubTableFormat13* format13;
        OtfCmapSubTableFormat14* format14;
    };
} OtfCmapSubTable;

/**
 * The array of encoding records specifies particular encodings and the offset to the
 * subtable for each encoding.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#encoding-records-and-encodings
 * */
typedef struct OtfCmapEncodingRecord {
    OtfPlatformEncoding platform_encoding;
    Uint32              sub_table_offset;
    OtfCmapSubTable     sub_table;
} OtfCmapEncodingRecord;

/**
 * @b This table defines the mapping of character codes to the glyph index values used in the font.
 * It may contain more than one subtable, in order to support more than one character encoding scheme.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/cmap#cmap-header
 * */
typedef struct OtfCmap {
    Uint16                 version;
    Uint16                 num_tables;
    OtfCmapEncodingRecord* encoding_records;
} OtfCmap;

OtfCmap* otf_cmap_init (OtfCmap* cmap, Uint8* data, Size size);
OtfCmap* otf_cmap_deinit (OtfCmap* cmap);
OtfCmap* otf_cmap_pprint (OtfCmap* cmap, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_CMAP_H
