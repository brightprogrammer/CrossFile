/**
 * @file Xft.h
 * @date Sun, 9th June 2024
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

#ifndef ANVIE_CROSSFILE_XFT_H
#define ANVIE_CROSSFILE_XFT_H

#include <Anvie/Types.h>

typedef struct XfDataStream XfDataStream;

/**
 * @b A FileLoader contains multiple type loaders that are executed in order
 * to load the file from a given data stream.
 * */
typedef struct XftFileLoader XftFileLoader;

/**
 * @b Type loader contains instructions for a single type to be loaded.
 *
 * If the type in return requires loading of some other type, that will automatically
 * be loaded. To load a particular type from a data stream, similar to file stream,
 * one needs to exec this in XftVm.
 * */
typedef struct XftTypeLoader XftTypeLoader;

/**
 * @b The CrossFile Type VM.
 *
 * All descriptions are compiled to bytecode for this machine, that can be later
 * on executed to load a structured file format from a binary data stream.
 * */
typedef struct XftVm XftVm;

Uint8* xft_vm_exec_file_loader (XftVm* vm, XftFileLoader* file_loader, XfDataStream* stream);
Uint8* xft_vm_exec_type_loader (XftVm* vm, XftTypeLoader* type_loader, XfDataStream* stream);

/* CrossFile Type Definition of a struct. */
#define XFT_STRUCT(struct_name, struct_def)                                                        \
    typedef struct struct_name struct_def struct_name;                                             \
    CString                               XF_STRUCT_DEF_STR_##struct_name = "struct " #struct_def

/* CrossFile Type Definition of a union. */
#define XFT_UNION(struct_name, struct_def)                                                         \
    typedef struct struct_name struct_def struct_name;                                             \
    CString                               XF_STRUCT_DEF_STR_##struct_name = "union" #struct_def

/* CrossFile Type Definition to be used inside struct definitons */
#define XFT_VECTOR(vec_type, vec_name, vec_length) vec_type* vec_name

/* XF Type Definition to be used inside struct definitons */
#define XFT_ARRAY(arr_type, arr_name, arr_length) arr_type arr_name[arr_length]

/* CrossFile Type Condition to be used inside struct definitons to switch
 * fields on/off for reading depending on whether condition is true or false */
#define XFT_IF(cond)

/* Documentation annotation for field inside a type description */
#define XFT_DOC(doc_str)

#endif // ANVIE_CROSSFILE_XFT_H
