/**
 * @file EndiannessHelpers.h
 * @date Wed, 22nd May 2024
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

#ifndef ANVIE_CROSSFILE_ENDIANNESS_HELPERS_H
#define ANVIE_CROSSFILE_ENDIANNESS_HELPERS_H

#define GET_AND_ADV_U1(d) (Uint8*)d)[0] ;                                                          \
    d++;
#define GET_AND_ADV_U2(d)                                                                          \
    BE2LE_U2 (((Uint16 *)d)[0]);                                                                   \
    d += 2;
#define GET_AND_ADV_U4(d)                                                                          \
    BE2LE_U4 (((Uint32 *)d)[0]);                                                                   \
    d += 4;
#define GET_AND_ADV_U8(d)                                                                          \
    BE2LE_U8 (((Uint64 *)d)[0]);                                                                   \
    d += 8;

#define GET_AND_ADV_I1(d) (Int8*)d)[0]  ;                                                          \
    d++;
#define GET_AND_ADV_I2(d)                                                                          \
    BE2LE_I2 (((Int16 *)d)[0]);                                                                    \
    d += 2;
#define GET_AND_ADV_I4(d)                                                                          \
    BE2LE_I4 (((Int32 *)d)[0]);                                                                    \
    d += 4;
#define GET_AND_ADV_I8(d)                                                                          \
    BE2LE_I8 (((Int64 *)d)[0]);                                                                    \
    d += 8;

#endif // ANVIE_CROSSFILE_ENDIANNESS_HELPERS_H
