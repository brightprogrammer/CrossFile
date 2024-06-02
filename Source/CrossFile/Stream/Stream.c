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
#include <Anvie/CrossFile/Struct.h>

/* local includes */
#include "Stream.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/**
 * @b Close the given file stream.
 * */
void xf_data_stream_close (XfDataStream *stream) {
    RETURN_IF (!stream, ERR_INVALID_ARGUMENTS);
    RETURN_IF (
        !stream->callbacks.close,
        "Given stream does not have a close method. This might indicate a bug in the application\n"
    );

    stream->callbacks.close (stream);
}

#define GEN_FN(ret_type, type)                                                                     \
    ret_type xf_data_stream_read_##type (XfDataStream *stream) {                                   \
        RETURN_VALUE_IF (!stream, ((ret_type)0), ERR_INVALID_ARGUMENTS);                           \
        RETURN_VALUE_IF (                                                                          \
            !stream->callbacks.read_##type,                                                        \
            ((ret_type)0),                                                                         \
            "Given stream does not have a read_" #type                                             \
            " method. This might indicate a bug in the application\n"                              \
        );                                                                                         \
                                                                                                   \
        return stream->callbacks.read_##type (stream);                                             \
    }


GEN_FN (Uint8, u8);
GEN_FN (Uint16, u16);
GEN_FN (Uint32, u32);
GEN_FN (Uint64, u64);

GEN_FN (Int8, i8);
GEN_FN (Int16, i16);
GEN_FN (Int32, i32);
GEN_FN (Int64, i64);

#undef GEN_FN

#define GEN_FN(arr_type, type)                                                                     \
    arr_type xf_data_stream_read_##type (XfDataStream *stream, arr_type buf, Size buf_size) {      \
        RETURN_VALUE_IF (!stream || buf || !buf_size, ((arr_type)0), ERR_INVALID_ARGUMENTS);       \
        RETURN_VALUE_IF (                                                                          \
            !stream->callbacks.read_##type,                                                        \
            ((arr_type)0),                                                                         \
            "Given stream does not have a read_" #type                                             \
            " method. This might indicate a bug in the application\n"                              \
        );                                                                                         \
                                                                                                   \
        return stream->callbacks.read_##type (stream, buf, buf_size);                              \
    }

GEN_FN (CString, cstring);

GEN_FN (Uint8 *, u8_arr);
GEN_FN (Uint16 *, u16_arr);
GEN_FN (Uint32 *, u32_arr);
GEN_FN (Uint64 *, u64_arr);

GEN_FN (Int8 *, i8_arr);
GEN_FN (Int16 *, i16_arr);
GEN_FN (Int32 *, i32_arr);
GEN_FN (Int64 *, i64_arr);

#undef GEN_FN

XfByteOrder xf_data_stream_get_byte_order (XfDataStream *stream) {
    RETURN_VALUE_IF (!stream, XF_BYTE_ORDER_UNKNOWN, ERR_INVALID_ARGUMENTS);

    return stream->byte_order;
}
