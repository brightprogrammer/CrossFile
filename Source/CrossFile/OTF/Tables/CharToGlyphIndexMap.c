/**
 * @file CharToGlyphIndexMap.c
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
#include <Anvie/CrossFile/Otf/Tables/CharToGlyphIndexMap.h>

/* libc */
#include <memory.h>

/**************************************************************************************************/
/******************************* ENCODING RECORD METHOD DEFINITIONS *******************************/
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
XfOtfEncodingRecord*
    xf_otf_encoding_record_init (XfOtfEncodingRecord* enc, Uint8* data, Size size) {
    RETURN_VALUE_IF (!enc || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < XF_OTF_ENCODING_RECORD_DATA_SIZE,
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
XfOtfEncodingRecord* xf_otf_encoding_record_pprint (XfOtfEncodingRecord* enc) {
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

/**************************************************************************************************/
/*************************** CHAR TO GLYPH INDEX MAP METHOD DEFINITIONS ***************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfOtfCharToGlyphIndexMap by reading and adjusting
 *    endinanness of data from given @c data buffer.
 *
 * @param cmap To be initialized.
 * @param data Data array containing data in Big Endian format to be loaded.
 * @param size Size of @c data.
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCharToGlyphIndexMap*
    xf_otf_char_to_glyph_index_map_init (XfOtfCharToGlyphIndexMap* cmap, Uint8* data, Size size) {
    RETURN_VALUE_IF (!cmap || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < XF_OTF_CHAR_TO_GLYPH_INDEX_MAP_DATA_SIZE,
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

    size -= XF_OTF_CHAR_TO_GLYPH_INDEX_MAP_DATA_SIZE;

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        if (!xf_otf_encoding_record_init (cmap->encoding_records + table_idx, data, size)) {
            PRINT_ERR (
                "Failed to read all encoding records in character to glyph index map \"cmap\".\n"
            );
            xf_otf_char_to_glyph_index_map_deinit (cmap);
            return Null;
        }

        size -= XF_OTF_ENCODING_RECORD_DATA_SIZE;
        data += XF_OTF_ENCODING_RECORD_DATA_SIZE;
    }

    return cmap;
}

/**
 * @b De-initialize contents of given @c XfOtfCharToGlyphIndexMap struct.
 *
 * @param cmap To be de-initialized
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCharToGlyphIndexMap* xf_otf_char_to_glyph_index_map_deinit (XfOtfCharToGlyphIndexMap* cmap) {
    RETURN_VALUE_IF (!cmap, Null, ERR_INVALID_ARGUMENTS);

    if (cmap->encoding_records) {
        FREE (cmap->encoding_records);
    }

    memset (cmap, 0, sizeof (XfOtfCharToGlyphIndexMap));

    return cmap;
}

/**
 * @b Pretty print contents of given @c XfOtfCharToGlyphIndexMap struct.
 *
 * @param cmap To be pretty-printed.
 *
 * @return @c cmap on success.
 * @return @c Null otherwise.
 * */
XfOtfCharToGlyphIndexMap* xf_otf_char_to_glyph_index_map_pprint (XfOtfCharToGlyphIndexMap* cmap) {
    RETURN_VALUE_IF (!cmap, Null, ERR_INVALID_ARGUMENTS);

    printf (
        "OTF Character To Glyph Index Map Table :\n"
        "\tversion = %u\n"
        "\tnum_tables = %u\n",
        cmap->version,
        cmap->num_tables
    );

    for (Uint16 table_idx = 0; table_idx < cmap->num_tables; table_idx++) {
        xf_otf_encoding_record_pprint (cmap->encoding_records + table_idx);
    }

    return cmap;
}
