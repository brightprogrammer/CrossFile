/**
 * @file RawData.c
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

typedef struct XfRawDataStream {
    XfDataStream data_stream;
    Uint8*       data;
    Size         data_size;
    Size         data_cursor;
} XfRawDataStream;

/**************************************************************************************************/
/************************** RAW DATA STREAM PRIVATE METHOD DECLARATIONS ***************************/
/**************************************************************************************************/

static void    raw_data_stream_close (XfRawDataStream* stream);
static Uint8   raw_data_stream_read_u8 (XfRawDataStream* stream);
static Uint16  raw_data_stream_read_u16 (XfRawDataStream* stream);
static Uint32  raw_data_stream_read_u32 (XfRawDataStream* stream);
static Uint64  raw_data_stream_read_u64 (XfRawDataStream* stream);
static Int8    raw_data_stream_read_i8 (XfRawDataStream* stream);
static Int16   raw_data_stream_read_i16 (XfRawDataStream* stream);
static Int32   raw_data_stream_read_i32 (XfRawDataStream* stream);
static Int64   raw_data_stream_read_i64 (XfRawDataStream* stream);
static CString raw_data_stream_read_cstring (XfRawDataStream* stream, CString buf, Size buf_size);
static Uint8*  raw_data_stream_read_u8_arr (XfRawDataStream* stream, Uint8* buf, Size buf_size);
static Uint16* raw_data_stream_read_u16_arr (XfRawDataStream* stream, Uint16* buf, Size buf_size);
static Uint32* raw_data_stream_read_u32_arr (XfRawDataStream* stream, Uint32* buf, Size buf_size);
static Uint64* raw_data_stream_read_u64_arr (XfRawDataStream* stream, Uint64* buf, Size buf_size);
static Int8*   raw_data_stream_read_i8_arr (XfRawDataStream* stream, Int8* buf, Size buf_size);
static Int16*  raw_data_stream_read_i16_arr (XfRawDataStream* stream, Int16* buf, Size buf_size);
static Int32*  raw_data_stream_read_i32_arr (XfRawDataStream* stream, Int32* buf, Size buf_size);
static Int64*  raw_data_stream_read_i64_arr (XfRawDataStream* stream, Int64* buf, Size buf_size);

/**************************************************************************************************/
/*************************** RAW DATA STREAM PUBLIC METHOD DEFINITIONS ****************************/
/**************************************************************************************************/

/**
 * @b Open a raw data stream by loading using given data array of given size 
 *  
 * No internal copy of given raw data will be made. This means the program must keep
 * the data array in memory as long as the raw data stream is not destroyed. Destroying
 * the given raw data memory reference is undefined-behavior.
 *
 * Raw data stream is most useful when one quickly wants to spin up a data stream when
 * data is already available and is volatile (like on stack).
 *
 * @param data Raw data to be treated as data stream.
 * @param data_size 
 * @param byte_order Byte Order or endianness of the file data stream.
 *
 * @return Reference to opened @c XfDataStream on success.
 * @return @ Null otherwise.
 * */
XfDataStream* xf_data_stream_open_raw_data (Uint8* data, Size data_size, XfByteOrder byte_order) {
    RETURN_VALUE_IF (!data || !data_size || !byte_order, Null, ERR_INVALID_ARGUMENTS);

    XfRawDataStream* rdstream = NEW (XfRawDataStream);
    RETURN_VALUE_IF (!rdstream, Null, ERR_OUT_OF_MEMORY);

    rdstream->data      = data;
    rdstream->data_size = data_size;

    XfDataStream* data_stream = DATA_STREAM (rdstream);
    data_stream->byte_order   = byte_order;

    data_stream->callbacks.close        = (DataStreamClose)raw_data_stream_close;
    data_stream->callbacks.read_u8      = (DataStreamReadU8)raw_data_stream_read_u8;
    data_stream->callbacks.read_u16     = (DataStreamReadU16)raw_data_stream_read_u16;
    data_stream->callbacks.read_u32     = (DataStreamReadU32)raw_data_stream_read_u32;
    data_stream->callbacks.read_u64     = (DataStreamReadU64)raw_data_stream_read_u64;
    data_stream->callbacks.read_i8      = (DataStreamReadI8)raw_data_stream_read_i8;
    data_stream->callbacks.read_i16     = (DataStreamReadI16)raw_data_stream_read_i16;
    data_stream->callbacks.read_i32     = (DataStreamReadI32)raw_data_stream_read_i32;
    data_stream->callbacks.read_i64     = (DataStreamReadI64)raw_data_stream_read_i64;
    data_stream->callbacks.read_cstring = (DataStreamReadCString)raw_data_stream_read_cstring;
    data_stream->callbacks.read_u8_arr  = (DataStreamReadU8Arr)raw_data_stream_read_u8_arr;
    data_stream->callbacks.read_u16_arr = (DataStreamReadU16Arr)raw_data_stream_read_u16_arr;
    data_stream->callbacks.read_u32_arr = (DataStreamReadU32Arr)raw_data_stream_read_u32_arr;
    data_stream->callbacks.read_u64_arr = (DataStreamReadU64Arr)raw_data_stream_read_u64_arr;
    data_stream->callbacks.read_i8_arr  = (DataStreamReadI8Arr)raw_data_stream_read_i8_arr;
    data_stream->callbacks.read_i16_arr = (DataStreamReadI16Arr)raw_data_stream_read_i16_arr;
    data_stream->callbacks.read_i32_arr = (DataStreamReadI32Arr)raw_data_stream_read_i32_arr;
    data_stream->callbacks.read_i64_arr = (DataStreamReadI64Arr)raw_data_stream_read_i64_arr;

    return data_stream;
}

