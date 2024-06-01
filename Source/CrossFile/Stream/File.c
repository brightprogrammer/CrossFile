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
#include <memory.h>

/* local includes */
#include "Stream.h"

typedef struct XfFileStream {
    XfDataStream data_stream;
    Size         file_size;
    Uint8*       file_data;
    CString      file_name;
    FILE*        file_handle;
    Size         file_cursor;
} XfFileStream;

#define FILE_STREAM(ptr) ((XfFileStream*)(ptr))

/**************************************************************************************************/
/**************************** FILE STREAM PRIVATE METHOD DECLARATIONS *****************************/
/**************************************************************************************************/

static void    file_stream_close (XfFileStream* stream);
static Uint8   file_stream_read_u8 (XfFileStream* stream);
static Uint16  file_stream_read_u16 (XfFileStream* stream);
static Uint32  file_stream_read_u32 (XfFileStream* stream);
static Uint64  file_stream_read_u64 (XfFileStream* stream);
static Int8    file_stream_read_i8 (XfFileStream* stream);
static Int16   file_stream_read_i16 (XfFileStream* stream);
static Int32   file_stream_read_i32 (XfFileStream* stream);
static Int64   file_stream_read_i64 (XfFileStream* stream);
static CString file_stream_read_cstring (XfFileStream* stream, CString buf, Size buf_size);
static Uint8*  file_stream_read_u8_arr (XfFileStream* stream, Uint8* buf, Size buf_size);
static Uint16* file_stream_read_u16_arr (XfFileStream* stream, Uint16* buf, Size buf_size);
static Uint32* file_stream_read_u32_arr (XfFileStream* stream, Uint32* buf, Size buf_size);
static Uint64* file_stream_read_u64_arr (XfFileStream* stream, Uint64* buf, Size buf_size);
static Int8*   file_stream_read_i8_arr (XfFileStream* stream, Int8* buf, Size buf_size);
static Int16*  file_stream_read_i16_arr (XfFileStream* stream, Int16* buf, Size buf_size);
static Int32*  file_stream_read_i32_arr (XfFileStream* stream, Int32* buf, Size buf_size);
static Int64*  file_stream_read_i64_arr (XfFileStream* stream, Int64* buf, Size buf_size);

/**************************************************************************************************/
/***************************** FILE STREAM PUBLIC METHOD DEFINITIONS ******************************/
/**************************************************************************************************/

XfDataStream* xf_data_stream_open_file (CString filename, XfByteOrder byte_order) {
    RETURN_VALUE_IF (!filename || !byte_order, Null, ERR_INVALID_ARGUMENTS);

    XfFileStream* file_stream = NEW (XfFileStream);
    RETURN_VALUE_IF (!file_stream, Null, ERR_OUT_OF_MEMORY);

    GOTO_HANDLER_IF (
        !(file_stream->file_handle = fopen (filename, "rb")),
        INIT_FAILED,
        ERR_FILE_OPEN_FAILED
    );

    file_stream->data_stream.callbacks.close    = (DataStreamClose)file_stream_close;
    file_stream->data_stream.callbacks.read_u8  = (DataStreamReadU8)file_stream_read_u8;
    file_stream->data_stream.callbacks.read_u16 = (DataStreamReadU16)file_stream_read_u16;
    file_stream->data_stream.callbacks.read_u32 = (DataStreamReadU32)file_stream_read_u32;
    file_stream->data_stream.callbacks.read_u64 = (DataStreamReadU64)file_stream_read_u64;
    file_stream->data_stream.callbacks.read_i8  = (DataStreamReadI8)file_stream_read_i8;
    file_stream->data_stream.callbacks.read_i16 = (DataStreamReadI16)file_stream_read_i16;
    file_stream->data_stream.callbacks.read_i32 = (DataStreamReadI32)file_stream_read_i32;
    file_stream->data_stream.callbacks.read_i64 = (DataStreamReadI64)file_stream_read_i64;
    file_stream->data_stream.callbacks.read_cstring =
        (DataStreamReadCString)file_stream_read_cstring;
    file_stream->data_stream.callbacks.read_u8_arr = (DataStreamReadU8Arr)file_stream_read_u8_arr;
    file_stream->data_stream.callbacks.read_u16_arr =
        (DataStreamReadU16Arr)file_stream_read_u16_arr;
    file_stream->data_stream.callbacks.read_u32_arr =
        (DataStreamReadU32Arr)file_stream_read_u32_arr;
    file_stream->data_stream.callbacks.read_u64_arr =
        (DataStreamReadU64Arr)file_stream_read_u64_arr;
    file_stream->data_stream.callbacks.read_i8_arr = (DataStreamReadI8Arr)file_stream_read_i8_arr;
    file_stream->data_stream.callbacks.read_i16_arr =
        (DataStreamReadI16Arr)file_stream_read_i16_arr;
    file_stream->data_stream.callbacks.read_i32_arr =
        (DataStreamReadI32Arr)file_stream_read_i32_arr;
    file_stream->data_stream.callbacks.read_i64_arr =
        (DataStreamReadI64Arr)file_stream_read_i64_arr;

    return DATA_STREAM (file_stream);

INIT_FAILED:
    file_stream_close (file_stream);
    return Null;
}

