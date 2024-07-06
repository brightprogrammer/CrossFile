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

/**
 * @b Reserve given number of bytes space for reading data into stream.
 *
 * @param stream
 * @param cap
 *
 * @return @c stream on success.
 * @return @c Null otherwise.
 * */
PUBLIC IoStream* io_stream_reserve (IoStream* stream, Size cap) {
    RETURN_VALUE_IF (!stream || !cap, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!stream->is_mutable, Null, "Attempt to resize an immutable stream.\n");

    if (cap > stream->capacity) {
        Uint8* data = REALLOCATE (stream->data, Uint8, cap);
        RETURN_VALUE_IF (!data, Null, ERR_OUT_OF_MEMORY);
        stream->data     = data;
        stream->capacity = cap;
    }

    return stream;
}

/**
 * @b Close the given file stream.
 *
 * @param stream
 * */
PUBLIC void io_stream_close (IoStream* stream) {
    RETURN_IF (!stream, ERR_INVALID_ARGUMENTS);
    RETURN_IF (
        !stream->close,
        "Given stream does not have a 'close' method. This might indicate a bug in the "
        "application\n"
    );

    if (stream->data) {
        memset (stream->data, 0, stream->size);
        FREE (stream->data);
    }

    stream->close (stream);
}

/**
 * @b File Stream seek implementation.
 * 
 * @param fstream.
 * @param off.
 *
 * @return @c fstream on success.
 * @return @c Null otherwise.
 * */
PUBLIC IoStream* io_stream_seek (IoStream* fstream, Int64 off) {
    RETURN_VALUE_IF (!fstream, Null, ERR_INVALID_ARGUMENTS);

    if (off > 0) {
        Int64 rem_size;
        RETURN_VALUE_IF (
            (rem_size = io_stream_get_remaining_size (fstream)) == -1,
            Null,
            "Failed to get remaining size of stream.\n"
        );

        RETURN_VALUE_IF (
            rem_size < off,
            Null,
            "Seek offset exceeds current remaining stream size.\n"
        );
    } else if (off < 0) {
        Int64 cursor;
        RETURN_VALUE_IF (
            (cursor = io_stream_get_cursor (fstream)) == -1,
            Null,
            "Failed to get remaining size of stream.\n"
        );

        RETURN_VALUE_IF (
            (cursor + off) < 0,
            Null,
            "Seek offset in reverse direction exceeds stream size.\n"
        );
    }

    fstream->cursor += off;
    return fstream;
}

/**
 * @b File Stream get_cursor implementation.
 * Get's the current read position in file stream.
 *
 * @param fstream.
 *
 * @return Non-negative value on success.
 * @return -1 otherwise.
 * */
PUBLIC Int64 io_stream_get_cursor (IoStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);
    return MIN (fstream->cursor, INT64_MAX);
}

/**
 * @b File Stream get_cursor implementation.
 * Get's the current read position in file stream.
 * 
 * @param fstream.
 *
 * @return @c fstream on success.
 * @return @c Null otherwise.
 * */
PUBLIC IoStream* io_stream_set_cursor (IoStream* fstream, Size pos) {
    RETURN_VALUE_IF (!fstream, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (pos > INT64_MAX, Null, "Cursor position exceeds limit.\n");
    fstream->cursor = pos;
    return fstream;
}

/**
 * @b File Stream get_size implementation.
 * Get's the total size of file loaded by file stream.
 * 
 * @param fstream.
 *
 * @return Non-negative value on success.
 * @return -1 otherwise. 0 might mean invalid size as well in some cases.
 * */
PUBLIC Int64 io_stream_get_size (IoStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);
    return MIN (fstream->size, INT64_MAX);
}

/**
 * @b Get remaining size to be read from stream.
 *
 * @param fstream
 *
 * @return Non-negative value on success.
 * @return -1 otherwise.
 * */
PUBLIC Int64 io_stream_get_remaining_size (IoStream* fstream) {
    RETURN_VALUE_IF (!fstream, -1, ERR_INVALID_ARGUMENTS);

    Int64 cursor;
    RETURN_VALUE_IF (
        (cursor = io_stream_get_cursor (fstream)) == -1,
        -1,
        "Failed to get cursor from stream.\n"
    );

    Int64 size;
    RETURN_VALUE_IF (
        (size = io_stream_get_size (fstream)) == -1,
        -1,
        "Failed to get size of stream.\n"
    );

    return size - cursor;
}

