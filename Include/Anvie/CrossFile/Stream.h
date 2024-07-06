/**
 * @file Stream.h
 * @date Fri, 1st June 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2024 Siddharth Mishra
 * @copyright Copyright 2024 Anvie Labs
 *
 * Copyright 2024 Siddharth Mishra, Anvie Labs
 * 
 * Redistribution and use in source and binary formio, with or without modification, are permitted 
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
 * IMPLIED WARRANTIEio, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
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

/**
 * @b A data stream is an opaque abstraction over the data input source.
 *
 * The input can be anything from files to sockets to interprocess communication.
 * As long as an internal stream implementation existio, and the API is followed,
 * users get a uniform interface to interact and read with any data stream.
 * */
typedef struct IoStream IoStream, TO_IoStream;

PUBLIC TO_IoStream* io_stream_open_file (CString filename, Bool is_writable);
PUBLIC void         io_stream_close (TO_IoStream* stream);

PUBLIC IoStream* io_stream_seek (IoStream* io, Int64 off);
PUBLIC Int64     io_stream_get_cursor (IoStream* stream);
PUBLIC IoStream* io_stream_set_cursor (IoStream* io, Size pos);
PUBLIC Int64     io_stream_get_size (IoStream* stream);
PUBLIC IoStream* io_stream_reserve (IoStream* io, Size nb);
PUBLIC Int64     io_stream_get_remaining_size (IoStream* stream);

/* readers */

PUBLIC IoStream* io_stream_read_bool (IoStream* io, PBool b);
PUBLIC IoStream* io_stream_read_char (IoStream* io, PChar c);

PUBLIC IoStream* io_stream_read_u8 (IoStream* io, PUint8 u8);
PUBLIC IoStream* io_stream_read_u16 (IoStream* io, PUint16 u16);
PUBLIC IoStream* io_stream_read_u32 (IoStream* io, PUint32 u32);
PUBLIC IoStream* io_stream_read_u64 (IoStream* io, PUint64 u64);

PUBLIC IoStream* io_stream_read_i8 (IoStream* io, PInt8 i8);
PUBLIC IoStream* io_stream_read_i16 (IoStream* io, PInt16 i16);
PUBLIC IoStream* io_stream_read_i32 (IoStream* io, PInt32 i32);
PUBLIC IoStream* io_stream_read_i64 (IoStream* io, PInt64 i64);

PUBLIC IoStream* io_stream_read_le_u8 (IoStream* io, PUint8 u8);
PUBLIC IoStream* io_stream_read_le_u16 (IoStream* io, PUint16 u16);
PUBLIC IoStream* io_stream_read_le_u32 (IoStream* io, PUint32 u32);
PUBLIC IoStream* io_stream_read_le_u64 (IoStream* io, PUint64 u64);

PUBLIC IoStream* io_stream_read_le_i8 (IoStream* io, PInt8 i8);
PUBLIC IoStream* io_stream_read_le_i16 (IoStream* io, PInt16 i16);
PUBLIC IoStream* io_stream_read_le_i32 (IoStream* io, PInt32 i32);
PUBLIC IoStream* io_stream_read_le_i64 (IoStream* io, PInt64 i64);

PUBLIC IoStream* io_stream_read_be_u8 (IoStream* io, PUint8 u8);
PUBLIC IoStream* io_stream_read_be_u16 (IoStream* io, PUint16 u16);
PUBLIC IoStream* io_stream_read_be_u32 (IoStream* io, PUint32 u32);
PUBLIC IoStream* io_stream_read_be_u64 (IoStream* io, PUint64 u64);

PUBLIC IoStream* io_stream_read_be_i8 (IoStream* io, PInt8 i8);
PUBLIC IoStream* io_stream_read_be_i16 (IoStream* io, PInt16 i16);
PUBLIC IoStream* io_stream_read_be_i32 (IoStream* io, PInt32 i32);
PUBLIC IoStream* io_stream_read_be_i64 (IoStream* io, PInt64 i64);

PUBLIC TO_U8Vec*  io_stream_read_seq_u8 (IoStream* io, Size size);
PUBLIC TO_U16Vec* io_stream_read_seq_u16 (IoStream* io, Size size);
PUBLIC TO_U32Vec* io_stream_read_seq_u32 (IoStream* io, Size size);
PUBLIC TO_U64Vec* io_stream_read_seq_u64 (IoStream* io, Size size);

PUBLIC TO_I8Vec*  io_stream_read_seq_i8 (IoStream* io, Size size);
PUBLIC TO_I16Vec* io_stream_read_seq_i16 (IoStream* io, Size size);
PUBLIC TO_I32Vec* io_stream_read_seq_i32 (IoStream* io, Size size);
PUBLIC TO_I64Vec* io_stream_read_seq_i64 (IoStream* io, Size size);

PUBLIC TO_U16Vec* io_stream_read_le_seq_u16 (IoStream* io, Size size);
PUBLIC TO_U32Vec* io_stream_read_le_seq_u32 (IoStream* io, Size size);
PUBLIC TO_U64Vec* io_stream_read_le_seq_u64 (IoStream* io, Size size);

