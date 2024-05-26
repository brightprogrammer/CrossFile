/**
 * @file Cmap.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Cmap.h>

/* libc */
#include <memory.h>

/* fwd declarations of private methods */
static inline XfOtfCmapEncodingRecord*
    encoding_record_init (XfOtfCmapEncodingRecord* enc, Uint8* data, Size size);
static inline XfOtfCmapEncodingRecord* encoding_record_deinit (XfOtfCmapEncodingRecord* enc);
static inline XfOtfCmapEncodingRecord*
    encoding_record_pprint (XfOtfCmapEncodingRecord* enc, Uint8 indent_level);

static inline XfOtfCmapSubHeader*
    sub_header_init (XfOtfCmapSubHeader* sub_head, Uint8* data, Size size);
static inline XfOtfCmapSubHeader*
    sub_header_pprint (XfOtfCmapSubHeader* sub_head, Uint8 indent_level);

static inline XfOtfCmapMapGroup* map_group_init (XfOtfCmapMapGroup* group, Uint8* data, Size size);
static inline XfOtfCmapMapGroup* map_group_pprint (XfOtfCmapMapGroup* group, Uint8 indent_level);

static inline XfOtfCmapVarSelector*
    var_selector_init (XfOtfCmapVarSelector* sel, Uint8* data, Size size);
static inline XfOtfCmapVarSelector*
    var_selector_pprint (XfOtfCmapVarSelector* sel, Uint8 indent_level);

static inline XfOtfCmapUnicodeRange*
    unicode_range_init (XfOtfCmapUnicodeRange* range, Uint8* data, Size size);
static inline XfOtfCmapUnicodeRange*
    unicode_range_pprint (XfOtfCmapUnicodeRange* range, Uint8 indent_level);

static inline XfOtfCmapDefaultUVSTable*
    default_uvs_table_init (XfOtfCmapDefaultUVSTable* default_uvs, Uint8* data, Size size);
static inline XfOtfCmapDefaultUVSTable*
    default_uvs_table_pprint (XfOtfCmapDefaultUVSTable* uvs_map, Uint8 indent_level);

static inline XfOtfCmapUVSMapping*
    uvs_mapping_init (XfOtfCmapUVSMapping* uvs_map, Uint8* data, Size size);
static inline XfOtfCmapUVSMapping*
    uvs_mapping_pprint (XfOtfCmapUVSMapping* uvs_map, Uint8 indent_level);

static inline XfOtfCmapNonDefaultUVSTable* non_default_uvs_table_init (
    XfOtfCmapNonDefaultUVSTable* non_default_uvs,
    Uint8*                       data,
    Size                         size
);
static inline XfOtfCmapNonDefaultUVSTable*
    non_default_uvs_table_pprint (XfOtfCmapNonDefaultUVSTable* non_default_uvs, Uint8 indent_level);

