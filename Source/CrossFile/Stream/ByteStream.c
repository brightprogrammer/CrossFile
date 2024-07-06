/**
 * @file ByteStream.c
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
 * @brief ByteStream is an immutable stream that exists only to wrap byte arrays into streams temporarily.
 * This is useful when data is coming in form a byte array. The array is owned by the creator of byte stream.
 * Since byte stream is immutable, when the array is resized, one needs to destroy previous stream and create
 * a new one with new resized array and new size, and this is a lightweight operation because byte stream does
 * not allocate anything other than the object itself!
 * */

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/Stream.h>

/* libc */
#include <memory.h>

/* local includes */
#include "Stream.h"

typedef struct XfByteStream {
    INHERITS_IO_STREAM();
} XfByteStream;

/**
 * @b Close byte stream.
 *
 * @param stream
 * */
PRIVATE void bstream_close (XfByteStream* stream) {
    RETURN_IF (!stream, ERR_INVALID_ARGUMENTS);

    /* since we don't own any pointer other than object itself, we won't free them. */
    memset (stream, 0, sizeof (XfByteStream));
    FREE (stream);
}

/**
 * @b Open a raw data stream by loading using given data array of given size 
 *
 * @param data Raw data to be treated as data stream.
 * @param data_size 
 *
 * @return Reference to opened @c IoStream on success.
 * @return @ Null otherwise.
 * */
IoStream* io_stream_open_byte_seq (PUint8 data, Size data_size) {
    RETURN_VALUE_IF (!data || !data_size, Null, ERR_INVALID_ARGUMENTS);

    XfByteStream* bstream = NEW (XfByteStream);
    RETURN_VALUE_IF (!bstream, Null, ERR_OUT_OF_MEMORY);

    bstream->stream.data     = data;
    bstream->stream.size     = data_size;
    bstream->stream.capacity = data_size;

    IoStream* data_stream   = IO_STREAM (bstream);
    data_stream->is_mutable = False;
    data_stream->close      = (IoStreamCloseClbk)bstream_close;

    return data_stream;
}
