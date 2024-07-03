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
 * */

#ifndef ANVIE_CROSSFILE_STREAM_H
#define ANVIE_CROSSFILE_STREAM_H

#include <Anvie/Common.h>
#include <Anvie/CrossFile/Utils/Vec.h>
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

PUBLIC XfDataStream* xf_data_stream_open_file (CString filename);
PUBLIC void          xf_data_stream_close (XfDataStream* stream);

// TODO:
PUBLIC XfDataStream* xf_data_stream_seek (XfDataStream* stream, Int64 off);
PUBLIC Int64         xf_data_stream_get_cursor (XfDataStream* stream);
PUBLIC Int64         xf_data_stream_get_size (XfDataStream* stream);
PUBLIC XfDataStream* xf_data_stream_reserve (XfDataStream* stream, Size nb);
PUBLIC Int64         xf_data_stream_get_remaining_size (XfDataStream* stream);

PUBLIC XfDataStream* xf_data_stream_read_t8 (XfDataStream* stream, Uint8* u8);
PUBLIC XfDataStream* xf_data_stream_read_t16 (XfDataStream* stream, Uint16* u16);
PUBLIC XfDataStream* xf_data_stream_read_t32 (XfDataStream* stream, Uint32* u32);
PUBLIC XfDataStream* xf_data_stream_read_t64 (XfDataStream* stream, Uint64* u64);

/**
 * Helper macro to generate wrapper around _read_tN methods.
 * */
#define GEN_READER_WRAPPER(Type, t, N)                                                             \
    PRIVATE XfDataStream* xf_data_stream_read_##t##N (XfDataStream* stream, Type##N* v) {          \
        RETURN_VALUE_IF (!stream || !v, Null, ERR_INVALID_ARGUMENTS);                              \
                                                                                                   \
        Type##N x;                                                                                 \
        RETURN_VALUE_IF (                                                                          \
            !xf_data_stream_read_t##N (stream, (Uint##N*)&x),                                      \
            Null,                                                                                  \
            "Failed to read " #N "-bit value from data stream.\n"                                  \
        );                                                                                         \
                                                                                                   \
        *v = x;                                                                                    \
                                                                                                   \
        return stream;                                                                             \
    }

GEN_READER_WRAPPER (Int, i, 8);
GEN_READER_WRAPPER (Int, i, 16);
GEN_READER_WRAPPER (Int, i, 32);
GEN_READER_WRAPPER (Int, i, 64);

GEN_READER_WRAPPER (Uint, u, 8);
GEN_READER_WRAPPER (Uint, u, 16);
GEN_READER_WRAPPER (Uint, u, 32);
GEN_READER_WRAPPER (Uint, u, 64);

#undef GEN_READER_WRAPPER

PRIVATE XfDataStream* xf_data_stream_read_bool (XfDataStream* stream, Bool* b) {
    RETURN_VALUE_IF (!stream || !b, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!xf_data_stream_read_u8 (stream, b), Null, "Failed to read Bool.\n");
    return stream;
}

PRIVATE XfDataStream* xf_data_stream_read_char (XfDataStream* stream, Char* c) {
    RETURN_VALUE_IF (!stream || !c, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!xf_data_stream_read_i8 (stream, (Int8*)c), Null, "Failed to read Bool.\n");
    return stream;
}


/**
 * Helper macro to generate wrapper method around native byte order specific readers.
 * */
#define GEN_BYTE_ORDER_SPECIFIC_READER(TypeN, tn, TN, ORDER, order)                                \
    PRIVATE XfDataStream* xf_data_stream_read_##order##_##tn (XfDataStream* stream, TypeN* v) {    \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        TypeN x = 0;                                                                               \
        RETURN_VALUE_IF (xf_data_stream_read_##tn (stream, &x), Null, "Failed to read " #TypeN);   \
        x  = XF_HOST_BYTE_ORDER_IS_##ORDER ? x : INVERT_BYTE_ORDER_##TN (x);                       \
        *v = x;                                                                                    \
        return stream;                                                                             \
    }

GEN_BYTE_ORDER_SPECIFIC_READER (Uint16, u16, U16, LSB, le);
GEN_BYTE_ORDER_SPECIFIC_READER (Uint32, u32, U32, LSB, le);
GEN_BYTE_ORDER_SPECIFIC_READER (Uint64, u64, U64, LSB, le);

GEN_BYTE_ORDER_SPECIFIC_READER (Int16, i16, I16, LSB, le);
GEN_BYTE_ORDER_SPECIFIC_READER (Int32, i32, I32, LSB, le);
GEN_BYTE_ORDER_SPECIFIC_READER (Int64, i64, I64, LSB, le);

GEN_BYTE_ORDER_SPECIFIC_READER (Uint16, u16, U16, MSB, be);
GEN_BYTE_ORDER_SPECIFIC_READER (Uint32, u32, U32, MSB, be);
GEN_BYTE_ORDER_SPECIFIC_READER (Uint64, u64, U64, MSB, be);

