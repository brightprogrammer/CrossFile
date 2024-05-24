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
static inline XfOtfEncodingRecord*
    encoding_record_init (XfOtfEncodingRecord* enc, Uint8* data, Size size);
static inline XfOtfEncodingRecord* encoding_record_pprint (XfOtfEncodingRecord* enc);
static inline XfOtfCmapSubHeader*
    sub_header_init (XfOtfCmapSubHeader* sub_head, Uint8* data, Size size);
static inline XfOtfCmapMapGroup* map_group_init (XfOtfCmapMapGroup* group, Uint8* data, Size size);
static inline XfOtfCmapVarSelector*
    var_selector_init (XfOtfCmapVarSelector* sel, Uint8* data, Size size);
static inline XfOtfCmapUnicodeRange*
    unicode_range_init (XfOtfCmapUnicodeRange* range, Uint8* data, Size size);
static inline XfOtfCmapDefaultUVSTable*
    default_uvs_table_init (XfOtfCmapDefaultUVSTable* default_uvs, Uint8* data, Size size);
static inline XfOtfCmapUVSMapping*
    uvs_mapping_init (XfOtfCmapUVSMapping* uvs_map, Uint8* data, Size size);
static inline XfOtfCmapNonDefaultUVSTable* non_default_uvs_table_init (
    XfOtfCmapNonDefaultUVSTable* non_default_uvs,
    Uint8*                       data,
    Size                         size
);
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
static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_init (XfOtfCmapSubTableFormat14* f14, Uint8* data, Size size);

/* size limit definitions for error checking */
#define ENCODING_RECORD_DATA_SIZE       (sizeof (XfOtfPlatformID) + sizeof (Uint16) + sizeof (Uint32))
#define CMAP_DATA_SIZE                  (sizeof (Uint16) * 2)
#define SUB_HEADER_DATA_SIZE            (sizeof (Uint16) * 3 + sizeof (Int16))
#define MAP_GROUP_DATA_SIZE             (sizeof (Uint32) * 3)
#define VAR_SELECTOR_DATA_SIZE          (sizeof (Uint8) * 3 + sizeof (Uint32) * 2)
#define UNICODE_RANGE_DATA_SIZE         (sizeof (Uint8) * 3 + sizeof (Uint8))
#define DEFAULT_UVS_TABLE_DATA_SIZE     (sizeof (Uint32))
#define UVS_MAPPING_DATA_SIZE           (sizeof (Uint8) * 3 + sizeof (Uint16))
#define NON_DEFAULT_UVS_TABLE_DATA_SIZE (sizeof (Uint32))
#define SUB_TABLE_FORMAT0_DATA_SIZE     (sizeof (Uint16) * 2 + sizeof (Uint8) * 256)
#define SUB_TABLE_FORMAT2_DATA_SIZE                                                                \
    (sizeof (Uint16) * 3 + sizeof (XfOtfCmapSubTableSubHeader*) + sizeof (Uint16*))
#define SUB_TABLE_FORMAT4_DATA_SIZE  (sizeof (Uint16) * 8)
#define SUB_TABLE_FORMAT6_DATA_SIZE  (sizeof (Uint16) * 5)
#define SUB_TABLE_FORMAT8_DATA_SIZE  (sizeof (Uint16) + sizeof (Uint32) * 3 + sizeof (Uint8) * 8192)
#define SUB_TABLE_FORMAT10_DATA_SIZE (sizeof (Uint16) + sizeof (Uint32) * 4)
#define SUB_TABLE_FORMAT12_DATA_SIZE (sizeof (Uint16) + sizeof (Uint32) * 3)
#define SUB_TABLE_FORMAT13_DATA_SIZE SUB_TABLE_FORMAT12_DATA_SIZE
#define SUB_TABLE_FORMAT14_DATA_SIZE (sizeof (Uint32) * 2)

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

    cmap->version    = GET_AND_ADV_U2 (data);
    cmap->num_tables = GET_AND_ADV_U2 (data);

    if (!cmap->num_tables) {
        return cmap;
    }

    cmap->encoding_records = ALLOCATE (XfOtfEncodingRecord, cmap->num_tables);
    RETURN_VALUE_IF (!cmap->encoding_records, Null, ERR_OUT_OF_MEMORY);

    size -= CMAP_DATA_SIZE;

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        if (!encoding_record_init (cmap->encoding_records + table_idx, data, size)) {
            PRINT_ERR (
                "Failed to read all encoding records in character to glyph index map "
                "\"cmap\".\n"
            );
            xf_otf_cmap_deinit (cmap);
            return Null;
        }

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
        FREE (cmap->encoding_records);
    }

    memset (cmap, 0, sizeof (XfOtfCmap));

    return cmap;
}

