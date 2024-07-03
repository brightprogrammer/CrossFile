/**
 * @file File.c
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

typedef struct XfFileStream {
    INHERITS_DATA_STREAM();

    Uint8* file_data;
    Size   file_size;
    Size   file_cursor;

    CString file_name;
    FILE*   file_handle;
} XfFileStream;

#define fstream(ptr) ((XfFileStream*)(ptr))

PRIVATE void          fstream_close (XfFileStream* stream);
PRIVATE XfFileStream* fstream_read_t8 (XfFileStream* stream, Uint8* u8);
PRIVATE XfFileStream* fstream_read_t16 (XfFileStream* stream, Uint16* u16);
PRIVATE XfFileStream* fstream_read_t32 (XfFileStream* stream, Uint32* u32);
PRIVATE XfFileStream* fstream_read_t64 (XfFileStream* stream, Uint64* u64);
PRIVATE XfFileStream* fstream_seek (XfFileStream* stream, Int64 off);
PRIVATE Int64         fstream_get_cursor (XfFileStream* stream);
PRIVATE Int64         fstream_get_size (XfFileStream* stream);
PRIVATE Int64         fstream_get_remaining_size (XfFileStream* stream);

/**
 * @b Open a file stream by loading given file and assuming file has given byte-order.
 *
 * @param filename Name of file to be loaded.
 * @param byte_order Byte Order or endianness of the file data stream.
 *
 * @return Reference to opened @c XfDataStream on success.
 * @return @ Null otherwise.
 * */
PUBLIC XfDataStream* xf_data_stream_open_file (CString filename) {
    RETURN_VALUE_IF (!filename, Null, ERR_INVALID_ARGUMENTS);

    XfFileStream* fstream = NEW (XfFileStream);
    RETURN_VALUE_IF (!fstream, Null, ERR_OUT_OF_MEMORY);

    GOTO_HANDLER_IF (
        !(fstream->file_handle = fopen (filename, "rb")),
        INIT_FAILED,
        ERR_FILE_OPEN_FAILED
    );

    /* get file size */
    fseek (fstream->file_handle, 0, SEEK_END);
    fstream->file_size = ftell (fstream->file_handle);
    fseek (fstream->file_handle, 0, SEEK_SET);
    GOTO_HANDLER_IF (!fstream->file_size, INIT_FAILED, "File size is zero. Cannot read file\n");

    /* create space to read file */
    GOTO_HANDLER_IF (
        !(fstream->file_data = ALLOCATE (Uint8, fstream->file_size)),
        INIT_FAILED,
        ERR_OUT_OF_MEMORY
    );

    /* read complete file */
    GOTO_HANDLER_IF (
        (fread (fstream->file_data, 1, fstream->file_size, fstream->file_handle) !=
         fstream->file_size),
        INIT_FAILED,
        "Failed to read complete file : %s",
        strerror (errno)
    );

    /* copy filename */
    GOTO_HANDLER_IF (!(fstream->file_name = strdup (filename)), INIT_FAILED, ERR_OUT_OF_MEMORY);

    XfDataStream* data_stream = DATA_STREAM (fstream);

    data_stream->callbacks.close      = (DataStreamClose)fstream_close;
    data_stream->callbacks.read_t8    = (DataStreamReadT8)fstream_read_t8;
    data_stream->callbacks.read_t16   = (DataStreamReadT16)fstream_read_t16;
    data_stream->callbacks.read_t32   = (DataStreamReadT32)fstream_read_t32;
    data_stream->callbacks.read_t64   = (DataStreamReadT64)fstream_read_t64;
    data_stream->callbacks.seek       = (DataStreamSeek)fstream_seek;
    data_stream->callbacks.get_cursor = (DataStreamGetCursor)fstream_get_cursor;
    data_stream->callbacks.get_size   = (DataStreamGetSize)fstream_get_size;
    data_stream->callbacks.get_remaining_size =
        (DataStreamGetRemainingSize)fstream_get_remaining_size;

    /* NOTE: File stream does not implement a reserve method. Not required as whole file is loaded at once. */
    data_stream->callbacks.reserve = (DataStreamReserve)Null;


    return data_stream;

INIT_FAILED:
    fstream_close (fstream);
    return Null;
}