/**************************************************************************************************/
/***************************** FILE STREAM PRIVATE METHOD DEFINITIONS *****************************/
/**************************************************************************************************/

static void file_stream_close (XfFileStream* fstream) {
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


#define GEN_FN(type, suffix)                                                                       \
    static type file_stream_read_##suffix (XfFileStream* fstream) {                                \
        RETURN_VALUE_IF (!fstream, 0, ERR_INVALID_ARGUMENTS);                                      \
        RETURN_VALUE_IF (                                                                          \
            fstream->file_cursor + sizeof (type) >= fstream->file_size,                            \
            0,                                                                                     \
            "Not sufficient data in stream to be read %zu bytes, available %zu\n",                 \
            sizeof (type),                                                                         \
            fstream->file_size >= fstream->file_cursor ?                                           \
                fstream->file_size - fstream->file_cursor :                                        \
                0                                                                                  \
        );                                                                                         \
                                                                                                   \
        type data             = *((type*)(fstream->file_data + fstream->file_cursor));             \
        fstream->file_cursor += sizeof (type);                                                     \
        return data;                                                                               \
    }

/* TODO: this needs to take care of endianness as well. */
GEN_FN (Uint8, u8)
GEN_FN (Uint16, u16)
GEN_FN (Uint32, u32)
GEN_FN (Uint64, u64)

#undef GEN_FN

static Uint32  file_stream_read_u32 (XfFileStream* fstream);
static Uint64  file_stream_read_u64 (XfFileStream* fstream);
static Int8    file_stream_read_i8 (XfFileStream* fstream);
static Int16   file_stream_read_i16 (XfFileStream* fstream);
static Int32   file_stream_read_i32 (XfFileStream* fstream);
static Int64   file_stream_read_i64 (XfFileStream* fstream);
static CString file_stream_read_cstring (XfFileStream* fstream, CString buf, Size buf_size);
static Uint8*  file_stream_read_u8_arr (XfFileStream* fstream, Uint8* buf, Size buf_size);
static Uint16* file_stream_read_u16_arr (XfFileStream* fstream, Uint16* buf, Size buf_size);
static Uint32* file_stream_read_u32_arr (XfFileStream* fstream, Uint32* buf, Size buf_size);
static Uint64* file_stream_read_u64_arr (XfFileStream* fstream, Uint64* buf, Size buf_size);
static Int8*   file_stream_read_i8_arr (XfFileStream* fstream, Int8* buf, Size buf_size);
static Int16*  file_stream_read_i16_arr (XfFileStream* fstream, Int16* buf, Size buf_size);
static Int32*  file_stream_read_i32_arr (XfFileStream* fstream, Int32* buf, Size buf_size);
static Int64*  file_stream_read_i64_arr (XfFileStream* fstream, Int64* buf, Size buf_size);