/**************************************************************************************************/
/*************************** RAW DATA STREAM PRIVATE METHOD DEFINITIONS ***************************/
/**************************************************************************************************/

static void raw_data_stream_close (XfRawDataStream* stream) {
    RETURN_IF (!stream, ERR_INVALID_ARGUMENTS);

    memset (stream, 0, sizeof (XfRawDataStream));
}

#define GEN_FN(type, type_caps, suffix)                                                            \
    static type raw_data_stream_read_##suffix (XfRawDataStream* stream) {                          \
        RETURN_VALUE_IF (!stream, ((type)0), ERR_INVALID_ARGUMENTS);                               \
        RETURN_VALUE_IF (                                                                          \
            stream->data_size <= stream->data_cursor + sizeof (type),                              \
            ((type)0),                                                                             \
            "Not sufficient data in stream to read %zu bytes, available %zu\n",                    \
            sizeof (type),                                                                         \
            stream->data_size > stream->data_cursor ? stream->data_size - stream->data_cursor : 0  \
        );                                                                                         \
                                                                                                   \
        type value;                                                                                \
        if ((XF_HOST_BYTE_ORDER_IS_LSB && stream->data_stream.byte_order == XF_BYTE_ORDER_MSB) ||  \
            (XF_HOST_BYTE_ORDER_IS_MSB && stream->data_stream.byte_order == XF_BYTE_ORDER_LSB)) {  \
            value =                                                                                \
                INVERT_BYTE_ORDER_##type_caps (*((type*)(stream->data + stream->data_cursor)));    \
        } else {                                                                                   \
            value = (*((type*)(stream->data + stream->data_cursor)));                              \
        }                                                                                          \
        stream->data_cursor += sizeof (type);                                                      \
        return value;                                                                              \
    }

GEN_FN (Uint8, U8, u8);
GEN_FN (Uint16, U16, u16);
GEN_FN (Uint32, U32, u32);
GEN_FN (Uint64, U64, u64);

GEN_FN (Int8, I8, i8);
GEN_FN (Int16, I16, i16);
GEN_FN (Int32, I32, i32);
GEN_FN (Int64, I64, i64);

#undef GEN_FN

static CString raw_data_stream_read_cstring (XfRawDataStream* stream, CString buf, Size buf_size) {
    RETURN_VALUE_IF (!stream || !buf || !buf_size, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        stream->data_size <= stream->data_cursor + sizeof (Char) * buf_size,
        Null,
        "Not sufficient data in stream to read %zu bytes, available %zu\n",
        sizeof (Char),
        stream->data_size > stream->data_cursor ? stream->data_size - stream->data_cursor : 0
    );

    memcpy ((void*)buf, stream->data, buf_size);

    return buf;
}
#define GEN_FN(elem_type, type_caps, suffix)                                                       \
    static elem_type* raw_data_stream_read_##suffix (                                              \
        XfRawDataStream* stream,                                                                   \
        elem_type*       buf,                                                                      \
        Size             buf_size                                                                  \
    ) {                                                                                            \
        RETURN_VALUE_IF (!stream, ((elem_type*)0), ERR_INVALID_ARGUMENTS);                         \
        RETURN_VALUE_IF (                                                                          \
            stream->data_size <= stream->data_cursor + sizeof (elem_type) * buf_size,              \
            ((elem_type*)0),                                                                       \
            "Not sufficient data in stream to read %zu bytes, available %zu\n",                    \
            sizeof (elem_type),                                                                    \
            stream->data_size > stream->data_cursor ? stream->data_size - stream->data_cursor : 0  \
        );                                                                                         \
                                                                                                   \
        if ((XF_HOST_BYTE_ORDER_IS_LSB && stream->data_stream.byte_order == XF_BYTE_ORDER_MSB) ||  \
            (XF_HOST_BYTE_ORDER_IS_MSB && stream->data_stream.byte_order == XF_BYTE_ORDER_LSB)) {  \
            for (Size s = 0; s < buf_size; s++) {                                                  \
                buf[s] = INVERT_BYTE_ORDER_##type_caps (((elem_type*)(stream->data))[s]);          \
                stream->data_cursor += sizeof (elem_type);                                         \
            }                                                                                      \
        } else {                                                                                   \
            for (Size s = 0; s < buf_size; s++) {                                                  \
                buf[s]               = (*((elem_type*)(stream->data + stream->data_cursor)));      \
                stream->data_cursor += sizeof (elem_type);                                         \
            }                                                                                      \
        }                                                                                          \
        return buf;                                                                                \
    }

GEN_FN (Uint8, U8, u8_arr);
GEN_FN (Uint16, U16, u16_arr);
GEN_FN (Uint32, U32, u32_arr);
GEN_FN (Uint64, U64, u64_arr);

GEN_FN (Int8, I8, i8_arr);
GEN_FN (Int16, I16, i16_arr);
GEN_FN (Int32, I32, i32_arr);
GEN_FN (Int64, I64, i64_arr);

#undef GEN_FN
