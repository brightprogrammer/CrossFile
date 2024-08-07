/**
 * @file Tables.h
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

#ifndef ANVIE_CROSSGUI_OTF_TABLES_H
#define ANVIE_CROSSGUI_OTF_TABLES_H

#include <Anvie/CrossFile/Otf/Tables/Cmap.h>
#include <Anvie/CrossFile/Otf/Tables/Head.h>
#include <Anvie/CrossFile/Otf/Tables/Hhea.h>
#include <Anvie/CrossFile/Otf/Tables/Hmtx.h>
#include <Anvie/CrossFile/Otf/Tables/Loca.h>
#include <Anvie/CrossFile/Otf/Tables/Maxp.h>
#include <Anvie/CrossFile/Otf/Tables/Name.h>

/**
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/otff#font-tables
 *
 * All these values are still in BigEndian format.
 * This means the table tag will never be converted to LittleEndian
 * anywhwere in the program. That being said, it won't matter much,
 * because everywhere, these enums are recommended.
 * */
typedef enum OtfTableTag : Uint32 {
    /* required for proper functioning of otf files */
    OTF_TABLE_TAG_CMAP = 0x70616d63, /* cmap */
    OTF_TABLE_TAG_HEAD = 0x64616568, /* head */
    OTF_TABLE_TAG_HHEA = 0x61656868, /* hhea */
    OTF_TABLE_TAG_HMTX = 0x78746d68, /* hmtx */
    OTF_TABLE_TAG_MAXP = 0x7078616d, /* maxp */
    OTF_TABLE_TAG_NAME = 0x656d616e, /* name */
    OTF_TABLE_TAG_OS_2 = 0x325f534f, /* OS/2 */
    OTF_TABLE_TAG_POST = 0x74736f70, /* post */

    /* required for font description */
    OTF_TABLE_TAG_CVT  = 0x20747663, /* cvt  */
    OTF_TABLE_TAG_FPGM = 0x6d677066, /* fpgm */
    OTF_TABLE_TAG_GLYF = 0x66796c67, /* glyf */
    OTF_TABLE_TAG_LOCA = 0x61636f6c, /* loca */
    OTF_TABLE_TAG_PREP = 0x70657270, /* prep */
    OTF_TABLE_TAG_GASP = 0x70736167  /* gasp */
} OtfTableTag;

/**
 * @b Describes a single table among various other tables in OTF files.
 *
 * The table directory (@c OtfTableDir) describes an array
 * of this structure.
 *
 * REF: https://learn.microsoft.com/en-us/typography/opentype/spec/otff#table-directory
 * */
typedef struct OtfTableRecord {
    OtfTableTag table_tag;
    Uint32      checksum;
    Uint32      offset; /**< @b Offset from beginning of file. */
    Uint32      length; /**< @b Length of table. */
} OtfTableRecord;

#define OTF_TABLE_RECORD_DATA_SIZE (sizeof (OtfTableTag) + sizeof (Uint32) * 3)

OtfTableRecord* otf_table_record_init (OtfTableRecord* record, Uint8* data, Size size);
OtfTableRecord* otf_table_record_pprint (OtfTableRecord* record, Uint8 indent_level);

/**
 * @b Top level struct in OTF files.
 *
 * This appears at the very beginning of OTF files and describes
 * the top level directory of tables for a single font, or directory of
 * tables for multiple fonts (depending on the sfnt_version field).
 *
 * REF: https://learn.microsoft.com/en-us/typography/opentype/spec/otff#table-directory
 * */
typedef struct OtfTableDir {
    Uint32          sfnt_version;
    Uint16          num_tables;
    Uint16          search_range;
    Uint16          entry_selector;
    Uint16          range_shift;
    OtfTableRecord* table_records;
} OtfTableDir;

#define OTF_TABLE_DIR_DATA_SIZE (sizeof (Uint16) * 4 + sizeof (Uint32))

OtfTableDir*    otf_table_dir_init (OtfTableDir* dir, Uint8* data, Size size);
OtfTableDir*    otf_table_dir_deinit (OtfTableDir* dir);
OtfTableRecord* otf_table_dir_find_record (OtfTableDir* dir, OtfTableTag table_tag);
OtfTableDir*    otf_table_dir_pprint (OtfTableDir* dir, Uint8 indent_level);

#endif // ANVIE_CROSSGUI_OTF_TABLES_H
