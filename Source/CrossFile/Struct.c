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
#include <Anvie/CrossFile/Struct.h>

/* libc */
#include <string.h>

/**************************************************************************************************/
/***************************** STRUCT FIELD DESCRIPTOR PUBLIC METHODS *****************************/
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
XfStructFieldDesc*
    xf_struct_field_desc_init_clone (XfStructFieldDesc* dst, XfStructFieldDesc* src) {
    RETURN_VALUE_IF (!dst || !src, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (!(dst->field_name = strdup (src->field_name)), Null, ERR_OUT_OF_MEMORY);

    GOTO_HANDLER_IF (!(dst->struct_desc = NEW (XfStructDesc)), INIT_FAILED, ERR_OUT_OF_MEMORY);
    GOTO_HANDLER_IF (
        !xf_struct_desc_init_clone (dst->struct_desc, src->struct_desc),
        INIT_FAILED,
        "Failed to create StructDesc clone\n"
    );

    dst->field_offset  = src->field_offset;
    dst->data_offset   = src->data_offset;
    dst->element_size  = src->element_size;
    dst->element_count = src->element_count;
    dst->pprinter      = src->pprinter;

    return dst;

INIT_FAILED:
    xf_struct_field_desc_deinit (dst);
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
XfStructFieldDesc* xf_struct_field_desc_deinit (XfStructFieldDesc* field_desc) {
    RETURN_VALUE_IF (!field_desc, Null, ERR_INVALID_ARGUMENTS);

    if (field_desc->struct_desc) {
        xf_struct_desc_deinit (field_desc->struct_desc);
        FREE (field_desc->struct_desc);
    }

    if (field_desc->field_name) {
        FREE (field_desc->field_name);
    }

    memset (field_desc, 0, sizeof (XfStructFieldDesc));
    return field_desc;
}

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

    Size               cap         = 4;
    XfStructFieldDesc* field_descs = ALLOCATE (XfStructFieldDesc, cap);
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

    XfStructFieldDesc* field_descs = ALLOCATE (XfStructFieldDesc, src->field.count);
    GOTO_HANDLER_IF (!field_descs, INIT_FAILED, ERR_OUT_OF_MEMORY);

    dst->field.descriptors = field_descs;
    dst->field.capacity    = src->field.count;
    dst->field.count       = src->field.count;

    for (Size s = 0; s < dst->field.count; s++) {
        GOTO_HANDLER_IF (
            !xf_struct_field_desc_init_clone (field_descs + s, src->field.descriptors + s),
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
            xf_struct_field_desc_deinit (struct_desc->field.descriptors + s);
        }

        FREE (struct_desc->field.descriptors);
    }

    memset (struct_desc, 0, sizeof (XfStructDesc));
    return struct_desc;
}

/**
 * @b Add a new field to the struct. The new field will be cloned and the new copy,
 * completely separate from given copy will be kept and destroyed later on.
 *
 * @param struct_desc
 * @param field_desc Description of field to be added to struct.
 *
 * @return Reference to new @c XfStructFieldDesc added on success.
 * @return @c Null otherwise.
 * */
XfStructFieldDesc*
    xf_struct_desc_add_field (XfStructDesc* struct_desc, XfStructFieldDesc* field_desc) {
    RETURN_VALUE_IF (!struct_desc || !field_desc, Null, ERR_INVALID_ARGUMENTS);

    if (struct_desc->field.count >= struct_desc->field.capacity) {
        Size               newcap = struct_desc->field.count ? struct_desc->field.count * 2 : 4;
        XfStructFieldDesc* field_descs =
            REALLOCATE (struct_desc->field.descriptors, XfStructFieldDesc, newcap);
        RETURN_VALUE_IF (!field_descs, Null, ERR_OUT_OF_MEMORY);

        struct_desc->field.descriptors = field_descs;
        struct_desc->field.capacity    = newcap;
    }

    RETURN_VALUE_IF (
        !xf_struct_field_desc_init_clone (
            struct_desc->field.descriptors + struct_desc->field.count,
            field_desc
        ),
        Null,
        ERR_OUT_OF_MEMORY
    );

    return struct_desc->field.descriptors + struct_desc->field.count++;
}
