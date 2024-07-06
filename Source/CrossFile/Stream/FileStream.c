/**
 * @file FileIoStream.c
 * @date Fri, 1st June 2024
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
#include <Anvie/CrossFile/Stream.h>

/* libc */
#include <errno.h>
#include <memory.h>

/* local includes */
#include "Stream.h"

typedef struct FileIoStream {
    INHERITS_IO_STREAM();

    CString file_name;
    FILE*   file_handle;
} FileIoStream;

#define FILE_STREAM(ptr) ((FileIoStream*)(ptr))

/**
 * @b File stream close implementation.
 *
 * @param fio.
 * */
PRIVATE void fio_close (FileIoStream* fio) {
    RETURN_IF (!fio, ERR_INVALID_ARGUMENTS);

    if (fio->file_handle) {
        fclose (fio->file_handle);
    }

    if (fio->file_name) {
        memset ((Char*)fio->file_name, 0, strlen (fio->file_name));
        FREE (fio->file_name);
    }

    memset (fio, 0, sizeof (FileIoStream));
    FREE (fio);
}

/**
 * @b Open a file stream.
 *
 * @param filename Name of file to be loaded.
 *
 * @return Reference to opened @c IoStream on success.
 * @return @ Null otherwise.
 * */
PUBLIC IoStream* io_stream_open_file (CString filename, Bool is_writable) {
    RETURN_VALUE_IF (!filename, Null, ERR_INVALID_ARGUMENTS);

    FileIoStream* fio = NEW (FileIoStream);
    RETURN_VALUE_IF (!fio, Null, ERR_OUT_OF_MEMORY);

    /* set callbacks, and make the stream mutable */
    IoStream* io   = IO_STREAM (fio);
    io->is_mutable = True;
    io->close      = (IoStreamCloseClbk)fio_close;

    GOTO_HANDLER_IF (
        !(fio->file_handle = fopen (filename, is_writable ? "r+b" : "rb")),
        FIO_OPEN_FAILED,
        "Failed to open file stream : %s\n",
        strerror (errno)
    );

    /* get file size */
    fseek (fio->file_handle, 0, SEEK_END);
    Size file_size = ftell (fio->file_handle);
    fseek (fio->file_handle, 0, SEEK_SET);
    GOTO_HANDLER_IF (!file_size, FIO_OPEN_FAILED, "File size is zero on disk. Cannot read file\n");

    /* create space to read file */
    GOTO_HANDLER_IF (
        !io_stream_reserve (IO_STREAM (fio), file_size),
        FIO_OPEN_FAILED,
        "Failed to reserve memory for reading file data.\n"
    );

    /* read complete file */
    GOTO_HANDLER_IF (
        ((fio->stream.size = fread (fio->stream.data, 1, file_size, fio->file_handle)) != file_size
        ),
        FIO_OPEN_FAILED,
        "Failed to read complete file : %s",
        strerror (errno)
    );

    /* copy filename */
    GOTO_HANDLER_IF (!(fio->file_name = strdup (filename)), FIO_OPEN_FAILED, ERR_OUT_OF_MEMORY);

    /* stream is now mutable only if it's writeable */
    io->is_mutable = is_writable;

    return io;

FIO_OPEN_FAILED:
    fio_close (fio);
    return Null;
}
