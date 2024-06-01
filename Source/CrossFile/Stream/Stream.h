/**
 * @file Stream.h
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
 *
 * @b Definition of opaque data stream object.
 * */

#ifndef ANVIE_CROSSFILE_SOURCE_STREAM_STREAM_H
#define ANVIE_CROSSFILE_SOURCE_STREAM_STREAM_H

#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Stream.h>

typedef void (*DataStreamClose) (XfDataStream* stream);

typedef Uint8 (*DataStreamReadU8) (XfDataStream* stream);
typedef Uint16 (*DataStreamReadU16) (XfDataStream* stream);
typedef Uint32 (*DataStreamReadU32) (XfDataStream* stream);
typedef Uint64 (*DataStreamReadU64) (XfDataStream* stream);

typedef Int8 (*DataStreamReadI8) (XfDataStream* stream);
typedef Int16 (*DataStreamReadI16) (XfDataStream* stream);
typedef Int32 (*DataStreamReadI32) (XfDataStream* stream);
typedef Int64 (*DataStreamReadI64) (XfDataStream* stream);

typedef CString (*DataStreamReadCString) (XfDataStream* stream, CString buf, Size buf_size);

typedef Uint8* (*DataStreamReadU8Arr) (XfDataStream* stream, Uint8* buf, Size buf_size);
typedef Uint16* (*DataStreamReadU16Arr) (XfDataStream* stream, Uint16* buf, Size buf_size);
typedef Uint32* (*DataStreamReadU32Arr) (XfDataStream* stream, Uint32* buf, Size buf_size);
typedef Uint64* (*DataStreamReadU64Arr) (XfDataStream* stream, Uint64* buf, Size buf_size);

typedef Int8* (*DataStreamReadI8Arr) (XfDataStream* stream, Int8* buf, Size buf_size);
typedef Int16* (*DataStreamReadI16Arr) (XfDataStream* stream, Int16* buf, Size buf_size);
typedef Int32* (*DataStreamReadI32Arr) (XfDataStream* stream, Int32* buf, Size buf_size);
typedef Int64* (*DataStreamReadI64Arr) (XfDataStream* stream, Int64* buf, Size buf_size);

typedef struct XfDataStreamCallbacks {
    DataStreamClose close;

    DataStreamReadU8  read_u8;
    DataStreamReadU16 read_u16;
    DataStreamReadU32 read_u32;
    DataStreamReadU64 read_u64;

    DataStreamReadI8  read_i8;
    DataStreamReadI16 read_i16;
    DataStreamReadI32 read_i32;
    DataStreamReadI64 read_i64;

    DataStreamReadCString read_cstring;

    DataStreamReadU8Arr  read_u8_arr;
    DataStreamReadU16Arr read_u16_arr;
    DataStreamReadU32Arr read_u32_arr;
    DataStreamReadU64Arr read_u64_arr;

    DataStreamReadI8Arr  read_i8_arr;
    DataStreamReadI16Arr read_i16_arr;
    DataStreamReadI32Arr read_i32_arr;
    DataStreamReadI64Arr read_i64_arr;
} XfDataStreamCallbacks;

struct XfDataStream {
    XfByteOrder           byte_order;
    XfDataStreamCallbacks callbacks;
};

#define DATA_STREAM(ptr) ((XfDataStream*)(ptr))

#endif // ANVIE_CROSSFILE_SOURCE_STREAM_STREAM_H