PUBLIC TO_I16Vec* io_stream_read_le_seq_i16 (IoStream* io, Size size);
PUBLIC TO_I32Vec* io_stream_read_le_seq_i32 (IoStream* io, Size size);
PUBLIC TO_I64Vec* io_stream_read_le_seq_i64 (IoStream* io, Size size);

PUBLIC TO_U16Vec* io_stream_read_be_seq_u16 (IoStream* io, Size size);
PUBLIC TO_U32Vec* io_stream_read_be_seq_u32 (IoStream* io, Size size);
PUBLIC TO_U64Vec* io_stream_read_be_seq_u64 (IoStream* io, Size size);

PUBLIC TO_I16Vec* io_stream_read_be_seq_i16 (IoStream* io, Size size);
PUBLIC TO_I32Vec* io_stream_read_be_seq_i32 (IoStream* io, Size size);
PUBLIC TO_I64Vec* io_stream_read_be_seq_i64 (IoStream* io, Size size);

/* writers */

PUBLIC IoStream* io_stream_write_bool (IoStream* io, Bool b);
PUBLIC IoStream* io_stream_write_char (IoStream* io, Char c);

PUBLIC IoStream* io_stream_write_u8 (IoStream* io, Uint8 u8);
PUBLIC IoStream* io_stream_write_u16 (IoStream* io, Uint16 u16);
PUBLIC IoStream* io_stream_write_u32 (IoStream* io, Uint32 u32);
PUBLIC IoStream* io_stream_write_u64 (IoStream* io, Uint64 u64);

PUBLIC IoStream* io_stream_write_i8 (IoStream* io, Int8 i8);
PUBLIC IoStream* io_stream_write_i16 (IoStream* io, Int16 i16);
PUBLIC IoStream* io_stream_write_i32 (IoStream* io, Int32 i32);
PUBLIC IoStream* io_stream_write_i64 (IoStream* io, Int64 i64);

PUBLIC IoStream* io_stream_write_le_u8 (IoStream* io, Uint8 u8);
PUBLIC IoStream* io_stream_write_le_u16 (IoStream* io, Uint16 u16);
PUBLIC IoStream* io_stream_write_le_u32 (IoStream* io, Uint32 u32);
PUBLIC IoStream* io_stream_write_le_u64 (IoStream* io, Uint64 u64);

PUBLIC IoStream* io_stream_write_le_i8 (IoStream* io, Int8 i8);
PUBLIC IoStream* io_stream_write_le_i16 (IoStream* io, Int16 i16);
PUBLIC IoStream* io_stream_write_le_i32 (IoStream* io, Int32 i32);
PUBLIC IoStream* io_stream_write_le_i64 (IoStream* io, Int64 i64);

PUBLIC IoStream* io_stream_write_be_u8 (IoStream* io, Uint8 u8);
PUBLIC IoStream* io_stream_write_be_u16 (IoStream* io, Uint16 u16);
PUBLIC IoStream* io_stream_write_be_u32 (IoStream* io, Uint32 u32);
PUBLIC IoStream* io_stream_write_be_u64 (IoStream* io, Uint64 u64);

PUBLIC IoStream* io_stream_write_be_i8 (IoStream* io, Int8 i8);
PUBLIC IoStream* io_stream_write_be_i16 (IoStream* io, Int16 i16);
PUBLIC IoStream* io_stream_write_be_i32 (IoStream* io, Int32 i32);
PUBLIC IoStream* io_stream_write_be_i64 (IoStream* io, Int64 i64);

PUBLIC IoStream* io_stream_write_seq_u8 (IoStream* io, PUint8* d8, Size size);
PUBLIC IoStream* io_stream_write_seq_u16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_seq_u32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_seq_u64 (IoStream* io, PUint64* d64, Size size);

PUBLIC IoStream* io_stream_write_seq_i8 (IoStream* io, PUint8* d8, Size size);
PUBLIC IoStream* io_stream_write_seq_i16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_seq_i32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_seq_i64 (IoStream* io, PUint64* d64, Size size);

PUBLIC IoStream* io_stream_write_le_seq_u16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_le_seq_u32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_le_seq_u64 (IoStream* io, PUint64* d64, Size size);

PUBLIC IoStream* io_stream_write_le_seq_i16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_le_seq_i32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_le_seq_i64 (IoStream* io, PUint64* d64, Size size);

PUBLIC IoStream* io_stream_write_be_seq_u16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_be_seq_u32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_be_seq_u64 (IoStream* io, PUint64* d64, Size size);

PUBLIC IoStream* io_stream_write_be_seq_i16 (IoStream* io, PUint16* d16, Size size);
PUBLIC IoStream* io_stream_write_be_seq_i32 (IoStream* io, PUint32* d32, Size size);
PUBLIC IoStream* io_stream_write_be_seq_i64 (IoStream* io, PUint64* d64, Size size);

#endif // ANVIE_CROSSFILE_STREAM_H
