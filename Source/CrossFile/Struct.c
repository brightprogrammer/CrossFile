/**
 * @file Struct.c
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
#include <Anvie/CrossFile/Struct.h>

/* libc */
#include <string.h>

/**
 * @b Represents a field in a struct. This can define basic types like Uint8, Uint16, Uint32, etc...
 *    or made up types like Uint24, or Int24. Also, can recursively define this field as another
 *    struct.
 * */
typedef struct StructFieldDesc {
    CString field_name;   /**< @b Name of field to be used for pretty printing. */
    Uint32  field_offset; /**< @b Field offset from starting of struct where data must be read */

    /**< @b @c True if field is basic, @c False if field is a struct. */
    Bool field_is_basic;

    /* tagged union */
    union {
        Size element_size; /**< @b Size of element in an array or just the size of field */

        /**
         * @b If this field is a struct, this is needed. This only describes element
         * and not the array (if this is one). An element count not equal to 1 means
         * there's an array of structs defined by the this field descriptor.
         * */
        XfStructDesc* element_desc;
    };

    /**
     * @b Greater than 1 if it's an array. Equal to 1 if this is just a single element,
     *    and equal to 0 if this is a dynamic array.
     *
     * Size of dynamic array is computed based on how many bytes is left to read.
     * */
    Uint32 element_count;

    /**
     * @b Optional custom pprinter. If this is non-null then it'll be used to print the value
     * along with original value. This is only used when field type is basic.
     * */
    XfStructFieldPprinter pprinter;
} StructFieldDesc;

static inline StructFieldDesc*
    struct_field_desc_init_clone (StructFieldDesc* dst, StructFieldDesc* src);
static inline StructFieldDesc* struct_field_desc_deinit (StructFieldDesc* field_desc);
static inline Uint8* struct_field_desc_deinit_data (StructFieldDesc* field_desc, Uint8* field_data);
static inline Uint8* struct_field_desc_init_data_from_stream (
    StructFieldDesc* field_desc,
    XfDataStream*    data_stream,
    Uint8*           buf,
    Size             buf_size
);

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
        Size             count;       /**< @b Number of descriptors. */
        Size             capacity;    /**< @b Capacity of descriptor array. */
        StructFieldDesc* descriptors; /**< @b Dynamic array to store descriptors. */
    } field;
};

static inline XfStructDesc*
                     struct_desc_add_field (XfStructDesc* struct_desc, StructFieldDesc* field_desc);
static inline Uint8* struct_desc_init_data_from_stream (
    XfStructDesc* struct_desc,
    XfDataStream* stream,
    Uint8*        buf,
    Size          buf_size
);

/**************************************************************************************************/
/******************************** STRUCT DESCRIPTOR PUBLIC METHODS ********************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c XfStructDesc object.
 *
 * @param name Name of struct. To be used during pprint call.
 *
 * @return @c struct_desc on success.
 * @return @c Null otherwise.
 * */
XfStructDesc* xf_struct_desc_init (XfStructDesc* struct_desc, CString name) {
    RETURN_VALUE_IF (!struct_desc || !name, Null, ERR_INVALID_ARGUMENTS);

    struct_desc->struct_name = strdup (name);

    Size             cap         = 4;
    StructFieldDesc* field_descs = ALLOCATE (StructFieldDesc, cap);
    GOTO_HANDLER_IF (!field_descs, INIT_FAILED, ERR_OUT_OF_MEMORY);

    struct_desc->field.capacity    = cap;
    struct_desc->field.count       = 0;
    struct_desc->field.descriptors = field_descs;

    return struct_desc;

INIT_FAILED:
    xf_struct_desc_deinit (struct_desc);
    return Null;
}

/**
 * @b Initialize clone of given @c XfStructDesc object.
 *
 * @param dst Destination @c XfStructDesc object.
 * @param src Source @c XfStructDesc object.
 *
 * @return @c dst on success.
 * @return @c Null otherwise.
 * */
