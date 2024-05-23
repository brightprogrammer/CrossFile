/**
 * @file File.c
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

/* libc */
#include <errno.h>
#include <string.h>

/* crossfile */
#include <Anvie/CrossFile/File.h>

/**
 * @b Open file and read it's contents in binary format.
 * This method is analogous to init method for other structs.
 *
 * @param file @c XfFile struct where read data will be stored.
 * @param filename Name/Path of file to be read.
 *
 * @return @c file on success.
 * @return @c Null otherwise.
 * */
XfFile* xf_file_open (XfFile* file, CString filename) {
    RETURN_VALUE_IF (!file || !filename, Null, ERR_INVALID_ARGUMENTS);

    /* open file */
    FILE* fhandle = fopen (filename, "rb");
    RETURN_VALUE_IF (!fhandle, Null, "Failed to open file : %s\n", strerror (errno));

    /* get file size */
    fseek (fhandle, 0, SEEK_END);
    Size file_size = ftell (fhandle);
    fseek (fhandle, 0, SEEK_SET);
    GOTO_HANDLER_IF (!file_size, INIT_FAILED, "File size is zero. Cannot read file\n");

    /* create space to read file */
    Uint8* file_data = ALLOCATE (Uint8, file_size);
    GOTO_HANDLER_IF (!file_data, INIT_FAILED, ERR_OUT_OF_MEMORY);

    /* read complete file */
    GOTO_HANDLER_IF (
        fread (file_data, 1, file_size, fhandle) != file_size,
        INIT_FAILED,
        "Failed to read complete file : %s",
        strerror (errno)
    );

    /* copy filename */
    file->name = strdup (filename);
    GOTO_HANDLER_IF (!file->name, INIT_FAILED, ERR_OUT_OF_MEMORY);

    file->data   = file_data;
    file->size   = file_size;
    file->handle = fhandle;

    return file;

INIT_FAILED:
    xf_file_close (file);
    return Null;
}

/**
 * @b Close any opened file and free all resources.
 * This will not destroy given struct. This method is analogous to
 * deinit method for other structs.
 * 
 * @param file
 *
 * @return @c file on success.
 * @return @c Null otherwise.
 * */
XfFile* xf_file_close (XfFile* file) {
    RETURN_VALUE_IF (!file, Null, ERR_INVALID_ARGUMENTS);

    if (file->name) {
        FREE (file->name);
    }

    if (file->data) {
        FREE (file->data);
    }

    if (file->handle) {
        fclose (file->handle);
    }

    memset (file, 0, sizeof (XfFile));

    return file;
}
