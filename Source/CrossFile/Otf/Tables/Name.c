/**
 * @file Name.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Name.h>

/* libc */
#include <memory.h>

#include "Anvie/CrossFile/Otf/Tables/Common.h"

#define NAME_RECORD_DATA_SIZE                                                                      \
    (XF_OTF_PLATFORM_ENCODING_DATA_SIZE + XF_OTF_LANGUAGE_DATA_SIZE + sizeof (XfOtfNameId) +       \
     sizeof (Uint16) * 2)
#define LANG_TAG_RECORD_DATA_SIZE (sizeof (Uint16) * 2)
#define NAME_DATA_SIZE            (sizeof (Uint16) * 3)

static inline CString name_id_to_str (XfOtfNameId name_id);

static inline XfOtfNameRecord *name_record_init (XfOtfNameRecord *record, Uint8 *data, Size size);
static inline XfOtfNameRecord *name_record_pprint (XfOtfNameRecord *record, Uint8 indent_level);

static inline XfOtfLangTagRecord *
    lang_tag_record_init (XfOtfLangTagRecord *tag, Uint8 *data, Size size);
static inline XfOtfLangTagRecord *
    lang_tag_record_pprint (XfOtfLangTagRecord *tag, Uint8 indent_level);

/**************************************************************************************************/
/*********************************** PUBLIC METHOD DEFINITIONS ************************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfOtfName object.
 *
 * @param name
 * @param data
 * @param size
 *
 * @return @c name on success.
 * @return @c Null otherwise.
 * */
XfOtfName *xf_otf_name_init (XfOtfName *name, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!name || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < NAME_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize name table\n"
    );

    /* to be used later when loading string table */
    Uint8 *table_data_start     = data;
    Size   table_available_size = size;

    name->version          = GET_AND_ADV_U2 (data);
    name->num_name_records = GET_AND_ADV_U2 (data);
    name->storage_offset   = GET_AND_ADV_U2 (data);

    if (name->num_name_records) {
        size -= NAME_DATA_SIZE;

        RETURN_VALUE_IF (
            size < name->num_name_records * NAME_RECORD_DATA_SIZE,
            Null,
            "Data buffer size not sufficient to initialize name table\n"
        );

        RETURN_VALUE_IF (
            !(name->name_records = ALLOCATE (XfOtfNameRecord, name->num_name_records)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        for (Size s = 0; s < name->num_name_records; s++) {
            GOTO_HANDLER_IF (
                !name_record_init (name->name_records + s, data, size),
                INIT_FAILED,
                "Failed to read a name record from the name table\n"
            );
            data += NAME_RECORD_DATA_SIZE;
            size -= NAME_RECORD_DATA_SIZE;
        }
    }

    /* version 1 has some extra fields in between */
    if (name->version == 1) {
        GOTO_HANDLER_IF (
            size < sizeof (Uint16),
            INIT_FAILED,
            "Data buffer size not sufficient to initialize name table\n"
        );
        name->num_lang_tags = GET_AND_ADV_U2 (data);

        if (name->num_lang_tags) {
            GOTO_HANDLER_IF (
                size < name->num_lang_tags * LANG_TAG_RECORD_DATA_SIZE,
                INIT_FAILED,
                "Data buffer size not sufficient to initialize name table\n"
            );

            GOTO_HANDLER_IF (
                !(name->lang_tags = ALLOCATE (XfOtfLangTagRecord, name->num_lang_tags)),
                INIT_FAILED,
                ERR_OUT_OF_MEMORY
            );

            for (Size s = 0; s < name->num_lang_tags; s++) {
                GOTO_HANDLER_IF (
                    !lang_tag_record_init (name->lang_tags + s, data, size),
                    INIT_FAILED,
                    "Failed to read a name record from the name table\n"
                );
                data += LANG_TAG_RECORD_DATA_SIZE;
                size -= LANG_TAG_RECORD_DATA_SIZE;
            }
        }
    }

    name->string_data_size = table_available_size - name->storage_offset;
    name->string_data      = Null;
    if (name->string_data_size) {
        GOTO_HANDLER_IF (
            size < name->string_data_size,
            INIT_FAILED,
            "Data buffer size not sufficient to initialize name table\n"
        );

        GOTO_HANDLER_IF (
            !(name->string_data = ALLOCATE (Char, name->string_data_size)),
            INIT_FAILED,
            ERR_OUT_OF_MEMORY
        );

        memcpy (name->string_data, table_data_start + name->storage_offset, name->string_data_size);
    }

    return name;

INIT_FAILED:
    xf_otf_name_deinit (name);
    return Null;
}

/**
 * @b De-initialize given @c XfOtfName object.
 *
 * @param name
 *
 * @return @c name on success.
 * @return @c Null otherwise.
 * */
XfOtfName *xf_otf_name_deinit (XfOtfName *name) {
    RETURN_VALUE_IF (!name, Null, ERR_INVALID_ARGUMENTS);

    if (name->string_data) {
        FREE (name->string_data);
    }

    if (name->lang_tags) {
        FREE (name->lang_tags);
    }

    if (name->name_records) {
        FREE (name->name_records);
    }

    memset (name, 0, sizeof (XfOtfName));
    return name;
}

XfOtfName *xf_otf_name_pprint (XfOtfName *name, Uint8 indent_level) {
    RETURN_VALUE_IF (!name, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Name Table :\n"
        "|%s|version = %u\n"
        "|%s|num_name_records = %u\n"
        "|%s|storage_offset = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        name->version,
        indent,
        name->num_name_records,
        indent,
        name->storage_offset
    );

    for (Uint16 s = 0; s < MIN (name->num_name_records, 6); s++) {
        name_record_pprint (name->name_records + s, indent_level + 1);
    }

    if (name->num_name_records > 6) {
        printf (
            "|%s|.\n"
            "|%s|. (probably many more entries like this here)\n"
            "|%s|.\n",
            indent,
            indent,
            indent
        );

        name_record_pprint (name->name_records + name->num_name_records - 1, indent_level + 1);
    }

    for (Uint16 s = 0; s < MIN (name->num_lang_tags, 6); s++) {
        lang_tag_record_pprint (name->lang_tags + s, indent_level + 1);
    }

    if (name->num_lang_tags > 6) {
        printf (
            "|%s|.\n"
            "|%s|. (probably many more entries like this here)\n"
            "|%s|.\n",
            indent,
            indent,
            indent
        );

        lang_tag_record_pprint (name->lang_tags + name->num_lang_tags - 1, indent_level + 1);
    }

    if (name->string_data_size) {
        printf (
            "|%s|string_data = %.*s...\n",
            indent,
            MIN (name->string_data_size, 32),
            name->string_data
        );
    }

    return name;
}

/**************************************************************************************************/
/*********************************** PRIVATE METHOD DEFINITIONS ***********************************/
/**************************************************************************************************/

static inline XfOtfNameRecord *name_record_init (XfOtfNameRecord *record, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!record || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < NAME_RECORD_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize name record\n"
    );

    record->platform_encoding.platform        = GET_AND_ADV_U2 (data);
    record->platform_encoding.encoding.custom = GET_AND_ADV_U2 (data);

    record->language.language.custom = GET_AND_ADV_U2 (data);
    record->language.platform        = record->platform_encoding.platform;

    record->name_id       = GET_AND_ADV_U2 (data);
    record->length        = GET_AND_ADV_U2 (data);
    record->string_offset = GET_AND_ADV_U2 (data);

    return record;
}

