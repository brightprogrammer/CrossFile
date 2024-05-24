/**
 * @file Tables.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables.h>

/* libc */
#include <memory.h>

/**************************************************************************************************/
/*************************************** OTF TABLE RECORDS ****************************************/
/**************************************************************************************************/

/**
 * @b Initialize the given table record with valid endianness.
 *
 * @param record reference to table record to be initialized.
 * @param data Reference to memory that contains raw data.
 *
 * @return @c record on success.
 * @return @c Null otherwise.
 * */
XfOtfTableRecord* xf_otf_table_record_init (XfOtfTableRecord* record, Uint8* data, Size size) {
    RETURN_VALUE_IF (!record || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < 4 * sizeof (Uint32),
        Null,
        "Data buffer size not sufficient to initialize table record.\n"
    );

    record->table_tag  = ((Uint32*)data)[0];
    data              += 4;
    record->checksum   = GET_AND_ADV_U4 (data);
    record->offset     = GET_AND_ADV_U4 (data);
    record->length     = GET_AND_ADV_U4 (data);

    return record;
}

/**
 * @b Pretty print given table record.
 *
 * @param record
 *
 * @return @c record on success.
 * @return @c Null otherwise.
 * */
XfOtfTableRecord* xf_otf_table_record_pprint (XfOtfTableRecord* record) {
    RETURN_VALUE_IF (!record, Null, ERR_INVALID_ARGUMENTS);

    printf (
        "OTF Table Record : \n"
        "\ttable_tag : 0x%08x (%.*s)\n"
        "\tchecksum : 0x%08x\n"
        "\toffset : %u\n"
        "\tlength : %u\n",
        record->table_tag,
        4, /* length of table tag string */
        (Char*)&record->table_tag,
        record->checksum,
        record->offset,
        record->length
    );

    return record;
}

/**************************************************************************************************/
/************************************** OTF TABLE DIRECTORY ***************************************/
/**************************************************************************************************/

/**
 * @b Initialize the given table directory with valid endianness.
 *
 * @param dir Reference to table directory to be initialized.
 * @param data Reference to memory that contains raw data.
 *
 * @return @c dir on success.
 * @return @c Null otherwise.
 * */
XfOtfTableDir* xf_otf_table_dir_init (XfOtfTableDir* dir, Uint8* data, Size size) {
    RETURN_VALUE_IF (!dir || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < XF_OTF_TABLE_DIR_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize font header table.\n"
    );

    dir->sfnt_version = GET_AND_ADV_U4 (data);
    RETURN_VALUE_IF (
        (dir->sfnt_version != 0x00010000 /* version 1.0 */ &&
         dir->sfnt_version != 0x4F54544F /* version OTTO */),
        Null,
        "Invalid sfnt_version in top level table directory.\n"
    );

    dir->num_tables = GET_AND_ADV_U2 (data);
    RETURN_VALUE_IF (!dir->num_tables, Null, "Table directory indicates absence of tables.\n");

    dir->search_range   = GET_AND_ADV_U2 (data);
    dir->entry_selector = GET_AND_ADV_U2 (data);
    dir->range_shift    = GET_AND_ADV_U2 (data);

    dir->table_records = ALLOCATE (XfOtfTableRecord, dir->num_tables);
    RETURN_VALUE_IF (!dir->table_records, Null, ERR_OUT_OF_MEMORY);

    size -= XF_OTF_TABLE_DIR_DATA_SIZE;

    for (Size table_idx = 0; table_idx < dir->num_tables; table_idx++) {
        if (!xf_otf_table_record_init (dir->table_records + table_idx, data, size)) {
            PRINT_ERR ("Failed to read a table record\n");
            xf_otf_table_dir_deinit (dir);
            return Null;
        }
        data += XF_OTF_TABLE_RECORD_DATA_SIZE;
        size -= XF_OTF_TABLE_RECORD_DATA_SIZE;
    }

    return dir;
}

/**
 * @b Deinitialize the given table directory by freeing all resources
 *    allocated by the init method.
 *
 * @param dir
 *
 * @return @c dir on success.
 * @return @c Null otherwise.
 * */
XfOtfTableDir* xf_otf_table_dir_deinit (XfOtfTableDir* dir) {
    RETURN_VALUE_IF (!dir, Null, ERR_INVALID_ARGUMENTS);

    if (dir->table_records) {
        FREE (dir->table_records);
    }

    memset (dir, 0, sizeof (XfOtfTableDir));

    return dir;
}

/**
 * @b Find table record entry based on given table tag.
 *
 * @param dir Table directory containing the required table record.
 * @param table_tag Table tag of required table.
 *
 * @return @c dir on success.
 * @return @c Null otherwise.
 * */
XfOtfTableRecord* xf_otf_table_dir_find_record (XfOtfTableDir* dir, XfOtfTableTag table_tag) {
    RETURN_VALUE_IF (!dir, Null, ERR_INVALID_ARGUMENTS);

    for (Size s = 0; s < dir->num_tables; s++) {
        if (dir->table_records[s].table_tag == table_tag) {
            return dir->table_records + s;
        }
    }

    return Null;
}

/**
 * @b Pretty print given table record.
 *
 * @param record
 *
 * @return @c record on success.
 * @return @c Null otherwise.
 * */
XfOtfTableDir* xf_otf_table_dir_pprint (XfOtfTableDir* dir) {
    RETURN_VALUE_IF (!dir, Null, ERR_INVALID_ARGUMENTS);

    printf (
        "OTF Table Directory : \n"
        "\tsfnt_version : 0x%08x\n"
        "\tnum_tables : %u\n"
        "\tsearch_range : %u \n"
        "\tentry_selector : %u \n"
        "\trange_shift : %u\n",
        dir->sfnt_version,
        dir->num_tables,
        dir->search_range,
        dir->entry_selector,
        dir->range_shift
    );

    for (Size s = 0; s < dir->num_tables; s++) {
        xf_otf_table_record_pprint (dir->table_records + s);
    }

    return dir;
}
