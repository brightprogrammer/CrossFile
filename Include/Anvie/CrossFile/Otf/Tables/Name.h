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

/* REF : https://learn.microsoft.com/en-us/typography/opentype/spec/name */

typedef struct XfOtfNameRecord {
    XfOtfPlatformEncoding platform_encoding;
    Uint16 language_id;
    Uint16 name_id;
    Uint16 length;
    Uint16 string_offset;
} XfOtfNameRecord;

typedef struct XfOtfLangTagRecord {
    Uint16 length;
    Uint16 lang_tag_offset;
} XfOtfLangTagRecord;

/**
 * @b A mix of Name V0 and V1. Based on the version information,
 * fields are loaded.
 * */
typedef struct XfOtfName {
    Uint16              version;          /* v0, v1 */
    Uint16              num_name_records; /* v0, v1 */
    Uint16              storage_offset;   /* v0, v1 */
    XfOtfNameRecord    *name_records;     /* v0, v1 */
    Uint16              num_lang_tags;    /* v1 */
    XfOtfLangTagRecord *lang_tags;        /* v1 */
    Uint16              string_data_size;
    Char               *string_data;      /* v0, v1 */
} XfOtfName;

XfOtfName *xf_otf_name_init (XfOtfName *name, Uint8 *data, Size size);
XfOtfName *xf_otf_name_deinit (XfOtfName *name);
XfOtfName *xf_otf_name_pprint (XfOtfName *name, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_NAME_H
