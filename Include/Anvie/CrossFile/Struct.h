/**
 * @file Struct.h
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

#ifndef ANVIE_CROSSFILE_STRUCT_H
#define ANVIE_CROSSFILE_STRUCT_H

#include <Anvie/Types.h>

typedef struct XfStructDesc XfStructDesc;
typedef struct XfDataStream XfDataStream;

/**
 * @b Custom pprint method to print extra information.
 *
 * @param desc Field descriptor.
 * @param data Pointer to data that needs to be pprinted.
 * */
typedef Bool (*XfStructFieldPprinter) (Uint8* data);

XfStructDesc* xf_struct_desc_init (XfStructDesc* struct_desc, CString name);
XfStructDesc* xf_struct_desc_init_clone (XfStructDesc* dst, XfStructDesc* src);
XfStructDesc* xf_struct_desc_deinit (XfStructDesc* struct_desc);
XfStructDesc* xf_struct_desc_add_basic_field (
    XfStructDesc*         struct_desc,
    CString               field_name,
    Uint32                field_offset,
    Size                  element_size,
    Uint32                element_count,
    XfStructFieldPprinter pprinter
);
XfStructDesc* xf_struct_desc_add_struct_field (
    XfStructDesc* struct_desc,
    CString       field_name,
    Uint32        field_offset,
    XfStructDesc* element_desc,
    Uint32        element_count
);
Uint8* xf_struct_desc_init_data_from_stream (XfStructDesc* struct_desc, XfDataStream* stream);
Uint8* xf_struct_desc_deinit_data (XfStructDesc* struct_desc, Uint8* struct_data);

#endif // ANVIE_CROSSFILE_STRUCT_H
