/**
 * @file Types.h
 * @date Mon, 8th January 2024
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

#ifndef ANVIE_TYPE_H
#define ANVIE_TYPE_H

/* ------------------------- BASE TYPES ------------------------ */

typedef unsigned long long int Uint64;
typedef unsigned int           Uint32;
typedef unsigned short int     Uint16;
typedef unsigned char          Uint8;
typedef unsigned long int Size;

typedef signed long long int Int64;
typedef signed int           Int32;
typedef signed short int     Int16;
typedef signed char          Int8;

typedef char Char;

typedef float       Float32;
typedef double      Float64;
typedef long double Float80;

typedef unsigned char Bool;

/* ------------------------- POINTERS ------------------------ */

typedef Uint8  *PUint8, TO_PUint8;
typedef Uint16 *PUint16, TO_PUint16;
typedef Uint32 *PUint32, TO_PUint32;
typedef Uint64 *PUint64, TO_PUint64;

typedef Int8  *PInt8, TO_PInt8;
typedef Int16 *PInt16, TO_PInt16;
typedef Int32 *PInt32, TO_PInt32;
typedef Int64 *PInt64, TO_PInt46;

typedef Char *PChar, TO_PChar;
typedef Bool *PBool, TO_PBool;

typedef Float32 *PFloat32, TO_PFloat32;
typedef Float64 *PFloat64, TO_PFloat64;
typedef Float80 *PFloat80, TO_PFloat80;

typedef void *PVoid, TO_PVoid;

typedef const Char *CString, TO_CString;

#define True  ((Bool)1)
#define False ((Bool)0)
#define Null  ((PVoid)0)

#ifndef SIZE_MAX
#    define SIZE_MAX ((Size)1 << (sizeof (Size) * 8 - 1))
#endif

/* -------------------- SIGNED INTEGER LIMITS ---------------------- */

#ifndef INT8_MAX
#    define INT8_MAX 0x7f
#endif

#ifndef INT8_MIN
#    define INT8_MIN 0x80
#endif

#ifndef INT16_MAX
#    define INT16_MAX 0x7fff
#endif

#ifndef INT16_MIN
#    define INT16_MIN 0x8000
#endif

#ifndef INT32_MAX
#    define INT32_MAX 0x7fffffff
#endif

#ifndef INT32_MIN
#    define INT32_MIN 0x80000000
#endif

#ifndef INT64_MAX
#    define INT64_MAX 0x7fffffffffffffff
#endif

#ifndef INT64_MIN
#    define INT64_MIN 0x8000000000000000
#endif

/* -------------------- UNSIGNED INTEGER LIMITS ---------------------- */

#ifndef UINT8_MAX
#    define UINT8_MAX 0xff
#endif

#ifndef UINT16_MAX
#    define UINT16_MAX 0xffff
#endif

#ifndef UINT32_MAX
#    define UINT32_MAX 0xffffffff
#endif

#ifndef UINT64_MAX
#    define UINT64_MAX 0xffffffffffffffff
#endif

#endif // ANVIE_TYPE_H
