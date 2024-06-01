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
 * @brief @c XfStructDesc can be used to describe structured data streams and how to read them.
 * It also provides a few methods to pretty print these structs. Basic types are already defined
 * and can be used directly but complex types needs to be defined by the user.
 *
 * @c XfStruct can recursively define a structured format. Recursive here means a struct containing
 * a struct that contains another struct and so on as many times as needed. Each struct contains a set
 * of field descriptors and the field descriptor contains information such as where to write the data,
 * how to read the data (endianness), at what offset to read the data, whether data is an array of an
 * element type or it's a single element. Dynamic arrays are also possible.
 * */

#ifndef ANVIE_CROSSFILE_STREAM_H
#define ANVIE_CROSSFILE_STREAM_H

#include <Anvie/Types.h>

/* fwd declarations */
typedef struct XfStructDesc XfStructDesc;

/**
 * @b A data stream is an opaque abstraction over the data input source.
 *
 * The input can be anything from files to sockets to interprocess communication.
 * As long as an internal stream implementation exists, and the API is followed,
 * users get a uniform interface to interact and read with any data stream.
 * */
typedef struct XfDataStream XfDataStream;

/**
 * This is a trick employed to detect host endianness from it's definition.
 * REF : https://stackoverflow.com/a/2103095
 * */
typedef enum XfByteOrder {
    XF_BYTE_ORDER_UNKNOWN = 0,
    XF_BYTE_ORDER_MSB     = 0x00010203, /**< @b Big Endian or most significant byte first. */
    XF_BYTE_ORDER_LSB     = 0x03020100  /**< @b Little Endian or least significant byte first. */
} XfByteOrder;

static const union {
    unsigned char bytes[4];
    XfByteOrder   value;
} __host_order__trickster__ = {
    {0, 1, 2, 3}
};

#define XF_HOST_BYTE_ORDER        (__host_order__trickster__.value)
#define XF_HOST_BYTE_ORDER_IS_MSB (XF_HOST_BYTE_ORDER == XF_BYTE_ORDER_MSB)
#define XF_HOST_BYTE_ORDER_IS_LSB (XF_HOST_BYTE_ORDER == XF_BYTE_ORDER_LSB)

XfDataStream* xf_data_stream_open_file (CString filename, XfByteOrder order);
void          xf_data_stream_close (XfDataStream* stream);

Uint8  xf_data_stream_read_u8 (XfDataStream* stream);
Uint16 xf_data_stream_read_u16 (XfDataStream* stream);
Uint32 xf_data_stream_read_u32 (XfDataStream* stream);
Uint64 xf_data_stream_read_u64 (XfDataStream* stream);

Int8  xf_data_stream_read_i8 (XfDataStream* stream);
Int16 xf_data_stream_read_i16 (XfDataStream* stream);
Int32 xf_data_stream_read_i32 (XfDataStream* stream);
Int64 xf_data_stream_read_i64 (XfDataStream* stream);

CString xf_data_stream_read_cstring (XfDataStream* stream, CString buf, Size buf_size);

Uint8*  xf_data_stream_read_u8_arr (XfDataStream* stream, Uint8* buf, Size buf_size);
Uint16* xf_data_stream_read_u16_arr (XfDataStream* stream, Uint16* buf, Size buf_size);
Uint32* xf_data_stream_read_u32_arr (XfDataStream* stream, Uint32* buf, Size buf_size);
Uint64* xf_data_stream_read_u64_arr (XfDataStream* stream, Uint64* buf, Size buf_size);

Int8*  xf_data_stream_read_i8_arr (XfDataStream* stream, Int8* buf, Size buf_size);
Int16* xf_data_stream_read_i16_arr (XfDataStream* stream, Int16* buf, Size buf_size);
Int32* xf_data_stream_read_i32_arr (XfDataStream* stream, Int32* buf, Size buf_size);
Int64* xf_data_stream_read_i64_arr (XfDataStream* stream, Int64* buf, Size buf_size);

Uint8* xf_data_stream_read_struct (XfDataStream* stream, XfStructDesc* struct_desc);

#endif // ANVIE_CROSSFILE_STREAM_H