XfStructDesc* xf_struct_desc_init_clone (XfStructDesc* dst, XfStructDesc* src) {
    RETURN_VALUE_IF (!dst || !src, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (!(dst->struct_name = strdup (src->struct_name)), Null, ERR_OUT_OF_MEMORY);

    StructFieldDesc* field_descs = ALLOCATE (StructFieldDesc, src->field.count);
    GOTO_HANDLER_IF (!field_descs, INIT_FAILED, ERR_OUT_OF_MEMORY);

    dst->field.descriptors = field_descs;
    dst->field.capacity    = src->field.count;
    dst->field.count       = src->field.count;

    for (Size s = 0; s < dst->field.count; s++) {
        GOTO_HANDLER_IF (
            !struct_field_desc_init_clone (field_descs + s, src->field.descriptors + s),
            INIT_FAILED,
            "Failed to clone field descriptor\n"
        );
    }

    return dst;

INIT_FAILED:
    xf_struct_desc_deinit (dst);
    return Null;
}

/**
 * @b De-initialize given struct descriptor (@c XfStructDesc).
 *
 * @param struct_desc Object to be de-initialized.
 *
 * @return @c struct_desc on success.
 * @return @c Null otherwise.
 * */
XfStructDesc* xf_struct_desc_deinit (XfStructDesc* struct_desc) {
    RETURN_VALUE_IF (!struct_desc, Null, ERR_INVALID_ARGUMENTS);

    if (struct_desc->struct_name) {
        FREE (struct_desc->struct_name);
    }

    if (struct_desc->field.descriptors) {
        for (Size s = 0; s < struct_desc->field.count; s++) {
            struct_field_desc_deinit (struct_desc->field.descriptors + s);
        }

        FREE (struct_desc->field.descriptors);
    }

    memset (struct_desc, 0, sizeof (XfStructDesc));
    return struct_desc;
}

/**
 * @b Add a basic field to the struct.
 *
 * Basic field is any one of the following : Uint8, Uint16, Uint32, Uint64,
 * CString, Size, Char, Int8, Int16, Int32, Int64 and Bool or any pointer
 * of these.
 *
 * @param struct_desc Where the new field will be added?
 * @param field_name Name of field (used only for pprinting).
 * @param field_offset Offset into the struct while reading and storing data.
 * @param element_size Size of basic element in bytes. 
 * @param element_count Number of elements (if an array). 
 *        - Value of 0 means variable sized array.
 *        - Value equal to 1 means single element. 
 *        - Value greater than 1 means fixed size array.
 *
 * @return @c struct_desc on success.
 * @return @c Null otherwise.
 * */
XfStructDesc* xf_struct_desc_add_basic_field (
    XfStructDesc*         struct_desc,
    CString               field_name,
    Uint32                field_offset,
    Size                  element_size,
    Uint32                element_count,
    XfStructFieldPprinter pprinter
) {
    RETURN_VALUE_IF (!struct_desc || !field_name || !element_size, Null, ERR_INVALID_ARGUMENTS);

    return struct_desc_add_field (
        struct_desc,
        &(StructFieldDesc
        ) {.field_name     = field_name,
           .field_offset   = field_offset,
           .field_is_basic = True,
           .element_size   = element_size,
           .element_count  = element_count,
           .pprinter       = pprinter}
    );

    return struct_desc;
}

/**
 * @b Add a struct field to the struct.
 *
 * @param struct_desc Where the new field will be added?
 * @param field_name Name of field (used only for pprinting).
 * @param field_offset Offset into the struct while reading and storing data.
 * @param element_desc Struct descriptor for the struct field.
 * @param element_count Number of structs (if an array). 
 *        - Value of 0 means variable sized array.
 *        - Value equal to 1 means single element. 
 *        - Value greater than 1 means fixed size array.
 *
 * @return @c struct_desc on success.
 * @return @c Null otherwise.
 * */
XfStructDesc* xf_struct_desc_add_struct_field (
    XfStructDesc* struct_desc,
    CString       field_name,
    Uint32        field_offset,
    XfStructDesc* element_desc,
    Uint32        element_count
) {
    RETURN_VALUE_IF (!struct_desc || !field_name || !element_desc, Null, ERR_INVALID_ARGUMENTS);

    return struct_desc_add_field (
        struct_desc,
        &(StructFieldDesc
        ) {.field_name     = field_name,
           .field_offset   = field_offset,
           .field_is_basic = False,
           .element_desc   = element_desc,
           .element_count  = element_count}
    );

    return struct_desc;
}

/**
 * @b Create and read struct based on struct description.
 *
 * @param stream Data stream from where struct must be loaded.
 * @param struct_desc Struct descripiton to follow while loading data from stream.
 *
 * @return Reference to memory containing data in format described by @c struct_desc on success.
 * @return @c Null otherwise.
 * */
Uint8* xf_struct_desc_init_data_from_stream (XfStructDesc* struct_desc, XfDataStream* stream) {
    RETURN_VALUE_IF (!stream || !struct_desc, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        !struct_desc->struct_size,
        Null,
        "Given structure description describes struct to have size 0. I cannot read this!\n"
    );

    Uint8* struct_data = ALLOCATE (Uint8, struct_desc->struct_size);
    Uint8* buf         = struct_data;
    Size   buf_size    = struct_desc->struct_size;

    for (Size s = 0; s < struct_desc->field.count; s++) {
        StructFieldDesc* field_desc = struct_desc->field.descriptors + s;

        GOTO_HANDLER_IF (
            !struct_field_desc_init_data_from_stream (field_desc, stream, buf, buf_size),
            READ_FAILED,
            "Failed to read/init field from data stream\n"
        );
    }

    return struct_data;

READ_FAILED:
    xf_struct_desc_deinit_data (struct_desc, struct_data);
    FREE (struct_data);
    return Null;
}

/**
 * @b Using given struct descriptor, deinitialize the contents of given struct data.
 *
 * @param struct_desc Structure descriptor to follow while de-initialization
 * @param struct_data Structure data to be de-initialized (not freed).
 *
 * @return @c struct_data on success.
 * @return @c Null otherwise.
 * */
Uint8* xf_struct_desc_deinit_data (XfStructDesc* struct_desc, Uint8* struct_data) {
    RETURN_VALUE_IF (!struct_desc || !struct_data, Null, ERR_INVALID_ARGUMENTS);
    /* TODO: */
    return struct_data;
}

/**************************************************************************************************/
/***************************** STRUCT FIELD DESCRIPTOR PRIVATE METHODS ****************************/
/**************************************************************************************************/

/**
 * @b Initialize given @c dst struct field descriptor by creating clone of @c src
 *
 * @param dst Reference to destination @c XfStructFieldDesc object.
 * @param src Reference to source @c XfStructFieldDesc object.
 *
 * @return @c field_desc on success.
 * @return @c Null otherwise.
 * */
static StructFieldDesc* struct_field_desc_init_clone (StructFieldDesc* dst, StructFieldDesc* src) {
    RETURN_VALUE_IF (!dst || !src, Null, ERR_INVALID_ARGUMENTS);

    /* make sure element count is non-zero */
    RETURN_VALUE_IF (
        !src->element_count,
        Null,
        "Field must have at least one element with given description. Current description states 0 "
        "elements.\n"
    );

    /* make sure element size is non-zero */
    RETURN_VALUE_IF (
        ((src->field_is_basic && !src->element_size) ||
         (!src->field_is_basic && !src->element_desc->struct_size)),
        Null,
        "Field cannot have 0 size. Current description describes field to have an element size of "
        "0.\n"
    );

    /* copy name */
    RETURN_VALUE_IF (!(dst->field_name = strdup (src->field_name)), Null, ERR_OUT_OF_MEMORY);

    /* copy description */
    if (src->field_is_basic) {
        dst->element_size = src->element_size;
    } else {
        GOTO_HANDLER_IF (!(dst->element_desc = NEW (XfStructDesc)), INIT_FAILED, ERR_OUT_OF_MEMORY);
        GOTO_HANDLER_IF (
            !xf_struct_desc_init_clone (dst->element_desc, src->element_desc),
            INIT_FAILED,
            "Failed to create StructDesc clone\n"
        );
    }

    dst->field_offset  = src->field_offset;
    dst->element_count = src->element_count;
    dst->pprinter      = src->pprinter;

    return dst;

INIT_FAILED:
    struct_field_desc_deinit (dst);
    return Null;
}

/**
 * @b De-initialize given struct field descriptor.
 *
 * @param field_desc Field descriptor to be deinitialized.
 *
 * @return @c field_desc on success.
 * @return @c Null otherwise.
 * */
StructFieldDesc* struct_field_desc_deinit (StructFieldDesc* field_desc) {
    RETURN_VALUE_IF (!field_desc, Null, ERR_INVALID_ARGUMENTS);

    if (!field_desc->field_is_basic && field_desc->element_desc) {
        xf_struct_desc_deinit (field_desc->element_desc);
        FREE (field_desc->element_desc);
    }

    if (field_desc->field_name) {
        FREE (field_desc->field_name);
    }

    memset (field_desc, 0, sizeof (StructFieldDesc));
    return field_desc;
}

Uint8* struct_field_desc_deinit_data (StructFieldDesc* field_desc, Uint8* field_data) {
    RETURN_VALUE_IF (!field_desc || !field_data, Null, ERR_INVALID_ARGUMENTS);
    /* TODO: */
    return field_data;
}

/**
 * @b Initialize given memory region with struct field data.
 *
 * Size of buffer must always match exactly the size of data to be read.
 * - If this field is a single element then size must be that of sigle element.
 * - If field is an array of known size then size must match the product of field size
 *   and element count.
 * - If field is a variable sized array then @c buf_size must be integral multiple of field size,
 *   and element count will be deduced by dividing buf_size by element size.
 *
 * @param field_desc
 * @param stream
 * @param buf
 * @param buf_size
 *
 * @return @c buf on success
 * @return @c Null otherwise.
 * */
static inline Uint8* struct_field_desc_init_data_from_stream (
    StructFieldDesc* field_desc,
    XfDataStream*    stream,
    Uint8*           buf,
    Size             buf_size
) {
    RETURN_VALUE_IF (!field_desc || !stream || !buf, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (!buf_size, Null, "No space left to write to in provided buffer\n");

    /* HACK: a read hack to read values that are not in powers of two or are of size greater than 8 */
    if (field_desc->field_is_basic) {
        for (Size s = 0; s < field_desc->element_count; s++) {
            if ((XF_HOST_BYTE_ORDER_IS_LSB && xf_data_stream_byte_order_is_msb (stream)) ||
                (XF_HOST_BYTE_ORDER_IS_MSB && xf_data_stream_byte_order_is_lsb (stream))) {
                /* invert read order */
                for (Size s = 0; s < field_desc->element_size; s++) {
                    buf[field_desc->element_size - 1 - s] = xf_data_stream_read_u8 (stream);
                }
            } else {
                /* read order remains same */
                for (Size s = 0; s < field_desc->element_size; s++) {
                    buf[s] = xf_data_stream_read_u8 (stream);
                }
            }
        }
    } else {
        Uint8* buf_iter = buf;
        RETURN_VALUE_IF (
            buf_size < (field_desc->element_count * field_desc->element_desc->struct_size),
            Null,
            "Given buffer size is less than amount of data to be read into it. Buffer to "
            "small. Cannot read into this.\n"
        );

        /* read all elements in the array */
        for (Size s = 0; s < field_desc->element_count; s++) {
            RETURN_VALUE_IF (
                !struct_desc_init_data_from_stream (
                    field_desc->element_desc,
                    stream,
                    buf_iter,
                    field_desc->element_desc->struct_size
                ),
                Null,
                "Failed to read struct field\n"
            );
            buf_iter += field_desc->element_desc->struct_size;
        }
    }

    return buf;
}

/**************************************************************************************************/
/************************** STRUCT DESCRIPTOR PRIVATE METHOD DEFINITIONS **************************/
/**************************************************************************************************/

/**
 * @b Add a new field to the struct. The new field will be cloned and the new copy,
 * completely separate from given copy will be kept and destroyed later on.
 *
 * @param struct_desc
 * @param field_desc Description  of field to be added to struct.
 *
 * @return Reference to new @c XfStructFieldDesc added on success.
 * @return @c Null otherwise.
 * */
static inline XfStructDesc*
    struct_desc_add_field (XfStructDesc* struct_desc, StructFieldDesc* field_desc) {
    RETURN_VALUE_IF (!struct_desc || !field_desc, Null, ERR_INVALID_ARGUMENTS);

    /* make sure element size is never zero */
    RETURN_VALUE_IF (
        ((field_desc->field_is_basic && !field_desc->element_size) ||
         (!field_desc->field_is_basic && !field_desc->element_desc->struct_size)),
        Null,
        "Element size in field cannot be zero!\n"
    );

    /* make sure element count is never zero */
    RETURN_VALUE_IF (
        !field_desc->element_count,
        Null,
        "Field must have at least one element with given description. Current description states 0 "
        "elements\n"
    );

    /* resize field array if required */
    if (struct_desc->field.count >= struct_desc->field.capacity) {
        Size             newcap = struct_desc->field.count ? struct_desc->field.count * 2 : 4;
        StructFieldDesc* field_descs =
            REALLOCATE (struct_desc->field.descriptors, StructFieldDesc, newcap);
        RETURN_VALUE_IF (!field_descs, Null, ERR_OUT_OF_MEMORY);

        struct_desc->field.descriptors = field_descs;
        struct_desc->field.capacity    = newcap;
    }

    /* clone given field to last element */
    RETURN_VALUE_IF (
        !struct_field_desc_init_clone (
            struct_desc->field.descriptors + struct_desc->field.count,
            field_desc
        ),
        Null,
        ERR_OUT_OF_MEMORY
    );

    /* append to struct size */
    struct_desc->struct_size = MAX (struct_desc->struct_size, field_desc->field_offset);

    if (field_desc->field_is_basic) {
        struct_desc->struct_size += field_desc->element_size * field_desc->element_count;
    } else {
        /* we can only use struct_size here if we assume the provided struct does not change */
        struct_desc->struct_size +=
            field_desc->element_desc->struct_size * field_desc->element_count;
    }

    return struct_desc;
}

static inline Uint8* struct_desc_init_data_from_stream (
    XfStructDesc* struct_desc,
    XfDataStream* stream,
    Uint8*        buf,
    Size          buf_size
) {
    RETURN_VALUE_IF(!struct_desc || !stream || !buf, Null, ERR_INVALID_ARGUMENTS);

    
}
