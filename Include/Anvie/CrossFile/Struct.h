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

/* fwd declarations */
typedef struct XfStructFieldDesc XfStructFieldDesc;
typedef struct XfStructDesc      XfStructDesc;

typedef enum XfStructFieldType {
    XF_STRUCT_FIELD_TYPE_BASIC,
    XF_STRUCT_FIELD_TYPE_STRUCT
} XfStructFieldType;

/**
 * @b Custom pprint method to print extra information.
 *
 * @param desc Field descriptor.
 * @param data Pointer to data that needs to be pprinted.
 * */
typedef Bool (*XfStructFieldPprinter) (XfStructFieldDesc* desc, void* data);

/**
 * @b Represents a field in a struct. This can define basic types like Uint8, Uint16, Uint32, etc...
 *    or made up types like Uint24, or Int24. Also, can recursively define this field as another
 *    struct.
 * */
struct XfStructFieldDesc {
    CString field_name;   /**< @b Name of field to be used for pretty printing. */
    Uint32  field_offset; /**< @b Field offset from starting of struct where data must be read */
    Uint32  data_offset;  /**< @b Offset of data in the data stream from where struct is loaded. */

    /**< @b A field can be either absic type or a struct */
    XfStructFieldType field_type;

    /* tagged union */
    union {
        Size size; /**< @b Size of element in an array or just the size of field */

        /**
         * @b If this field is a struct, this is needed. This only describes element
         * and not the array (if this is one). An element count not equal to 1 means
         * there's an array of structs defined by the this field descriptor.
         * */
        XfStructDesc* desc;
    } element;

    /**
     * @b Greater than 1 if it's an array. Equal to 1 if this is just a single element,
     *    and equal to 0 if this is a dynamic array.
     *
     * Size of dynamic array is computed based on how many bytes is left to read.
     * */
    Uint32 element_count;

    /**
     * @b Optional custom pprinter. If this is non-null then it'll be used to print the value
     * along with original value.
     * */
    XfStructFieldPprinter pprinter;
};

XfStructFieldDesc* xf_struct_field_desc_init_clone (XfStructFieldDesc* dst, XfStructFieldDesc* src);
XfStructFieldDesc* xf_struct_field_desc_deinit (XfStructFieldDesc* field_desc);
Uint8* xf_struct_field_desc_deinit_data (XfStructFieldDesc* field_desc, Uint8* field_data);

/**
 * @b Describes a struct with as many fields as possible in a recursive way.
 *
 * A struct contains field descriptions that itself may describe further structs.
 * This
 * */
struct XfStructDesc {
    CString struct_name; /**< @b Name of struct to be used for pretty printing. */
    Size    struct_size; /**< @b Auto computed value as fields keep adding up. */

    /** Dynamic array of field descriptors */
    struct {
        Size               count;       /**< @b Number of descriptors. */
        Size               capacity;    /**< @b Capacity of descriptor array. */
        XfStructFieldDesc* descriptors; /**< @b Dynamic array to store descriptors. */
    } field;
};

XfStructDesc* xf_struct_desc_init (XfStructDesc* struct_desc, CString name);
XfStructDesc* xf_struct_desc_init_clone (XfStructDesc* dst, XfStructDesc* src);
XfStructDesc* xf_struct_desc_deinit (XfStructDesc* struct_desc);
XfStructFieldDesc*
       xf_struct_desc_add_field (XfStructDesc* struct_desc, XfStructFieldDesc* field_desc);
Uint8* xf_struct_desc_deinit_data (XfStructDesc* struct_desc, Uint8* struct_data);

#endif // ANVIE_CROSSFILE_STRUCT_H
