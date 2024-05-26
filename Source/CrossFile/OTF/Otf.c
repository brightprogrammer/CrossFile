/**
 * @file Otf.c
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

#include <Anvie/CrossFile/Otf/Otf.h>

/* libc */
#include <memory.h>

#include "Anvie/CrossFile/Otf/Tables/Maxp.h"

XfOtfFile* xf_otf_file_open (XfOtfFile* otf_file, CString filename) {
    RETURN_VALUE_IF (!otf_file || !filename, Null, ERR_INVALID_ARGUMENTS);

    /* read whole file */
    RETURN_VALUE_IF (!xf_file_open (&otf_file->file, filename), Null, ERR_FILE_OPEN_FAILED);

    /* load table directory */
    xf_otf_table_dir_init (&otf_file->table_directory, otf_file->file.data, otf_file->file.size);

    /* if this value is 8 in the end then we've possibly found all the required records
     * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/otff#font-tables */
    Uint8 found_required_records_count = 0;

    /* iterate over table records and load known records */
    for (Size s = 0; s < otf_file->table_directory.num_tables; s++) {
        /* get record */
        XfOtfTableRecord* record = otf_file->table_directory.table_records + s;

        switch (record->table_tag) {
            case XF_OTF_TABLE_TAG_CMAP : {
                RETURN_VALUE_IF (
                    !xf_otf_cmap_init (
                        &otf_file->cmap,
                        otf_file->file.data + record->offset,
                        record->length
                    ),
                    Null,
                    "Failed to initialize char to glyph index map \"cmap\".\n"
                );
                found_required_records_count++;
                break;
            }

            case XF_OTF_TABLE_TAG_HEAD : {
                RETURN_VALUE_IF (
                    !xf_otf_head_init (
                        &otf_file->head,
                        otf_file->file.data + record->offset,
                        record->length
                    ),
                    Null,
                    "Failed to initialize font header table \"head\".\n"
                );
                found_required_records_count++;
                break;
            }

            case XF_OTF_TABLE_TAG_MAXP : {
                RETURN_VALUE_IF (
                    !xf_otf_maxp_init (
                        &otf_file->maxp,
                        otf_file->file.data + record->offset,
                        record->length
                    ),
                    Null,
                    "Failed to initialize max profile table \"maxp\".\n"
                );
                found_required_records_count++;
                break;
            }

            default :
                continue;
        }
    }

    /* check whether we've found all required records */
    GOTO_HANDLER_IF (
        found_required_records_count != 3,
        INIT_FAILED,
        "Failed to find some of the required table records.\n"
    );

    return otf_file;

INIT_FAILED:
    xf_otf_file_close (otf_file);
    return Null;
}

XfOtfFile* xf_otf_file_close (XfOtfFile* otf_file) {
    RETURN_VALUE_IF (!otf_file, Null, ERR_INVALID_ARGUMENTS);

    xf_otf_cmap_deinit (&otf_file->cmap);

    if (otf_file->table_directory.table_records) {
        FREE (otf_file->table_directory.table_records);
    }

    xf_file_close (&otf_file->file);

    memset (otf_file, 0, sizeof (XfOtfFile));

    return otf_file;
}

XfOtfFile* xf_otf_file_pprint (XfOtfFile* otf_file, Uint8 indent_level) {
    RETURN_VALUE_IF (!otf_file, Null, ERR_INVALID_ARGUMENTS);

    /* make sure indent level is atleast 1 always */
    indent_level = indent_level ? indent_level : 1;

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OpenType Font File : \n"
        "|%s|name : %s\n"
        "|%s|size : %.2f KiB\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        otf_file->file.name,
        indent,
        otf_file->file.size / 1024.f
    );

    xf_otf_table_dir_pprint (&otf_file->table_directory, indent_level + 1);
    xf_otf_head_pprint (&otf_file->head, indent_level + 1);
    xf_otf_maxp_pprint (&otf_file->maxp, indent_level + 1);
    xf_otf_cmap_pprint (&otf_file->cmap, indent_level + 1);

    return otf_file;
}