/**
 * @b File stream close implementation.
 *
 * @param fstream.
 * */
PRIVATE void fstream_close (XfFileStream* fstream) {
    RETURN_IF (!fstream, ERR_INVALID_ARGUMENTS);

    if (fstream->file_data) {
        FREE (fstream->file_data);
    }

    if (fstream->file_handle) {
        fclose (fstream->file_handle);
    }

    if (fstream->file_name) {
        FREE (fstream->file_name);
    }

    memset (fstream, 0, sizeof (XfFileStream));
    FREE (fstream);
}

/**
 * @b File Stream seek implementation.
 * 
 * @param fstream.
 * @param off.
 *
 * @return @c fstream on success.
 * @return @c Null otherwise.
 * */
PRIVATE XfFileStream* fstream_seek (XfFileStream* fstream, Int64 off) {
    RETURN_VALUE_IF (!fstream, Null, ERR_INVALID_ARGUMENTS);

    if (off > 0) {
        Int64 rem_size;
        RETURN_VALUE_IF (
            (rem_size = fstream_get_remaining_size (fstream)) == -1,
            Null,
            "Failed to get remaining size of stream.\n"
        );

        RETURN_VALUE_IF (
            rem_size < off,
            Null,
            "Seek offset exceeds current remaining stream size.\n"
        );
    } else if (off < 0) {
        Int64 cursor;
        RETURN_VALUE_IF (
            (cursor = fstream_get_cursor (fstream)) == -1,
            Null,
            "Failed to get remaining size of stream.\n"
        );

        RETURN_VALUE_IF (
            (cursor + off) < 0,
            Null,
            "Seek offset in reverse direction exceeds stream size.\n"
        );
    }

    fstream->file_cursor += off;
    return fstream;
}

/**
 * @b File Stream get_cursor implementation.
 * Get's the current read position in file stream.
 * 
 * @param fstream.
 *
 * @return Non-negative value on success.
 * @return -1 otherwise.
 * */
PRIVATE Int64 fstream_get_cursor (XfFileStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);
    return fstream->file_cursor;
}

/**
 * @b File Stream get_size implementation.
 * Get's the total size of file loaded by file stream.
 * 
 * @param fstream.
 *
 * @return Non-negative value on success.
 * @return -1 otherwise. 0 might mean invalid size as well in some cases.
 * */
PRIVATE Int64 fstream_get_size (XfFileStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);
    return fstream->file_size;
}

PRIVATE Int64 fstream_get_remaining_size (XfFileStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);

    Int64 cursor;
    RETURN_VALUE_IF (
        (cursor = fstream_get_cursor (fstream)) == -1,
        -1,
        "Failed to get cursor from stream.\n"
    );

    Int64 size;
    RETURN_VALUE_IF (
        (size = fstream_get_size (fstream)) == -1,
        -1,
        "Failed to get size of stream.\n"
    );

    return size - cursor;
}

#define GEN_FN(N)                                                                                  \
    PRIVATE XfFileStream* fstream_read_t##N (XfFileStream* fstream, Uint##N* v) {                  \
        RETURN_VALUE_IF (!fstream || !v, Null, ERR_INVALID_ARGUMENTS);                             \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            fstream_get_remaining_size (fstream) < (N >> 3),                                       \
            Null,                                                                                  \
            "Not enough data left in data stream.\n"                                               \
        );                                                                                         \
                                                                                                   \
        Uint##N x = ((Uint##N*)(fstream->file_data + fstream->file_cursor))[0];                    \
        RETURN_VALUE_IF (                                                                          \
            !fstream_seek (fstream, (N >> 3)),                                                     \
            Null,                                                                                  \
            "Failed to seek ahead after reading.\n"                                                \
        );                                                                                         \
        *v = x;                                                                                    \
                                                                                                   \
        return fstream;                                                                            \
    }

GEN_FN (8)
GEN_FN (16)
GEN_FN (32)
GEN_FN (64)

#undef GEN_FN