/**
 * @b Pretty print contents of given @c XfOtfCmap struct.
 *
 * @param cmap To be pretty-printed.
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCmap* xf_otf_cmap_pprint (XfOtfCmap* cmap) {
    RETURN_VALUE_IF (!cmap, Null, ERR_INVALID_ARGUMENTS);

    printf (
        "OTF Character To Glyph Index Map Table :\n"
        "\tversion = %u\n"
        "\tnum_tables = %u\n",
        cmap->version,
        cmap->num_tables
    );

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        encoding_record_pprint (cmap->encoding_records + table_idx);
    }

    return cmap;
}

/**************************************************************************************************/
/*********************************** PRIVATE METHOD DEFINITIONS ***********************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfOtfEncodingRecord by reading and adjusting
*    endinanness of data from given @c data buffer.
 *
 * @param enc To be initialized.
 * @param data Data array containing data in Big Endian format to be loaded.
 * @param size Size of @c data.
 *
 * @return @c enc on success.
 * @return @c Null otherwise.
 * */
static inline XfOtfEncodingRecord*
    encoding_record_init (XfOtfEncodingRecord* enc, Uint8* data, Size size) {
    RETURN_VALUE_IF (!enc || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < ENCODING_RECORD_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize encoding record.\n"
    );

    enc->platform_id        = GET_AND_ADV_U2 (data);
    enc->encoding_id.custom = GET_AND_ADV_U2 (data);
    enc->subtable_offset    = GET_AND_ADV_U4 (data);

    return enc;
}

/**
 * @b Pretty print contents of given @c XfOtfEncodingRecord struct.
 *
 * @param enc To be pretty-printed.
 *
 * @return @c enc on success.
 * @return @c Null otherwise.
 * */
static inline XfOtfEncodingRecord* encoding_record_pprint (XfOtfEncodingRecord* enc) {
    RETURN_VALUE_IF (!enc, Null, ERR_INVALID_ARGUMENTS);

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

    if (enc->platform_id <= XF_OTF_PLATFORM_ID_MAX) {
        CString encoding_format_str = Null;
        switch (enc->platform_id) {
            case XF_OTF_PLATFORM_ID_VARIOUS : {
                if (enc->encoding_id.various <= XF_OTF_VARIOUS_ENCODING_ID_MAX) {
                    encoding_format_str = enc_various_to_str_map[enc->encoding_id.various];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_PLATFORM_ID_MAC : {
                if (enc->encoding_id.mac <= XF_OTF_MAC_ENCODING_ID_MAX) {
                    encoding_format_str = enc_mac_to_str_map[enc->encoding_id.mac];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_PLATFORM_ID_ISO : {
                if (enc->encoding_id.iso <= XF_OTF_ISO_ENCODING_ID_MAX) {
                    encoding_format_str = enc_iso_to_str_map[enc->encoding_id.iso];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_PLATFORM_ID_WIN : {
                if (enc->encoding_id.win <= XF_OTF_WIN_ENCODING_ID_MAX) {
                    encoding_format_str = enc_win_to_str_map[enc->encoding_id.win];
                } else {
                    encoding_format_str = "Unknown";
                }
                break;
            }
            case XF_OTF_PLATFORM_ID_CUSTOM :
                encoding_format_str = "Custom";
                break;
        }

        printf (
            "OTF Char To Glyph Map Encoding Format :\n"
            "\tplatform_id = %x (%s)\n"
            "\tencoding_id = %x (%s)\n"
            "\tsubtable_offset = %08x\n",
            enc->platform_id,
            platform_id_to_str_map[enc->platform_id],
            enc->encoding_id.custom,
            encoding_format_str,
            enc->subtable_offset
        );
    } else {
        printf (
            "OTF Char To Glyph Map Encoding Format :\n"
            "\tplatform_id = %x (Unknown)\n"
            "\tencoding_id = %x (Unknown)\n"
            "\tsubtable_offset = %08x\n",
            enc->platform_id,
            enc->encoding_id.custom,
            enc->subtable_offset
        );
    }

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
static inline XfOtfCmapSubTableFormat13*
    sub_table_format13_init (XfOtfCmapSubTableFormat13* f13, Uint8* data, Size size);
static inline XfOtfCmapSubTableFormat14*
    sub_table_format14_init (XfOtfCmapSubTableFormat14* f14, Uint8* data, Size size);