GEN_BYTE_ORDER_SPECIFIC_READER (Int16, i16, I16, MSB, be);
GEN_BYTE_ORDER_SPECIFIC_READER (Int32, i32, I32, MSB, be);
GEN_BYTE_ORDER_SPECIFIC_READER (Int64, i64, I64, MSB, be);

#undef GEN_BYTE_ORDER_SPECIFIC_READER

/* Sequence readers return vectors like this because if we directly take a buffer,
 * and reading fails somewhere in the middle, this might lead to data corruption.
 *
 * This way we make sure all reads are atomic. Either you read whole or you dont!
 *
 * Also, the vectors returned are TO_XYZ types, so ownership is taken by the caller.
 * */

#define GEN_SEQ_READER_WAPPER(ItemType, suffix, VecTypeName)                                       \
    PRIVATE TO_##VecTypeName* xf_data_stream_read_seq_##suffix (                                   \
        XfDataStream* stream,                                                                      \
        Size          seq_size                                                                     \
    ) {                                                                                            \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        if (!seq_size) {                                                                           \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            xf_data_stream_get_remaining_size (stream) < (Int64)(seq_size * sizeof (ItemType)),    \
            Null,                                                                                  \
            "Not enough data left in data stream.\n"                                               \
        );                                                                                         \
                                                                                                   \
        TO_##VecTypeName* seq = anv_##suffix##_vec_create();                                       \
        anv_##suffix##_vec_reserve (seq, seq_size);                                                \
        seq->size = seq_size;                                                                      \
                                                                                                   \
        /* read into vector */                                                                     \
        ItemType* iter = Null;                                                                     \
        ANV_VEC_FOREACH (seq, iter, {                                                              \
            GOTO_HANDLER_IF (                                                                      \
                !xf_data_stream_read_##suffix (stream, iter),                                      \
                READ_SEQ_FAILED,                                                                   \
                "Failed to read sequence of '" #ItemType "'.\n"                                    \
            );                                                                                     \
        });                                                                                        \
                                                                                                   \
        return seq;                                                                                \
                                                                                                   \
READ_SEQ_FAILED:                                                                                   \
        anv_##suffix##_vec_destroy (seq);                                                          \
        return Null;                                                                               \
    }

GEN_SEQ_READER_WAPPER (Char, char, CharVec);
GEN_SEQ_READER_WAPPER (Uint8, u8, U8Vec);
GEN_SEQ_READER_WAPPER (Uint16, u16, U16Vec);
GEN_SEQ_READER_WAPPER (Uint32, u32, U32Vec);
GEN_SEQ_READER_WAPPER (Uint64, u64, U64Vec);

GEN_SEQ_READER_WAPPER (Int8, i8, I8Vec);
GEN_SEQ_READER_WAPPER (Int16, i16, I16Vec);
GEN_SEQ_READER_WAPPER (Int32, i32, I32Vec);
GEN_SEQ_READER_WAPPER (Int64, i64, I64Vec);

#undef GEN_SEQ_READER_WAPPER

/** 
 * Helper macro for generation of byte order specific sequence reader wrapper methods.
 * */
#define GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER(ItemType, suffix, VecTypeName, order)            \
    PRIVATE TO_##VecTypeName* xf_data_stream_read_##order##_seq_##suffix (                         \
        XfDataStream* stream,                                                                      \
        Size          seq_size                                                                     \
    ) {                                                                                            \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        if (!seq_size) {                                                                           \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            xf_data_stream_get_remaining_size (stream) < (Int64)(seq_size * sizeof (ItemType)),    \
            Null,                                                                                  \
            "Not enough data left in data stream.\n"                                               \
        );                                                                                         \
                                                                                                   \
        TO_##VecTypeName* seq = anv_##suffix##_vec_create();                                       \
        anv_##suffix##_vec_reserve (seq, seq_size);                                                \
        seq->size = seq_size;                                                                      \
                                                                                                   \
        /* read into vector */                                                                     \
        ItemType* iter = Null;                                                                     \
        ANV_VEC_FOREACH (seq, iter, {                                                              \
            GOTO_HANDLER_IF (                                                                      \
                !xf_data_stream_read_##order##_##suffix (stream, iter),                            \
                READ_SEQ_FAILED,                                                                   \
                "Failed to read sequence of '" #ItemType "'.\n"                                    \
            );                                                                                     \
        });                                                                                        \
                                                                                                   \
        return seq;                                                                                \
                                                                                                   \
READ_SEQ_FAILED:                                                                                   \
        anv_##suffix##_vec_destroy (seq);                                                          \
        return Null;                                                                               \
    }

GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint16, u16, U16Vec, le);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint32, u32, U32Vec, le);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint64, u64, U64Vec, le);

GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int16, i16, I16Vec, le);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int32, i32, I32Vec, le);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int64, i64, I64Vec, le);

GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint16, u16, U16Vec, be);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint32, u32, U32Vec, be);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Uint64, u64, U64Vec, be);

GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int16, i16, I16Vec, be);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int32, i32, I32Vec, be);
GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER (Int64, i64, I64Vec, be);

#undef GEN_BYTE_ORDER_SPECIFIC_SEQ_READER_WAPPER

#endif // ANVIE_CROSSFILE_STREAM_H
