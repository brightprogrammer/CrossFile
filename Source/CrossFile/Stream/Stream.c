/**
 * @file Stream.c
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

/* local includes */
#include "Stream.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * @b Close the given file stream.
 *
 * @param stream
 * */
PUBLIC void xf_data_stream_close (XfDataStream* stream) {
    RETURN_IF (!stream, ERR_INVALID_ARGUMENTS);
    RETURN_IF (
        !stream->callbacks.close,
        "Given stream does not have a close method. This might indicate a bug in the application\n"
    );

    stream->callbacks.close (stream);
}

/**
 * @b Seek to offset from starting of stream. 
 *
 * @param stream
 * @param nb Number of bytes to seek. Parity of value decides direction of seek.
 *
 * @return @c stream on success.
 * @return @c Null otherwise.
 * */
PUBLIC XfDataStream* xf_data_stream_seek (XfDataStream* stream, Int64 nb) {
    RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        !stream->callbacks.seek,
        Null,
        "Given stream does not have a 'set_seek' method. This might indicate a bug in the "
        "application\n"
    );

    return stream->callbacks.seek (stream, nb);
}

/**
 * @b Get seek offset from starting of stream. 
 *
 * @param stream
 *
 * @return Non-negative value on success.
 * @return -1 otherwise.
 * */
PUBLIC Int64 xf_data_stream_get_cursor (XfDataStream* stream) {
    RETURN_VALUE_IF (!stream, -1, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        !stream->callbacks.get_cursor,
        -1,
        "Given stream does not have a 'get_cursor' method. This might indicate a bug in the "
        "application\n"
    );

    return stream->callbacks.get_cursor (stream);
}

/**
 * @b Get total size of stream. 
 *
 * @param stream
 *
 * @return Non-zero value on success.
 * @return 0 otherwise.
 * */
PUBLIC Int64 xf_data_stream_get_size (XfDataStream* stream) {
    RETURN_VALUE_IF (!stream, 0, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        !stream->callbacks.get_size,
        0,
        "Given stream does not have a 'get_size' method. This might indicate a bug in the "
        "application\n"
    );

    return stream->callbacks.get_size (stream);
}

/**
 * @b Inform internal stream implementation to reserve this many bytes.
 * This is recommended in case the size of upcoming stream is knwon.
 *
 * @param stream
 * @param nb Number of bytes to reserve.
 *
 * @return @c stream on success.
 * @return @c Null otherwise.
 * */
PUBLIC XfDataStream* xf_data_stream_reserve (XfDataStream* stream, Size nb) {
    RETURN_VALUE_IF (!stream, 0, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        !stream->callbacks.reserve,
        0,
        "Given stream does not have a 'reserve' method. This might indicate a bug in the "
        "application\n"
    );

    return stream->callbacks.reserve (stream, nb);
}

/**
 * @b Number of bytes left to read.
 *
 * @param stream
 *
 * @return Non-negative value on success.
 * @return -1 otherwise.
 * */
PUBLIC Int64 xf_data_stream_get_remaining_size (XfDataStream* stream) {
    RETURN_VALUE_IF (!stream, -1, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        !stream->callbacks.get_remaining_size,
        0,
        "Given stream does not have a 'get_remaining_size' method. This might indicate a bug in "
        "the application\n"
    );

    return stream->callbacks.get_remaining_size (stream);
}

/* helper macros */
#define GEN_FN(n)                                                                                  \
    PUBLIC XfDataStream* xf_data_stream_read_t##n (XfDataStream* stream, Uint##n* v) {             \
        RETURN_VALUE_IF (!stream || !v, Null, ERR_INVALID_ARGUMENTS);                              \
        RETURN_VALUE_IF (                                                                          \
            !stream->callbacks.read_t##n,                                                          \
            Null,                                                                                  \
            "Given stream does not have a read_t" #n                                               \
            " method. This might indicate a bug in the application.\n"                             \
        );                                                                                         \
                                                                                                   \
        Uint##n x = 0;                                                                             \
        RETURN_VALUE_IF (                                                                          \
            !stream->callbacks.read_t##n (stream, &x),                                             \
            Null,                                                                                  \
            "Failed to read " #n "-bit value from given stream.\n"                                 \
        );                                                                                         \
                                                                                                   \
        *v = x;                                                                                    \
                                                                                                   \
        return stream;                                                                             \
    }


GEN_FN (8);
GEN_FN (16);
GEN_FN (32);
GEN_FN (64);

#undef GEN_FN
