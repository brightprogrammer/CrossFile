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

typedef XfDataStream* (*DataStreamReadT8) (XfDataStream* stream, Uint8* u8);
typedef XfDataStream* (*DataStreamReadT16) (XfDataStream* stream, Uint16* u16);
typedef XfDataStream* (*DataStreamReadT32) (XfDataStream* stream, Uint32* u32);
typedef XfDataStream* (*DataStreamReadT64) (XfDataStream* stream, Uint64* u64);
typedef XfDataStream* (*DataStreamSeek) (XfDataStream* stream, Int64 off);
typedef Int64 (*DataStreamGetCursor) (XfDataStream* stream);
typedef Size (*DataStreamGetSize) (XfDataStream* stream);
typedef XfDataStream* (*DataStreamReserve) (XfDataStream* stream, Size nb);
typedef Int64 (*DataStreamGetRemainingSize) (XfDataStream* stream);

typedef struct XfDataStreamCallbacks {
    DataStreamClose close;

    DataStreamReadT8  read_t8;
    DataStreamReadT16 read_t16;
    DataStreamReadT32 read_t32;
    DataStreamReadT64 read_t64;

    DataStreamSeek             seek;
    DataStreamGetCursor        get_cursor;
    DataStreamGetSize          get_size;
    DataStreamReserve          reserve;
    DataStreamGetRemainingSize get_remaining_size;
} XfDataStreamCallbacks;

struct XfDataStream {
    XfDataStreamCallbacks callbacks;
};

/* just some syntactic sugar to explicitly state the implicit inheritance */
#define INHERITS_DATA_STREAM() XfDataStream data_stream

#define DATA_STREAM(ptr) ((XfDataStream*)(ptr))

#endif // ANVIE_CROSSFILE_SOURCE_STREAM_STREAM_H