static inline XfOtfCmapSubTableFormat0*
    sub_table_format0_init (XfOtfCmapSubTableFormat0* f0, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat2*
    sub_table_format2_init (XfOtfCmapSubTableFormat2* f2, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat4*
    sub_table_format4_init (XfOtfCmapSubTableFormat4* f4, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat6*
    sub_table_format6_init (XfOtfCmapSubTableFormat6* f6, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat8*
    sub_table_format8_init (XfOtfCmapSubTableFormat8* f8, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat10*
    sub_table_format10_init (XfOtfCmapSubTableFormat10* f10, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat12*
    sub_table_format12_init (XfOtfCmapSubTableFormat12* f12, Uint8* data, Size size);
#define sub_table_format13_init sub_table_format12_init
static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_init (XfOtfCmapSubTableFormat14* f14, Uint8* data, Size size);

static inline XfOtfCmapSubTableFormat2*  sub_table_format2_deinit (XfOtfCmapSubTableFormat2* f2);
static inline XfOtfCmapSubTableFormat4*  sub_table_format4_deinit (XfOtfCmapSubTableFormat4* f4);
static inline XfOtfCmapSubTableFormat6*  sub_table_format6_deinit (XfOtfCmapSubTableFormat6* f6);
static inline XfOtfCmapSubTableFormat8*  sub_table_format8_deinit (XfOtfCmapSubTableFormat8* f8);
static inline XfOtfCmapSubTableFormat10* sub_table_format10_deinit (XfOtfCmapSubTableFormat10* f10);
static inline XfOtfCmapSubTableFormat12* sub_table_format12_deinit (XfOtfCmapSubTableFormat12* f12);
#define sub_table_format13_deinit sub_table_format12_deinit
static inline XfOtfCmapSubTableFormat14* sub_table_format14_deinit (XfOtfCmapSubTableFormat14* f14);

static inline XfOtfCmapSubTableFormat0*
    sub_table_format0_pprint (XfOtfCmapSubTableFormat0* f0, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat2*
    sub_table_format2_pprint (XfOtfCmapSubTableFormat2* f2, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat4*
    sub_table_format4_pprint (XfOtfCmapSubTableFormat4* f4, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat6*
    sub_table_format6_pprint (XfOtfCmapSubTableFormat6* f6, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat8*
    sub_table_format8_pprint (XfOtfCmapSubTableFormat8* f8, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat10*
    sub_table_format10_pprint (XfOtfCmapSubTableFormat10* f10, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat12*
    sub_table_format12_pprint (XfOtfCmapSubTableFormat12* f12, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat13*
    sub_table_format13_pprint (XfOtfCmapSubTableFormat13* f13, Uint8 indent_level);
static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_pprint (XfOtfCmapSubTableFormat14* f14, Uint8 indent_level);

static inline XfOtfCmapSubTable*
    sub_table_init (XfOtfCmapSubTable* sub_table, Uint8* data, Size size);
static inline XfOtfCmapSubTable* sub_table_deinit (XfOtfCmapSubTable* sub_table);
static inline XfOtfCmapSubTable*
    sub_table_pprint (XfOtfCmapSubTable* sub_table, Uint8 indent_level);

/* size limit definitions for error checking */
#define ENCODING_RECORD_DATA_SIZE       (sizeof (XfOtfCmapPlatformId) + sizeof (Uint16) + sizeof (Uint32))
#define CMAP_DATA_SIZE                  (sizeof (Uint16) * 2)
#define SUB_HEADER_DATA_SIZE            (sizeof (Uint16) * 3 + sizeof (Int16))
#define MAP_GROUP_DATA_SIZE             (sizeof (Uint32) * 3)
#define VAR_SELECTOR_DATA_SIZE          (sizeof (Uint8) * 3 + sizeof (Uint32) * 2)
#define UNICODE_RANGE_DATA_SIZE         (sizeof (Uint8) * 3 + sizeof (Uint8))
#define DEFAULT_UVS_TABLE_DATA_SIZE     (sizeof (Uint32))
#define UVS_MAPPING_DATA_SIZE           (sizeof (Uint8) * 3 + sizeof (Uint16))
#define NON_DEFAULT_UVS_TABLE_DATA_SIZE (sizeof (Uint32))
#define SUB_TABLE_FORMAT0_DATA_SIZE     (sizeof (Uint16) * 2 + sizeof (Uint8) * 256)
#define SUB_TABLE_FORMAT2_DATA_SIZE     (sizeof (Uint16) * 2 + sizeof (Uint16) * 256)
#define SUB_TABLE_FORMAT4_DATA_SIZE     (sizeof (Uint16) * 6)
#define SUB_TABLE_FORMAT6_DATA_SIZE     (sizeof (Uint16) * 4)
#define SUB_TABLE_FORMAT8_DATA_SIZE     (sizeof (Uint16) + sizeof (Uint32) * 3 + sizeof (Uint8) * 8192)
#define SUB_TABLE_FORMAT10_DATA_SIZE    (sizeof (Uint16) + sizeof (Uint32) * 4)
#define SUB_TABLE_FORMAT12_DATA_SIZE    (sizeof (Uint16) + sizeof (Uint32) * 3)
#define SUB_TABLE_FORMAT13_DATA_SIZE    SUB_TABLE_FORMAT12_DATA_SIZE
#define SUB_TABLE_FORMAT14_DATA_SIZE    (sizeof (Uint32) * 2)

#define PPRINT_ARR_U1(arr, last_idx)                                                               \
    putchar ('[');                                                                                 \
    for (Int64 s = 0; s < MIN (6, last_idx); s++) {                                                \
        printf ("%u, ", arr[s]);                                                                   \
    }                                                                                              \
    printf ("...]\n");

#define PPRINT_ARR_U2(arr, last_idx)                                                               \
    putchar ('[');                                                                                 \
    for (Int64 s = 0; s < MIN (6, last_idx); s++) {                                                \
        printf ("%u, ", arr[s]);                                                                   \
    }                                                                                              \
    printf ("...]\n");

#define PPRINT_ARR_I2(arr, last_idx)                                                               \
    putchar ('[');                                                                                 \
    for (Int64 s = 0; s < MIN (6, last_idx); s++) {                                                \
        printf ("%d, ", arr[s]);                                                                   \
    }                                                                                              \
    printf ("...]\n");

/**************************************************************************************************/
/*********************************** PUBLIC METHOD DEFINITIONS ************************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfOtfCmap by reading and adjusting
 *    endinanness of data from given @c data buffer.
 *
 * @param cmap To be initialized.
 * @param data Data array containing data in Big Endian format to be loaded.
 * @param size Size of @c data.
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCmap* xf_otf_cmap_init (XfOtfCmap* cmap, Uint8* data, Size size) {
    RETURN_VALUE_IF (!cmap || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < CMAP_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize character to glyph index map \"cmap\".\n"
    );

    /* required for reading sub tables */
    Uint8* data_start    = data;
    Size   original_size = size;

    cmap->version    = GET_AND_ADV_U2 (data);
    cmap->num_tables = GET_AND_ADV_U2 (data);

    if (!cmap->num_tables) {
        return cmap;
    }

    cmap->encoding_records = ALLOCATE (XfOtfCmapEncodingRecord, cmap->num_tables);
    RETURN_VALUE_IF (!cmap->encoding_records, Null, ERR_OUT_OF_MEMORY);

    size -= CMAP_DATA_SIZE;

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        XfOtfCmapEncodingRecord* enc = cmap->encoding_records + table_idx;

        if (!encoding_record_init (enc, data, size)) {
            PRINT_ERR (
                "Failed to read all encoding records in character to glyph index map "
                "\"cmap\".\n"
            );
            xf_otf_cmap_deinit (cmap);
            return Null;
        }

        RETURN_VALUE_IF (
            !sub_table_init (
                &enc->sub_table,
                data_start + enc->sub_table_offset,
                original_size - enc->sub_table_offset
            ),
            Null,
            "Failed to read subtable inside cmap encoding record\n"
        );


        size -= ENCODING_RECORD_DATA_SIZE;
        data += ENCODING_RECORD_DATA_SIZE;
    }

    return cmap;
}

/**
 * @b De-initialize contents of given @c XfOtfCmap struct.
 *
 * @param cmap To be de-initialized
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCmap* xf_otf_cmap_deinit (XfOtfCmap* cmap) {
    RETURN_VALUE_IF (!cmap, Null, ERR_INVALID_ARGUMENTS);

    if (cmap->encoding_records) {
        for (Size idx = 0; idx < cmap->num_tables; idx++) {
            encoding_record_deinit (cmap->encoding_records + idx);
        }

        FREE (cmap->encoding_records);
    }

    memset (cmap, 0, sizeof (XfOtfCmap));

    return cmap;
}

/**
 * @b Pretty print contents of given @c XfOtfCmap struct.
 *
 * @param cmap To be pretty-printed.
 * @param indent_level
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCmap* xf_otf_cmap_pprint (XfOtfCmap* cmap, Uint8 indent_level) {
    RETURN_VALUE_IF (!cmap, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Character To Glyph Index Map Table :\n"
        "|%s|version = %u\n"
        "|%s|num_tables = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        cmap->version,
        indent,
        cmap->num_tables
    );

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        encoding_record_pprint (cmap->encoding_records + table_idx, indent_level + 1);
    }

    return cmap;
}

/**************************************************************************************************/
/*********************************** PRIVATE METHOD DEFINITIONS ***********************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfOtfCmapEncodingRecord by reading and adjusting
*    endinanness of data from given @c data buffer.
 *
 * @param enc To be initialized.
 * @param data Data array containing data in Big Endian format to be loaded.
 * @param size Size of @c data.
 *
 * @return @c enc on success.
 * @return @c Null otherwise.
 * */
static inline XfOtfCmapEncodingRecord*
    encoding_record_init (XfOtfCmapEncodingRecord* enc, Uint8* data, Size size) {
    RETURN_VALUE_IF (!enc || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < ENCODING_RECORD_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize encoding record.\n"
    );

    enc->platform_id        = GET_AND_ADV_U2 (data);
    enc->encoding_id.custom = GET_AND_ADV_U2 (data);
    enc->sub_table_offset   = GET_AND_ADV_U4 (data);

    return enc;
}

/**
 * @b Deinit given encoding record structure.
 *
 * @param enc
 *
 * @return @c enc on success.
 * @return @c Null otherwise.
 * */
static inline XfOtfCmapEncodingRecord* encoding_record_deinit (XfOtfCmapEncodingRecord* enc) {
    RETURN_VALUE_IF (!enc, Null, ERR_INVALID_ARGUMENTS);

    sub_table_deinit (&enc->sub_table);

    return enc;
}

/**
 * @b Pretty print contents of given @c XfOtfCmapEncodingRecord struct.
 *
 * @param enc To be pretty-printed.
 *
 * @return @c enc on success.
 * @return @c Null otherwise.
 * */
static inline XfOtfCmapEncodingRecord*
    encoding_record_pprint (XfOtfCmapEncodingRecord* enc, Uint8 indent_level) {
    RETURN_VALUE_IF (!enc, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    static const CString platform_id_to_str_map[] = {
        "VARIOUS",
        "MAC",
        "ISO",
        "WIN",
        "CUSTOM",
    };

    static const CString enc_various_to_str_map[] = {
        "Unicode 1.0",
        "Unicode 1.1",
        "ISO/IEC 10646",
        "Unicode 2.0+ (BMP only)",
        "Unicode 2.0+ (Full repertoire)",
        "Unicode Variation Sequences",
        "Unicode full repertoire"
    };
    static const CString enc_iso_to_str_map[] = {"7-bit ASCII", "ISO 10646\0", "ISO 8859-1\0"};
    static const CString enc_win_to_str_map[] = {
        "Symbol",
        "Unicode BMP",
        "ShiftJIS",
        "PRC",
        "Big5",
        "Wansung",
        "Johab",
        "Reserved",
        "Reserved",
        "Reserved",
        "Unicode full repertoire"
    };
    static CString enc_mac_to_str_map[] = {
        "Roman",
        "Japanese",
        "Chinese (Traditional)",
        "Korean",
        "Arabic",
        "Hebrew",
        "Greek",
        "Russian",
        "RSymbol",
        "Devanagari",
        "Gurmukhi",
        "Gujarati",
        "Odia",
        "Bangla",
        "Tamil",
        "Telugu",
        "Kannada",
        "Malayalam",
        "Sinhalese",
        "Burmese",
        "Khmer",
        "Thai",
        "Laotian",
        "Georgian",
        "Armenian",
        "Chinese (Simplified)",
        "Tibetan",
        "Mongolian",
        "Geez",
        "Slavic",
        "Vietnamese",
        "Sindhi",
        "Uninterpreted"
    };

    if (enc->platform_id <= XF_OTF_CMAP_PLATFORM_ID_MAX) {
        CString encoding_format_str = Null;
        switch (enc->platform_id) {
            case XF_OTF_CMAP_PLATFORM_ID_VARIOUS : {
                if (enc->encoding_id.various <= XF_OTF_CMAP_VARIOUS_ENCODING_ID_MAX) {
                    encoding_format_str = enc_various_to_str_map[enc->encoding_id.various];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_CMAP_PLATFORM_ID_MAC : {
                if (enc->encoding_id.mac <= XF_OTF_CMAP_MAC_ENCODING_ID_MAX) {
                    encoding_format_str = enc_mac_to_str_map[enc->encoding_id.mac];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_CMAP_PLATFORM_ID_ISO : {
                if (enc->encoding_id.iso <= XF_OTF_CMAP_ISO_ENCODING_ID_MAX) {
                    encoding_format_str = enc_iso_to_str_map[enc->encoding_id.iso];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_CMAP_PLATFORM_ID_WIN : {
                if (enc->encoding_id.win <= XF_OTF_CMAP_WIN_ENCODING_ID_MAX) {
                    encoding_format_str = enc_win_to_str_map[enc->encoding_id.win];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_CMAP_PLATFORM_ID_CUSTOM :
                encoding_format_str = "Custom";
                break;
        }

        printf (
            "|%.*s|OTF Char To Glyph Map Encoding Format :\n"
            "|%s|platform_id = %x (%s)\n"
            "|%s|encoding_id = %x (%s)\n"
            "|%s|sub_table_offset = %08x\n",
            indent_level - 1 ? indent_level - 1 : 1,
            indent,
            indent,
            enc->platform_id,
            platform_id_to_str_map[enc->platform_id],
            indent,
            enc->encoding_id.custom,
            encoding_format_str,
            indent,
            enc->sub_table_offset
        );
    } else {
        printf (
            "|%.*s|OTF Char To Glyph Map Encoding Format :\n"
            "|%s|platform_id = %x (Unknown)\n"
            "|%s|encoding_id = %x (Unknown)\n"
            "|%s|sub_table_offset = %08x\n",
            indent_level - 1 ? indent_level - 1 : 1,
            indent,
            indent,
            enc->platform_id,
            indent,
            enc->encoding_id.custom,
            indent,
            enc->sub_table_offset
        );
    }

    sub_table_pprint (&enc->sub_table, indent_level + 1);

    return enc;
}

static inline XfOtfCmapSubHeader*
    sub_header_init (XfOtfCmapSubHeader* sub_head, Uint8* data, Size size) {
    RETURN_VALUE_IF (!sub_head || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_HEADER_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub header\n"
    );

    sub_head->first_code      = GET_AND_ADV_U2 (data);
    sub_head->entry_count     = GET_AND_ADV_U2 (data);
    sub_head->id_delta        = GET_AND_ADV_I2 (data);
    sub_head->id_range_offset = GET_AND_ADV_U2 (data);

    return sub_head;
}

static inline XfOtfCmapSubHeader*
    sub_header_pprint (XfOtfCmapSubHeader* sub_head, Uint8 indent_level) {
    RETURN_VALUE_IF (!sub_head, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Header :\n"
        "|%s|first_code = %u\n"
        "|%s|entry_count = %u\n"
        "|%s|id_delta = %d\n"
        "|%s|id_range_offset = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        sub_head->first_code,
        indent,
        sub_head->entry_count,
        indent,
        sub_head->id_delta,
        indent,
        sub_head->id_range_offset
    );

    return sub_head;
}

static inline XfOtfCmapMapGroup* map_group_init (XfOtfCmapMapGroup* group, Uint8* data, Size size) {
    RETURN_VALUE_IF (!group || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < MAP_GROUP_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sequential/constant map group\n"
    );

    group->start_char_code = GET_AND_ADV_U4 (data);
    group->end_char_code   = GET_AND_ADV_U4 (data);
    group->glyph_id        = GET_AND_ADV_U4 (data);

    return group;
}

static inline XfOtfCmapMapGroup* map_group_pprint (XfOtfCmapMapGroup* group, Uint8 indent_level) {
    RETURN_VALUE_IF (!group, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap (Sequential/Constant) Map Group :\n"
        "|%s|start_char_code = %u\n"
        "|%s|end_char_code = %u\n"
        "|%s|glyph_id = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        group->start_char_code,
        indent,
        group->end_char_code,
        indent,
        group->glyph_id
    );

    return group;
}

static inline XfOtfCmapVarSelector*
    var_selector_init (XfOtfCmapVarSelector* sel, Uint8* data, Size size) {
    RETURN_VALUE_IF (!sel || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < VAR_SELECTOR_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap variation selector\n"
    );

    sel->var_selector = 0;
    sel->uint24[2]    = GET_AND_ADV_U1 (data);
    sel->uint24[1]    = GET_AND_ADV_U1 (data);
    sel->uint24[0]    = GET_AND_ADV_U1 (data);

    sel->default_uvs_offset     = GET_AND_ADV_U4 (data);
    sel->non_default_uvs_offset = GET_AND_ADV_U4 (data);

    return sel;
}

static inline XfOtfCmapVarSelector*
    var_selector_pprint (XfOtfCmapVarSelector* sel, Uint8 indent_level) {
    RETURN_VALUE_IF (!sel, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Variation Selector :\n"
        "|%s|var_selector = %u\n"
        "|%s|default_uvs_offset = %u\n"
        "|%s|non_default_uvs_offset = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        sel->var_selector,
        indent,
        sel->default_uvs_offset,
        indent,
        sel->non_default_uvs_offset
    );

    if (sel->default_uvs_offset) {
        default_uvs_table_pprint (&sel->default_uvs_table, indent_level + 1);
    }

    if (sel->non_default_uvs_offset) {
        non_default_uvs_table_pprint (&sel->non_default_uvs_table, indent_level + 1);
    }

    return sel;
}

static inline XfOtfCmapUnicodeRange*
    unicode_range_init (XfOtfCmapUnicodeRange* range, Uint8* data, Size size) {
    RETURN_VALUE_IF (!range || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < UNICODE_RANGE_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap unicode range\n"
    );

    range->start_unicode_value = 0;
    range->uint24[2]           = GET_AND_ADV_U1 (data);
    range->uint24[1]           = GET_AND_ADV_U1 (data);
    range->uint24[0]           = GET_AND_ADV_U1 (data);

    range->additional_count = GET_AND_ADV_U1 (data);

    return range;
}

static inline XfOtfCmapUnicodeRange*
    unicode_range_pprint (XfOtfCmapUnicodeRange* range, Uint8 indent_level) {
    RETURN_VALUE_IF (!range, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Unicode Range :\n"
        "|%s|start_unicode_value = %u\n"
        "|%s|additional_count = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        range->start_unicode_value,
        indent,
        range->additional_count
    );

    return range;
}

static inline XfOtfCmapDefaultUVSTable*
    default_uvs_table_init (XfOtfCmapDefaultUVSTable* default_uvs, Uint8* data, Size size) {
    RETURN_VALUE_IF (!default_uvs || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < UNICODE_RANGE_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap default uvs table\n"
    );

    default_uvs->num_unicode_value_ranges  = GET_AND_ADV_U4 (data);
    size                                  -= sizeof (Uint32);

    RETURN_VALUE_IF (
        size < UNICODE_RANGE_DATA_SIZE * default_uvs->num_unicode_value_ranges,
        Null,
        "Data buffer size not sufficient for initialization of cmap default uvs table\n"
    );

    default_uvs->ranges = ALLOCATE (XfOtfCmapUnicodeRange, default_uvs->num_unicode_value_ranges);
    RETURN_VALUE_IF (!default_uvs->ranges, Null, ERR_OUT_OF_MEMORY);

    for (Size range_idx = 0; range_idx < default_uvs->num_unicode_value_ranges; range_idx++) {
        if (!unicode_range_init (default_uvs->ranges + range_idx, data, size)) {
            PRINT_ERR ("Failed to read unicode range inside cmap table\n");
            FREE (default_uvs->ranges);
            return Null;
        }

        data += UNICODE_RANGE_DATA_SIZE;
        size -= UNICODE_RANGE_DATA_SIZE;
    }

    return default_uvs;
}

static inline XfOtfCmapDefaultUVSTable*
    default_uvs_table_pprint (XfOtfCmapDefaultUVSTable* default_uvs, Uint8 indent_level) {
    RETURN_VALUE_IF (!default_uvs, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Default UVS Table :\n"
        "|%s|num_unicode_value_ranges = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        default_uvs->num_unicode_value_ranges
    );

    for (Size s = 0; s < default_uvs->num_unicode_value_ranges; s++) {
        unicode_range_pprint (default_uvs->ranges + s, indent_level + 1);
    }

    return default_uvs;
}

static inline XfOtfCmapUVSMapping*
    uvs_mapping_init (XfOtfCmapUVSMapping* uvs_map, Uint8* data, Size size) {
    RETURN_VALUE_IF (!uvs_map || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < UNICODE_RANGE_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap uvs mapping\n"
    );

    uvs_map->unicode_value = 0;
    uvs_map->uint24[2]     = GET_AND_ADV_U1 (data);
    uvs_map->uint24[1]     = GET_AND_ADV_U1 (data);
    uvs_map->uint24[0]     = GET_AND_ADV_U1 (data);

    uvs_map->glyph_id = GET_AND_ADV_U2 (data);

    return uvs_map;
}

static inline XfOtfCmapUVSMapping*
    uvs_mapping_pprint (XfOtfCmapUVSMapping* uvs_map, Uint8 indent_level) {
    RETURN_VALUE_IF (!uvs_map, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap UVS Mapping :\n"
        "|%s|unicode_alue = %u\n"
        "|%s|glyph_id = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        uvs_map->unicode_value,
        indent,
        uvs_map->glyph_id
    );

    return uvs_map;
}

static inline XfOtfCmapNonDefaultUVSTable* non_default_uvs_table_init (
    XfOtfCmapNonDefaultUVSTable* non_default_uvs,
    Uint8*                       data,
    Size                         size
) {
    RETURN_VALUE_IF (!non_default_uvs || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < UNICODE_RANGE_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap non default uvs table\n"
    );

    non_default_uvs->num_uvs_mappings  = GET_AND_ADV_U4 (data);
    size                              -= sizeof (Uint32);

    RETURN_VALUE_IF (
        size < UVS_MAPPING_DATA_SIZE * non_default_uvs->num_uvs_mappings,
        Null,
        "Data buffer size not sufficient for initialization of cmap non default uvs table\n"
    );

    non_default_uvs->uvs_mappings =
        ALLOCATE (XfOtfCmapUVSMapping, non_default_uvs->num_uvs_mappings);
    RETURN_VALUE_IF (!non_default_uvs->uvs_mappings, Null, ERR_OUT_OF_MEMORY);

    for (Size range_idx = 0; range_idx < non_default_uvs->num_uvs_mappings; range_idx++) {
        if (!uvs_mapping_init (non_default_uvs->uvs_mappings + range_idx, data, size)) {
            PRINT_ERR ("Failed to read unicode range inside cmap table\n");
            FREE (non_default_uvs->uvs_mappings);
            return Null;
        }

        data += UNICODE_RANGE_DATA_SIZE;
        size -= UNICODE_RANGE_DATA_SIZE;
    }

    return non_default_uvs;
}

static inline XfOtfCmapNonDefaultUVSTable* non_default_uvs_table_pprint (
    XfOtfCmapNonDefaultUVSTable* non_default_uvs,
    Uint8                        indent_level
) {
    RETURN_VALUE_IF (!non_default_uvs, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Non Default UVS Table :\n"
        "|%s|num_uvs_mappings = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        non_default_uvs->num_uvs_mappings
    );

    for (Size s = 0; s < non_default_uvs->num_uvs_mappings; s++) {
        uvs_mapping_pprint (non_default_uvs->uvs_mappings + s, indent_level + 1);
    }

    return non_default_uvs;
}

static inline XfOtfCmapSubTableFormat0*
    sub_table_format0_init (XfOtfCmapSubTableFormat0* f0, Uint8* data, Size size) {
    RETURN_VALUE_IF (!f0 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT0_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 0\n"
    );

    f0->length   = GET_AND_ADV_U2 (data);
    f0->language = GET_AND_ADV_U2 (data);
    memcpy (f0->glyph_id_array, data, sizeof (Uint8) * 256);

    return f0;
}

static inline XfOtfCmapSubTableFormat0*
    sub_table_format0_pprint (XfOtfCmapSubTableFormat0* f0, Uint8 indent_level) {
    RETURN_VALUE_IF (!f0, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 0 :\n"
        "|%s|format = 0 (Byte encoding table)\n"
        "|%s|length = %u\n"
        "|%s|languge = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f0->length,
        indent,
        f0->language
    );

    printf ("|%s|gylph_id_array = ", indent);
    PPRINT_ARR_U1 (f0->glyph_id_array, 255);

    return f0;
}

static inline XfOtfCmapSubTableFormat2*
    sub_table_format2_init (XfOtfCmapSubTableFormat2* f2, Uint8* data, Size size) {
    RETURN_VALUE_IF (!f2 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT2_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 2\n"
    );

    f2->length   = GET_AND_ADV_U2 (data);
    f2->language = GET_AND_ADV_U2 (data);

    GET_ARR_AND_ADV_U2 (f2->sub_header_keys, 0, 256);

    /* read sub headers */
    {
        /* total number of sub headers is same as max value in sub header keys */
        Uint16 max = 0;
        for (Size s = 0; s < 256; s++) {
            max = max < f2->sub_header_keys[s] ? f2->sub_header_keys[s] : max;
        }

        size -= SUB_TABLE_FORMAT2_DATA_SIZE;

        /* read sub header table */
        f2->num_sub_headers = max;
        if (f2->num_sub_headers) {
            GOTO_HANDLER_IF (
                size < SUB_HEADER_DATA_SIZE * f2->num_sub_headers,
                INIT_FAILED,
                "Data buffer size not sufficient for initialization of cmap sub_table format 2\n"
            );

            GOTO_HANDLER_IF (
                !(f2->sub_headers = ALLOCATE (XfOtfCmapSubHeader, f2->num_sub_headers)),
                INIT_FAILED,
                ERR_OUT_OF_MEMORY
            );

            for (Size s = 0; s < f2->num_sub_headers; s++) {
                GOTO_HANDLER_IF (
                    !sub_header_init (f2->sub_headers + s, data, size),
                    INIT_FAILED,
                    "Failed to read sub header record inside sub table format2\n"
                );

                data += SUB_HEADER_DATA_SIZE;
                size -= SUB_HEADER_DATA_SIZE;
            }

        } else {
            f2->sub_headers = Null;
        }
    }

    /* read glyph id array */
    {
        Size remaining_size =
            f2->length - SUB_TABLE_FORMAT2_DATA_SIZE - SUB_HEADER_DATA_SIZE * f2->num_sub_headers;

        if (remaining_size && remaining_size % 2 == 0) {
            f2->num_glyph_ids = remaining_size / sizeof (Uint16);

            GOTO_HANDLER_IF (
                size < remaining_size,
                INIT_FAILED,
                "Data buffer size not sufficient for initialization of cmap sub_table format 2\n"
            );

            GOTO_HANDLER_IF (
                !(f2->glyph_id_array = ALLOCATE (Uint16, f2->num_glyph_ids)),
                INIT_FAILED,
                ERR_OUT_OF_MEMORY
            );

            GET_ARR_AND_ADV_U2 (f2->glyph_id_array, 0, f2->num_glyph_ids);
        } else {
            f2->num_glyph_ids  = 0;
            f2->glyph_id_array = Null;
        }
    }

    return f2;

INIT_FAILED:
    sub_table_format2_deinit (f2);
    return Null;
}

static inline XfOtfCmapSubTableFormat2* sub_table_format2_deinit (XfOtfCmapSubTableFormat2* f2) {
    RETURN_VALUE_IF (!f2, Null, ERR_INVALID_ARGUMENTS);

    if (f2->glyph_id_array) {
        FREE (f2->glyph_id_array);
    }

    if (f2->sub_headers) {
        FREE (f2->sub_headers);
    }

    memset (f2, 0, sizeof (XfOtfCmapSubTableFormat2));
    return f2;
}

static inline XfOtfCmapSubTableFormat2*
    sub_table_format2_pprint (XfOtfCmapSubTableFormat2* f2, Uint8 indent_level) {
    RETURN_VALUE_IF (!f2, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 2 :\n"
        "|%s|format = 2 (High-byte mapping through table)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f2->length,
        indent,
        f2->language
    );

    printf ("|%s|sub_header_keys = ", indent);
    PPRINT_ARR_U1 (f2->sub_header_keys, 255);

    printf ("|%s|num_sub_headers = %u\n", indent, f2->num_sub_headers);
    for (Size s = 0; s < f2->num_sub_headers; s++) {
        sub_header_pprint (f2->sub_headers + s, indent_level + 1);
    }

    printf (
        "|%s|num_glyph_ids = %u\n"
        "|%s|glyph_id_array = ",
        indent,
        f2->num_glyph_ids,
        indent
    );
    PPRINT_ARR_U1 (f2->glyph_id_array, f2->num_glyph_ids);

    return f2;
}

static inline XfOtfCmapSubTableFormat4*
    sub_table_format4_init (XfOtfCmapSubTableFormat4* f4, Uint8* data, Size size) {
    RETURN_VALUE_IF (!f4 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT4_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 4\n"
    );

    f4->length   = GET_AND_ADV_U2 (data);
    f4->language = GET_AND_ADV_U2 (data);

    /* this data is seg count times two, so we adjust this so we don't have to
     * compute sub_count/2 everytime we need to use it. */
    f4->seg_count  = GET_AND_ADV_U2 (data);
    f4->seg_count /= 2;

    f4->search_range   = GET_AND_ADV_U2 (data);
    f4->entry_selector = GET_AND_ADV_U2 (data);
    f4->range_shift    = GET_AND_ADV_U2 (data);

    if (f4->seg_count) {
        size -= SUB_TABLE_FORMAT4_DATA_SIZE;

        Size required_size = (sizeof (Uint16) * f4->seg_count * 4 + sizeof (Uint16));

        RETURN_VALUE_IF (
            size < required_size,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 4\n"
        );

        /* instead of allocating 5 small chunks of memory, allocate single big one,
         * and manually allocate parts of it to different arrays. */
        Uint16* mem = ALLOCATE (Uint16, f4->seg_count * 4);
        RETURN_VALUE_IF (!mem, Null, ERR_OUT_OF_MEMORY);

        f4->end_code         = mem;
        f4->start_code       = mem + f4->seg_count;
        f4->id_delta         = (Int16*)(mem + 2 * f4->seg_count);
        f4->id_range_offsets = mem + 3 * f4->seg_count;

        GET_ARR_AND_ADV_U2 (f4->end_code, 0, f4->seg_count);
        f4->reserved_pad = GET_AND_ADV_U2 (data);
        GET_ARR_AND_ADV_U2 (f4->end_code, 0, f4->seg_count);
        GET_ARR_AND_ADV_I2 (f4->id_delta, 0, f4->seg_count);
        GET_ARR_AND_ADV_U2 (f4->id_range_offsets, 0, f4->seg_count);
        size -= required_size;

        Int64 remaining_size =
            (Int64)f4->length -
            (Int64)(SUB_TABLE_FORMAT4_DATA_SIZE /* everything before the appearance of first array */
                    + sizeof (Uint16) /* for format */ +
                    required_size /* all the arrays + reserved0 */);

        GOTO_HANDLER_IF (
            remaining_size < 0,
            INIT_FAILED,
            "Buffer overflow detected while reading cmap sub table format 4\n"
        );

        RETURN_VALUE_IF (
            (Int64)size < remaining_size,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 4\n"
        );

        GOTO_HANDLER_IF (
            (remaining_size & 1) != 0,
            INIT_FAILED,
            "Unaligned memory found while reading cmap sub table format 4\n"
        );

        if (remaining_size) {
            f4->num_glyph_ids  = remaining_size / sizeof (Uint16);
            f4->glyph_id_array = ALLOCATE (Uint16, f4->num_glyph_ids);
            GOTO_HANDLER_IF (!f4->glyph_id_array, INIT_FAILED, ERR_OUT_OF_MEMORY);
            GET_ARR_AND_ADV_U2 (f4->glyph_id_array, 0, f4->num_glyph_ids);
        } else {
            f4->num_glyph_ids  = 0;
            f4->glyph_id_array = Null;
        }
    }

    return f4;

INIT_FAILED:
    sub_table_format4_deinit (f4);
    return Null;
}

static inline XfOtfCmapSubTableFormat4* sub_table_format4_deinit (XfOtfCmapSubTableFormat4* f4) {
    RETURN_VALUE_IF (!f4, Null, ERR_INVALID_ARGUMENTS);

    if (f4->glyph_id_array) {
        FREE (f4->glyph_id_array);
    }

    /* only one array is allocated and others are given offsets into
     * the large memory chunk. */
    if (f4->end_code) {
        FREE (f4->end_code);
    }

    memset (f4, 0, sizeof (XfOtfCmapSubTableFormat4));

    return f4;
}

static inline XfOtfCmapSubTableFormat4*
    sub_table_format4_pprint (XfOtfCmapSubTableFormat4* f4, Uint8 indent_level) {
    RETURN_VALUE_IF (!f4, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 4\n"
        "|%s|format = 4 (Segment mapping to delta values)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n"
        "|%s|seg_count = %u\n"
        "|%s|search_range = %u\n"
        "|%s|entry_selector = %u\n"
        "|%s|range_shift = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f4->length,
        indent,
        f4->language,
        indent,
        f4->seg_count,
        indent,
        f4->search_range,
        indent,
        f4->entry_selector,
        indent,
        f4->range_shift
    );

    printf ("|%s|end_code = ", indent);
    PPRINT_ARR_U2 (f4->end_code, f4->seg_count - 1);
    printf ("|%s|start_code = ", indent);
    PPRINT_ARR_U2 (f4->start_code, f4->seg_count - 1);
    printf ("|%s|id_delta = ", indent);
    PPRINT_ARR_I2 (f4->id_delta, f4->seg_count - 1);
    printf ("|%s|id_range_offsets = ", indent);
    PPRINT_ARR_U2 (f4->id_range_offsets, f4->seg_count - 1);

    if (f4->num_glyph_ids) {
        printf (
            "|%s|num_glyph_ids = %u\n"
            "|%s|glyph_id_array = ",
            indent,
            f4->num_glyph_ids,
            indent
        );
        PPRINT_ARR_U2 (f4->glyph_id_array, f4->num_glyph_ids - 1);
    } else {
        printf (
            "|%s|num_glyph_ids = 0\n"
            "|%s|glyph_id_array = []\n",
            indent,
            indent
        );
    }

    return f4;
}

static inline XfOtfCmapSubTableFormat6*
    sub_table_format6_init (XfOtfCmapSubTableFormat6* f6, Uint8* data, Size size) {
    RETURN_VALUE_IF (!f6 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT6_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 6\n"
    );

    f6->length      = GET_AND_ADV_U2 (data);
    f6->language    = GET_AND_ADV_U2 (data);
    f6->first_code  = GET_AND_ADV_U2 (data);
    f6->entry_count = GET_AND_ADV_U2 (data);

    if (f6->entry_count) {
        size -= SUB_TABLE_FORMAT6_DATA_SIZE;

        RETURN_VALUE_IF (
            size < sizeof (Uint16) * f6->entry_count,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 6\n"
        );

        f6->glyph_id_array = ALLOCATE (Uint16, f6->entry_count);
        RETURN_VALUE_IF (!f6->glyph_id_array, Null, ERR_OUT_OF_MEMORY);

        GET_ARR_AND_ADV_U2 (f6->glyph_id_array, 0, f6->entry_count);
    }

    return f6;
}

static inline XfOtfCmapSubTableFormat6* sub_table_format6_deinit (XfOtfCmapSubTableFormat6* f6) {
    RETURN_VALUE_IF (!f6, Null, ERR_INVALID_ARGUMENTS);

    if (f6->glyph_id_array) {
        FREE (f6->glyph_id_array);
        f6->glyph_id_array = Null;
    }

    return f6;
}

static inline XfOtfCmapSubTableFormat6*
    sub_table_format6_pprint (XfOtfCmapSubTableFormat6* f6, Uint8 indent_level) {
    RETURN_VALUE_IF (!f6, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 6 :\n"
        "|%s|format = 6 (Trimmed table mapping)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n"
        "|%s|first_code = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f6->length,
        indent,
        f6->language,
        indent,
        f6->first_code
    );

    printf ("|%s|glyph_id_array = ", indent);
    PPRINT_ARR_U2 (f6->glyph_id_array, f6->entry_count - 1);

    return f6;
}

static inline XfOtfCmapSubTableFormat8*
    sub_table_format8_init (XfOtfCmapSubTableFormat8* f8, Uint8* data, Size size) {
    RETURN_VALUE_IF (!f8 || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT8_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 8\n"
    );

    f8->reserved = GET_AND_ADV_U2 (data);
    f8->length   = GET_AND_ADV_U4 (data);
    f8->language = GET_AND_ADV_U4 (data);

    memcpy (f8->is32, data, sizeof (Uint8) * 8192);

    f8->num_groups = GET_AND_ADV_U4 (data);

    if (f8->num_groups) {
        size -= SUB_TABLE_FORMAT8_DATA_SIZE;

        RETURN_VALUE_IF (
            size < MAP_GROUP_DATA_SIZE * f8->num_groups,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 8\n"
        );

        RETURN_VALUE_IF (
            !(f8->groups = ALLOCATE (XfOtfCmapMapGroup, f8->num_groups)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        for (Size gidx = 0; gidx < f8->num_groups; gidx++) {
            if (!map_group_init (f8->groups + gidx, data, size)) {
                PRINT_ERR ("Failed to read sequential map group\n");
                FREE (f8->groups);
                return Null;
            }
        }
    }

    return f8;
}

static inline XfOtfCmapSubTableFormat8* sub_table_format8_deinit (XfOtfCmapSubTableFormat8* f8) {
    RETURN_VALUE_IF (!f8, Null, ERR_INVALID_ARGUMENTS);

    if (f8->groups) {
        FREE (f8->groups);
        f8->groups = Null;
    }

    return f8;
}

static inline XfOtfCmapSubTableFormat8*
    sub_table_format8_pprint (XfOtfCmapSubTableFormat8* f8, Uint8 indent_level) {
    RETURN_VALUE_IF (!f8, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 8 :\n"
        "|%s|format = 8 (mixed 16-bit and 32-bit coverage)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f8->length,
        indent,
        f8->language
    );

    printf ("|%s|is32 = ", indent);
    PPRINT_ARR_U1 (f8->is32, 8192 - 1);

    printf ("|%s|num_groups = %u\n", indent, f8->num_groups);

    for (Size s = 0; s < f8->num_groups; s++) {
        map_group_pprint (f8->groups + s, indent_level + 1);
    }

    return f8;
}

static inline XfOtfCmapSubTableFormat10*
    sub_table_format10_init (XfOtfCmapSubTableFormat10* fa, Uint8* data, Size size) {
    RETURN_VALUE_IF (!fa || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT10_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 10\n"
    );

    fa->reserved        = GET_AND_ADV_U2 (data);
    fa->length          = GET_AND_ADV_U4 (data);
    fa->language        = GET_AND_ADV_U4 (data);
    fa->start_char_code = GET_AND_ADV_U4 (data);
    fa->num_chars       = GET_AND_ADV_U4 (data);

    if (fa->num_chars) {
        size -= SUB_TABLE_FORMAT10_DATA_SIZE;

        RETURN_VALUE_IF (
            size < sizeof (Uint16) * fa->num_chars,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 10\n"
        );

        RETURN_VALUE_IF (
            !(fa->glyph_id_array = ALLOCATE (Uint16, fa->num_chars)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        GET_ARR_AND_ADV_U2 (fa->glyph_id_array, 0, fa->num_chars);
    }

    return fa;
}

static inline XfOtfCmapSubTableFormat10* sub_table_format10_deinit (XfOtfCmapSubTableFormat10* fa) {
    RETURN_VALUE_IF (!fa, Null, ERR_INVALID_ARGUMENTS);

    if (fa->glyph_id_array) {
        FREE (fa->glyph_id_array);
        fa->glyph_id_array = Null;
    }

    return fa;
}

static inline XfOtfCmapSubTableFormat10*
    sub_table_format10_pprint (XfOtfCmapSubTableFormat10* f10, Uint8 indent_level) {
    RETURN_VALUE_IF (!f10, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 8 :\n"
        "|%s|format = 8 (Trimmed array)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n"
        "|%s|start_char_code = %u\n"
        "|%s|num_chars = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f10->length,
        indent,
        f10->language,
        indent,
        f10->start_char_code,
        indent,
        f10->num_chars
    );

    printf ("|%s|glyph_id_array = ", indent);
    PPRINT_ARR_U2 (f10->glyph_id_array, f10->num_chars - 1);

    return f10;
}

static inline XfOtfCmapSubTableFormat12*
    sub_table_format12_init (XfOtfCmapSubTableFormat12* fc, Uint8* data, Size size) {
    RETURN_VALUE_IF (!fc || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT12_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 12/13\n"
    );

    fc->reserved   = GET_AND_ADV_U2 (data);
    fc->length     = GET_AND_ADV_U4 (data);
    fc->language   = GET_AND_ADV_U4 (data);
    fc->num_groups = GET_AND_ADV_U4 (data);

    if (fc->num_groups) {
        size -= SUB_TABLE_FORMAT12_DATA_SIZE;

        RETURN_VALUE_IF (
            size < MAP_GROUP_DATA_SIZE * fc->num_groups,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 12/13\n"
        );

        RETURN_VALUE_IF (
            !(fc->groups = ALLOCATE (XfOtfCmapMapGroup, fc->num_groups)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        for (Size gidx = 0; gidx < fc->num_groups; gidx++) {
            if (!map_group_init (fc->groups + gidx, data, size)) {
                PRINT_ERR ("Failed to read sequential/constant map group\n");
                FREE (fc->groups);
                return Null;
            }
        }
    }

    return fc;
}

static inline XfOtfCmapSubTableFormat12* sub_table_format12_deinit (XfOtfCmapSubTableFormat12* fc) {
    RETURN_VALUE_IF (!fc, Null, ERR_INVALID_ARGUMENTS);

    if (fc->groups) {
        FREE (fc->groups);
        fc->groups = Null;
    }

    return fc;
}

static inline XfOtfCmapSubTableFormat12*
    sub_table_format12_pprint (XfOtfCmapSubTableFormat12* f12, Uint8 indent_level) {
    RETURN_VALUE_IF (!f12, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 12 :\n"
        "|%s|format = 12 (Segmented coverage)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n"
        "|%s|num_groups = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f12->length,
        indent,
        f12->language,
        indent,
        f12->num_groups
    );

    for (Size s = 0; s < f12->num_groups; s++) {
        map_group_pprint (f12->groups + s, indent_level + 1);
    }

    return f12;
}

static inline XfOtfCmapSubTableFormat13*
    sub_table_format13_pprint (XfOtfCmapSubTableFormat13* f13, Uint8 indent_level) {
    RETURN_VALUE_IF (!f13, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 12 :\n"
        "|%s|format = 13 (Many-to-one range mappings)\n"
        "|%s|length = %u\n"
        "|%s|language = %u\n"
        "|%s|num_groups = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        f13->length,
        indent,
        f13->language,
        indent,
        f13->num_groups
    );

    for (Size s = 0; s < f13->num_groups; s++) {
        map_group_pprint (f13->groups + s, indent_level + 1);
    }

    return f13;
}

static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_init (XfOtfCmapSubTableFormat14* fe, Uint8* data, Size size) {
    RETURN_VALUE_IF (!fe || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < SUB_TABLE_FORMAT14_DATA_SIZE,
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table format 14\n"
    );

    fe->length            = GET_AND_ADV_U4 (data);
    fe->num_var_selectors = GET_AND_ADV_U4 (data);

    if (fe->num_var_selectors) {
        size -= SUB_TABLE_FORMAT14_DATA_SIZE;

        RETURN_VALUE_IF (
            size < VAR_SELECTOR_DATA_SIZE,
            Null,
            "Data buffer size not sufficient for initialization of cmap sub_table format 14\n"
        );

        RETURN_VALUE_IF (
            !(fe->var_selectors = ALLOCATE (XfOtfCmapVarSelector, fe->num_var_selectors)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        for (Size vsidx = 0; vsidx < fe->num_var_selectors; vsidx++) {
            if (!var_selector_init (fe->var_selectors + vsidx, data, size)) {
                PRINT_ERR ("Failed to read sequential/constant map group\n");
                FREE (fe->var_selectors);
                return Null;
            }
        }
    }

    return fe;
}

static inline XfOtfCmapSubTableFormat14* sub_table_format14_deinit (XfOtfCmapSubTableFormat14* fe) {
    RETURN_VALUE_IF (!fe, Null, ERR_INVALID_ARGUMENTS);

    if (fe->var_selectors) {
        FREE (fe->var_selectors);
        fe->var_selectors = Null;
    }

    return fe;
}

static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_pprint (XfOtfCmapSubTableFormat14* fe, Uint8 indent_level) {
    RETURN_VALUE_IF (!fe, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|Cmap Sub Table Format 14\n"
        "|%s|format = 14 (Unicode Variation Sequences)\n"
        "|%s|length = %u\n"
        "|%s|num_var_selectors = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        indent,
        fe->length,
        indent,
        fe->num_var_selectors
    );

    for (Size s = 0; s < fe->num_var_selectors; s++) {
        var_selector_pprint (fe->var_selectors + s, indent_level + 1);
    }

    return fe;
}

static inline XfOtfCmapSubTable*
    sub_table_init (XfOtfCmapSubTable* sub_table, Uint8* data, Size size) {
    RETURN_VALUE_IF (!sub_table || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < sizeof (Uint16),
        Null,
        "Data buffer size not sufficient for initialization of cmap sub_table\n"
    );

    sub_table->format  = GET_AND_ADV_U2 (data);
    size              -= sizeof (Uint16);

    switch (sub_table->format) {
#define DEF_CASE(fmt)                                                                              \
    case fmt : {                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            !(sub_table->format##fmt = NEW (XfOtfCmapSubTableFormat##fmt)),                        \
            Null,                                                                                  \
            ERR_OUT_OF_MEMORY                                                                      \
        );                                                                                         \
        RETURN_VALUE_IF (                                                                          \
            !sub_table_format##fmt##_init (sub_table->format##fmt, data, size),                    \
            Null,                                                                                  \
            "Failed to read cmap sub_table format " #fmt "\n"                                      \
        );                                                                                         \
        return sub_table;                                                                          \
    }
        DEF_CASE (0)
        DEF_CASE (2)
        DEF_CASE (4)
        DEF_CASE (6)
        DEF_CASE (8)
        DEF_CASE (10)
        DEF_CASE (12)
        DEF_CASE (13)
        DEF_CASE (14)

#undef DEF_CASE
        default :
            PRINT_ERR ("Invalid sub table format \"%u\"\n", sub_table->format);
            return sub_table;
    }
}

static inline XfOtfCmapSubTable* sub_table_deinit (XfOtfCmapSubTable* sub_table) {
    RETURN_VALUE_IF (!sub_table, Null, ERR_INVALID_ARGUMENTS);

    switch (sub_table->format) {
#define DEF_CASE(fmt)                                                                              \
    case fmt : {                                                                                   \
        sub_table_format##fmt##_deinit (sub_table->format##fmt);                                   \
        break;                                                                                     \
    }

        case 0 :
            break;

            DEF_CASE (2)
            DEF_CASE (4)
            DEF_CASE (6)
            DEF_CASE (8)
            DEF_CASE (10)
            DEF_CASE (12)
            DEF_CASE (13)
            DEF_CASE (14)

#undef DEF_CASE
        default :
            PRINT_ERR ("Invalid sub table format \"%u\"\n", sub_table->format);
            return Null;
    }

    /* all union members have same value, so just free any one, the format does
     * not matter now. */
    if (sub_table->format0) {
        FREE (sub_table->format0);
        sub_table->format0 = Null;
    }

    return sub_table;
}

static inline XfOtfCmapSubTable*
    sub_table_pprint (XfOtfCmapSubTable* sub_table, Uint8 indent_level) {
    RETURN_VALUE_IF (!sub_table, Null, ERR_INVALID_ARGUMENTS);

    switch (sub_table->format) {
#define DEF_CASE(fmt)                                                                              \
    case fmt : {                                                                                   \
        sub_table_format##fmt##_pprint (sub_table->format##fmt, indent_level);                     \
        return sub_table;                                                                          \
    }

        DEF_CASE (0)
        DEF_CASE (2)
        DEF_CASE (4)
        DEF_CASE (6)
        DEF_CASE (8)
        DEF_CASE (10)
        DEF_CASE (12)
        DEF_CASE (13)
        DEF_CASE (14)

#undef DEF_CASE
        default :
            PRINT_ERR ("Invalid sub table format \"%u\"\n", sub_table->format);
            return Null;
    }
}