/* gneerate generic n-bit readers */
#define GEN_GENERIC_NBIT_READERS(N)                                                                \
    PRIVATE IoStream* io_stream_read_t##N (IoStream* stream, Uint##N* v) {                         \
        RETURN_VALUE_IF (!stream || !v, Null, ERR_INVALID_ARGUMENTS);                              \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            io_stream_get_remaining_size (stream) < (N >> 3),                                      \
            Null,                                                                                  \
            "Not enough data left in data stream.\n"                                               \
        );                                                                                         \
                                                                                                   \
        Uint##N x = ((Uint##N*)(stream->data + stream->cursor))[0];                                \
        RETURN_VALUE_IF (                                                                          \
            !io_stream_seek (stream, (N >> 3)),                                                    \
            Null,                                                                                  \
            "Failed to seek ahead after reading.\n"                                                \
        );                                                                                         \
        *v = x;                                                                                    \
                                                                                                   \
        return stream;                                                                             \
    }

GEN_GENERIC_NBIT_READERS (8)
GEN_GENERIC_NBIT_READERS (16)
GEN_GENERIC_NBIT_READERS (32)
GEN_GENERIC_NBIT_READERS (64)

#undef GEN_GENERIC_NBIT_READERS

/**
 * Helper macro to generate wrapper around _read_tN methods.
 * */
#define GEN_READER_WRAPPER(Type, t, N)                                                             \
    PUBLIC IoStream* io_stream_read_##t##N (IoStream* stream, Type##N* v) {                        \
        RETURN_VALUE_IF (!stream || !v, Null, ERR_INVALID_ARGUMENTS);                              \
                                                                                                   \
        Type##N x;                                                                                 \
        RETURN_VALUE_IF (                                                                          \
            !io_stream_read_t##N (stream, (Uint##N*)&x),                                           \
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

PUBLIC IoStream* io_stream_read_bool (IoStream* stream, Bool* b) {
    RETURN_VALUE_IF (!stream || !b, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!io_stream_read_u8 (stream, b), Null, "Failed to read Bool.\n");
    return stream;
}

PUBLIC IoStream* io_stream_read_char (IoStream* stream, Char* c) {
    RETURN_VALUE_IF (!stream || !c, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!io_stream_read_i8 (stream, (Int8*)c), Null, "Failed to read Bool.\n");
    return stream;
}

/**
 * Helper macro to generate wrapper method around native byte order specific readers.
 * */
#define GEN_BYTE_ORDER_SPECIFIC_READER(TypeN, tn, TN, ORDER, order)                                \
    PUBLIC IoStream* io_stream_read_##order##_##tn (IoStream* stream, TypeN* v) {                  \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        TypeN x = 0;                                                                               \
        RETURN_VALUE_IF (                                                                          \
            !io_stream_read_##tn (stream, &x),                                                     \
            Null,                                                                                  \
            "Failed to read '" #TypeN "'.\n"                                                       \
        );                                                                                         \
        x  = HOST_BYTE_ORDER_IS_##ORDER ? x : INVERT_BYTE_ORDER_##TN (x);                          \
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
    PUBLIC TO_##VecTypeName* io_stream_read_seq_##suffix (IoStream* stream, Size seq_size) {       \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        if (!seq_size) {                                                                           \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            io_stream_get_remaining_size (stream) < (Int64)(seq_size * sizeof (ItemType)),         \
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
                !io_stream_read_##suffix (stream, iter),                                           \
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
    PUBLIC TO_##VecTypeName* io_stream_read_##order##_seq_##suffix (                               \
        IoStream* stream,                                                                          \
        Size      seq_size                                                                         \
    ) {                                                                                            \
        RETURN_VALUE_IF (!stream, Null, ERR_INVALID_ARGUMENTS);                                    \
        if (!seq_size) {                                                                           \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            io_stream_get_remaining_size (stream) < (Int64)(seq_size * sizeof (ItemType)),         \
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
                !io_stream_read_##order##_##suffix (stream, iter),                                 \
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
