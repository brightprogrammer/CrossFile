/**
 * @file Name.h
 * @date Sun, 26th May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_NAME_H
#define ANVIE_CROSSFILE_OTF_TABLES_NAME_H

#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Otf/Tables/Common.h>

typedef enum OtfNameId : Uint16 {
    OTF_NAME_ID_MIN                               = 0,
    OTF_NAME_ID_COPYRIGHT_NOTICE                  = 0,
    OTF_NAME_ID_FONT_FAMILY_NAME                  = 1,
    OTF_NAME_ID_FONT_SUBFAMILY_NAME               = 2,
    OTF_NAME_ID_UNIQUE_FONT_IDENTIFIER            = 3,
    OTF_NAME_ID_FULL_FONT_FAMILY_NAME             = 4,
    OTF_NAME_ID_VERSION_STRING                    = 5,
    OTF_NAME_ID_POSTSCRIPT_NAME                   = 6,
    OTF_NAME_ID_TRADEMARK                         = 7,
    OTF_NAME_ID_MANUFACTURER_NAME                 = 8,
    OTF_NAME_ID_DESIGNER_NAME                     = 9,
    OTF_NAME_ID_DESCRIPTION                       = 10,
    OTF_NAME_ID_VENDOR_URL                        = 11,
    OTF_NAME_ID_DESIGNER_URL                      = 12,
    OTF_NAME_ID_LICENSE                           = 13,
    OTF_NAME_ID_LICENSE_URL                       = 14,
    OTF_NAME_ID_RESERVED0                         = 15,
    OTF_NAME_ID_TYPOGRAPHIC_FAMILY_NAME           = 16,
    OTF_NAME_ID_TYPOGRAPHIC_SUBFAMILY_NAME        = 17,
    OTF_NAME_ID_MAC_COMPATIBLE_FULL               = 18,
    OTF_NAME_ID_SAMPLE_TEXT                       = 19,
    OTF_NAME_ID_POSTSCRIPT_CID_FONT_NAME          = 20,
    OTF_NAME_ID_WWS_FAMILY_NAME                   = 21,
    OTF_NAME_ID_WWS_SUBFAMILY_NAME                = 22,
    OTF_NAME_ID_LIGHT_BACKGROUND_PALETTE          = 23,
    OTF_NAME_ID_DARK_BACKGROUND_PALETTE           = 24,
    OTF_NAME_ID_VARIATIONS_POSTSCRIPT_NAME_PREFIX = 25,
    OTF_NAME_ID_MAX                               = 25,
    OTF_NAME_ID_RESERVED_MIN                      = 26,
    OTF_NAME_ID_RESERVED_MAX                      = 0xffff
} OtfNameId;

/* REF : https://learn.microsoft.com/en-us/typography/opentype/spec/name */

typedef struct OtfNameRecord {
    OtfPlatformEncoding platform_encoding;
    OtfLanguage         language;
    OtfNameId           name_id;
    Uint16              length;
    Uint16              string_offset;
} OtfNameRecord;

typedef struct OtfLangTagRecord {
    Uint16 length;
    Uint16 lang_tag_offset;
} OtfLangTagRecord;

/**
 * @b A mix of Name V0 and V1. Based on the version information,
 * fields are loaded.
 * */
typedef struct OtfName {
    Uint16            version;          /* v0, v1 */
    Uint16            num_name_records; /* v0, v1 */
    Uint16            storage_offset;   /* v0, v1 */
    OtfNameRecord    *name_records;     /* v0, v1 */
    Uint16            num_lang_tags;    /* v1 */
    OtfLangTagRecord *lang_tags;        /* v1 */
    Uint16            string_data_size;
    Char             *string_data;      /* v0, v1 */
} OtfName;

OtfName *otf_name_init (OtfName *name, Uint8 *data, Size size);
OtfName *otf_name_deinit (OtfName *name);
OtfName *otf_name_pprint (OtfName *name, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_NAME_H
