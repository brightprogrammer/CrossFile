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

#ifndef ANVIE_CROSSFILE_SOURCE_IO_STREAM_H
#define ANVIE_CROSSFILE_SOURCE_IO_STREAM_H

#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Stream.h>

typedef void (*IoStreamCloseClbk) (IoStream* io);

struct IoStream {
    PUint8 data;     /**< @b Whenever the stream receives new data, it'll append to this buffer. */
    Size   size;     /**< @b Current set size of stream. */
    Size   capacity; /**< @b Reserved capacity of stream. This is max bytes it can hold. */
    Size   cursor;   /**< @b Points to the next byte to start reading/writing from. */

    Bool is_mutable; /**< @b Decides whether or not a stream is resizable. */

    /* all callbacks are optional (except close, it's recommended to provide that one) */

    IoStreamCloseClbk close;
};

/* just some syntactic sugar to explicitly state the implicit inheritance */
#define INHERITS_IO_STREAM() IoStream stream

#define IO_STREAM(ptr) ((IoStream*)(ptr))

/**
 * This is a trick employed to detect host endianness from it's definition.
 * REF : https://stackoverflow.com/a/2103095
 * */
typedef enum ByteOrder {
    BYTE_ORDER_UNKNOWN = 0,
    BYTE_ORDER_MSB     = 0x00010203, /**< @b Big Endian or most significant byte first. */
    BYTE_ORDER_LSB     = 0x03020100  /**< @b Little Endian or least significant byte first. */
} ByteOrder;

static const union {
    unsigned char bytes[4];
    ByteOrder     value;
} __host_order__trickster__ = {
    {0, 1, 2, 3}
};

#define HOST_BYTE_ORDER        (__host_order__trickster__.value)
#define HOST_BYTE_ORDER_IS_MSB (HOST_BYTE_ORDER == BYTE_ORDER_MSB)
#define HOST_BYTE_ORDER_IS_LSB (HOST_BYTE_ORDER == BYTE_ORDER_LSB)

#endif // ANVIE_CROSSFILE_SOURCE_IO_STREAM_H