static inline XfOtfNameRecord *name_record_pprint (XfOtfNameRecord *record, Uint8 indent_level) {
    RETURN_VALUE_IF (!record, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Name Record :\n"
        "|%s|platform = %x (%s)\n"
        "|%s|encoding_id = %x (%s)\n"
        "|%s|language = %x (%s)\n"
        "|%s|name_id = %u (%s)\n"
        "|%s|length = %u\n"
        "|%s|string_offset = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        record->platform_encoding.platform,
        xf_otf_platform_encoding_get_platform_str (record->platform_encoding),
        indent,
        record->platform_encoding.encoding.custom,
        xf_otf_platform_encoding_get_encoding_str (record->platform_encoding),
        indent,
        record->language.language.custom,
        xf_otf_language_to_str (record->language),
        indent,
        record->name_id,
        name_id_to_str (record->name_id),
        indent,
        record->length,
        indent,
        record->string_offset
    );

    return record;
}

static inline XfOtfLangTagRecord *
    lang_tag_record_init (XfOtfLangTagRecord *tag, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!tag || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < LANG_TAG_RECORD_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize lang tag record.\n"
    );

    tag->length          = GET_AND_ADV_U2 (data);
    tag->lang_tag_offset = GET_AND_ADV_U2 (data);

    return tag;
}

static inline XfOtfLangTagRecord *
    lang_tag_record_pprint (XfOtfLangTagRecord *tag, Uint8 indent_level) {
    RETURN_VALUE_IF (!tag, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Lang Tag Record :\n"
        "|%s|length = %u\n"
        "|%s|lang_tag_offset = %u\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        tag->length,
        indent,
        tag->lang_tag_offset
    );

    return tag;
}

static inline CString name_id_to_str (XfOtfNameId id) {
    // Array of name ids corresponding to the enum values
    static const char *names[] = {
        "COPYRIGHT NOTICE",                 // 0
        "FONT FAMILY NAME",                 // 1
        "FONT SUBFAMILY NAME",              // 2
        "UNIQUE FONT IDENTIFIER",           // 3
        "FULL FONT FAMILY NAME",            // 4
        "VERSION STRING",                   // 5
        "POSTSCRIPT NAME",                  // 6
        "TRADEMARK",                        // 7
        "MANUFACTURER NAME",                // 8
        "DESIGNER NAME",                    // 9
        "DESCRIPTION",                      // 10
        "VENDOR URL",                       // 11
        "DESIGNER URL",                     // 12
        "LICENSE",                          // 13
        "LICENSE URL",                      // 14
        "RESERVED0",                        // 15
        "TYPOGRAPHIC FAMILY NAME",          // 16
        "TYPOGRAPHIC SUBFAMILY NAME",       // 17
        "MAC COMPATIBLE FULL",              // 18
        "SAMPLE TEXT",                      // 19
        "POSTSCRIPT CID FONT NAME",         // 20
        "WWS FAMILY NAME",                  // 21
        "WWS SUBFAMILY NAME",               // 22
        "LIGHT BACKGROUND PALETTE",         // 23
        "DARK BACKGROUND PALETTE",          // 24
        "VARIATIONS POSTSCRIPT NAME PREFIX" // 25
    };

    // Check if the value is within the reserved range
    if (id >= XF_OTF_NAME_ID_RESERVED_MIN) {
        return "RESERVED";
    }

    // Return the corresponding string from the array
    return names[id];
}
