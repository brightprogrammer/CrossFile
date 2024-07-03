/**
 * @file Vec.h
 * @date 29th June 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) Siddharth Mishra. All Rights Reserved.
 * @copyright Copyright (c) Anvie Labs. All Rights Reserved.
 * */

#ifndef ANVIE_CROSSFILE_UTILS_VEC_H
#define ANVIE_CROSSFILE_UTILS_VEC_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* libc */
#include <memory.h>

typedef void* (*AnvGenericVecItemDataCloneInit) (void* dst, void* src);
typedef void* (*AnvGenericVecItemDataCloneDeinit) (void* data);

#ifndef ANV_VEC_INITIAL_ITEM_CAPACITY
#    define ANV_VEC_INITIAL_ITEM_CAPACITY 32
#endif

/** @b Convinient foreach definition of vector types */
#define ANV_VEC_FOREACH(vec, iter, body)                                                           \
    do {                                                                                           \
        for (iter = vec->data; iter < vec->data + vec->size; iter++) {                             \
            {body};                                                                                \
        }                                                                                          \
    } while (0)

/**
 * @b Use to define new vector type.
 *
 * @param VecTypeName Name of new vector type.
 * @param fn_infix Function name to place in between "anv" and "_vec_<op>"
 * @param ItemDataType Vector item type.
 * @param vec_item_clone_init Vector item clone method (can be Null)
 * @param vec_item_clone_deinit Vector item clone deinit method (can be Null)
 * */
#define ANV_MAKE_VEC(                                                                              \
    VecTypeName,                                                                                   \
    fn_infix,                                                                                      \
    ItemDataType,                                                                                  \
    vec_item_clone_init,                                                                           \
    vec_item_clone_deinit                                                                          \
)                                                                                                  \
    /* separate typedefs make sure different vector types don't mix */                             \
    typedef struct VecTypeName {                                                                   \
        ItemDataType* data;                                                                        \
        Size          size;                                                                        \
        Size          capacity;                                                                    \
    } VecTypeName, TO_##VecTypeName;                                                               \
                                                                                                   \
    /* forward declarations */                                                                     \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_create();                                            \
    PRIVATE void         anv_##fn_infix##_vec_destroy (VecTypeName* vec);                          \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_init (VecTypeName* vec);                             \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_deinit (VecTypeName* vec);                           \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_append (VecTypeName* vec, ItemDataType* item);       \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_clone_create (VecTypeName* vec);                     \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_clone_init (VecTypeName* dst, VecTypeName* src);     \
                                                                                                   \
    /* create new vector */                                                                        \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_create() {                                           \
        VecTypeName* vec = NEW (VecTypeName);                                                      \
        RETURN_VALUE_IF (!vec, Null, ERR_OUT_OF_MEMORY);                                           \
                                                                                                   \
        if (!anv_##fn_infix##_vec_init (vec)) {                                                    \
            PRINT_ERR (ERR_OBJECT_INITIALIZATION_FAILED);                                          \
            anv_##fn_infix##_vec_destroy (vec);                                                    \
            return Null;                                                                           \
        }                                                                                          \
        return vec;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* destroy given vector */                                                                     \
    PRIVATE void anv_##fn_infix##_vec_destroy (VecTypeName* vec) {                                 \
        RETURN_IF (!vec, ERR_INVALID_ARGUMENTS);                                                   \
        anv_##fn_infix##_vec_deinit (vec);                                                         \
        FREE (vec);                                                                                \
    }                                                                                              \
                                                                                                   \
    /* initialize given vector object */                                                           \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_init (VecTypeName* vec) {                            \
        RETURN_VALUE_IF (!vec, Null, ERR_INVALID_ARGUMENTS);                                       \
                                                                                                   \
        Size cap  = ANV_VEC_INITIAL_ITEM_CAPACITY;                                                 \
        vec->data = ALLOCATE (ItemDataType, cap);                                                  \
        RETURN_VALUE_IF (!vec->data, Null, ERR_OUT_OF_MEMORY);                                     \
        vec->size     = 0;                                                                         \
        vec->capacity = cap;                                                                       \
                                                                                                   \
        return vec;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* de-initialize given vector object */                                                        \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_deinit (VecTypeName* vec) {                          \
        RETURN_VALUE_IF (!vec, Null, ERR_INVALID_ARGUMENTS);                                       \
                                                                                                   \
        AnvGenericVecItemDataCloneDeinit deiniter =                                                \
            (AnvGenericVecItemDataCloneDeinit)vec_item_clone_deinit;                               \
                                                                                                   \
        if (deiniter) {                                                                            \
            ItemDataType* iter = Null;                                                             \
            ANV_VEC_FOREACH (vec, iter, deiniter (iter););                                         \
        } else {                                                                                   \
            memset (vec->data, 0, sizeof (ItemDataType) * vec->size);                              \
        }                                                                                          \
                                                                                                   \
        FREE (vec->data);                                                                          \
        memset (vec, 0, sizeof (VecTypeName));                                                     \
                                                                                                   \
        return vec;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* append a new item to given vector object */                                                 \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_append (VecTypeName* vec, ItemDataType* item) {      \
        RETURN_VALUE_IF (!vec || !item, Null, ERR_INVALID_ARGUMENTS);                              \
                                                                                                   \
        if (vec->size >= vec->capacity) {                                                          \
            Size          newcap = vec->size ? vec->size * 2 : ANV_VEC_INITIAL_ITEM_CAPACITY;      \
            ItemDataType* temp   = REALLOCATE (vec->data, ItemDataType, newcap);                   \
            RETURN_VALUE_IF (!temp, Null, ERR_OUT_OF_MEMORY);                                      \
                                                                                                   \
            vec->data     = temp;                                                                  \
            vec->capacity = newcap;                                                                \
        }                                                                                          \
                                                                                                   \
        AnvGenericVecItemDataCloneInit initer =                                                    \
            (AnvGenericVecItemDataCloneInit)vec_item_clone_init;                                   \
                                                                                                   \
        if (initer) {                                                                              \
            RETURN_VALUE_IF (                                                                      \
                !initer (vec->data + vec->size++, item),                                           \
                Null,                                                                              \
                "Failed to make item clone"                                                        \
            );                                                                                     \
        } else {                                                                                   \
            vec->data[vec->size++] = *item;                                                        \
        }                                                                                          \
                                                                                                   \
        return vec;                                                                                \
    }                                                                                              \
                                                                                                   \
    /* create clone of given vector object */                                                      \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_clone_create (VecTypeName* vec) {                    \
        RETURN_VALUE_IF (!vec, Null, ERR_INVALID_ARGUMENTS);                                       \
                                                                                                   \
        VecTypeName* clone_vec = anv_##fn_infix##_vec_create();                                    \
        RETURN_VALUE_IF (                                                                          \
            !clone_vec,                                                                            \
            Null,                                                                                  \
            "Failed to create new '%s' vector object.\n",                                          \
            #VecTypeName                                                                           \
        );                                                                                         \
                                                                                                   \
        GOTO_HANDLER_IF (                                                                          \
            !anv_##fn_infix##_vec_clone_init (clone_vec, vec),                                     \
            CLONE_FAILED,                                                                          \
            "Failed to create clone of '%s' vector.\n",                                            \
            #VecTypeName                                                                           \
        );                                                                                         \
                                                                                                   \
        return clone_vec;                                                                          \
                                                                                                   \
CLONE_FAILED:                                                                                      \
        anv_##fn_infix##_vec_destroy (clone_vec);                                                  \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* init clone of given vector object */                                                        \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_clone_init (VecTypeName* dst, VecTypeName* src) {    \
        RETURN_VALUE_IF (!src || !dst, Null, ERR_INVALID_ARGUMENTS);                               \
                                                                                                   \
        /* in case of vectors we can reuse the memory,
         * so we just reallocate if required and later on deinit all data */                                             \
        if (src->capacity > dst->capacity) {                                                       \
            ItemDataType* temp = REALLOCATE (dst->data, ItemDataType, src->capacity);              \
            RETURN_VALUE_IF (!temp, Null, ERR_OUT_OF_MEMORY);                                      \
                                                                                                   \
            dst->data     = temp;                                                                  \
            dst->capacity = src->capacity;                                                         \
        }                                                                                          \
                                                                                                   \
        AnvGenericVecItemDataCloneDeinit deiniter =                                                \
            (AnvGenericVecItemDataCloneDeinit)vec_item_clone_deinit;                               \
                                                                                                   \
        /* deinit all data if possible */                                                          \
        if (deiniter) {                                                                            \
            ItemDataType* dst_iter = Null;                                                         \
            ANV_VEC_FOREACH (dst, dst_iter, deiniter (dst_iter););                                 \
        }                                                                                          \
                                                                                                   \
        /* initialize iterators */                                                                 \
        ItemDataType* dst_iter = dst->data;                                                        \
        ItemDataType* src_iter = Null;                                                             \
        dst->size              = 0;                                                                \
                                                                                                   \
        AnvGenericVecItemDataCloneInit initer =                                                    \
            (AnvGenericVecItemDataCloneInit)vec_item_clone_init;                                   \
                                                                                                   \
        /* init item clones */                                                                     \
        if (initer) {                                                                              \
            ANV_VEC_FOREACH (src, src_iter, {                                                      \
                dst_iter++;                                                                        \
                GOTO_HANDLER_IF (                                                                  \
                    !initer (dst_iter, src_iter),                                                  \
                    CLONE_FAILED,                                                                  \
                    "Failed to create item clone\n"                                                \
                );                                                                                 \
                dst->size++;                                                                       \
            });                                                                                    \
        } else {                                                                                   \
            ANV_VEC_FOREACH (src, src_iter, {                                                      \
                dst_iter++;                                                                        \
                *dst_iter = *src_iter;                                                             \
                dst->size++;                                                                       \
            });                                                                                    \
        }                                                                                          \
                                                                                                   \
        return dst;                                                                                \
                                                                                                   \
CLONE_FAILED:                                                                                      \
        anv_##fn_infix##_vec_deinit (dst);                                                         \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    PRIVATE VecTypeName* anv_##fn_infix##_vec_reserve (VecTypeName* vec, Size cap) {               \
        RETURN_VALUE_IF (!vec, Null, ERR_INVALID_ARGUMENTS);                                       \
        if (cap <= vec->capacity) {                                                                \
            return vec;                                                                            \
        }                                                                                          \
                                                                                                   \
        ItemDataType* tmp = REALLOCATE (vec->data, ItemDataType, cap);                             \
        RETURN_VALUE_IF (!tmp, Null, ERR_OUT_OF_MEMORY);                                           \
        vec->data     = tmp;                                                                       \
        vec->capacity = cap;                                                                       \
                                                                                                   \
        return vec;                                                                                \
    }

ANV_MAKE_VEC (CharVec, char, Char, Null, Null);
ANV_MAKE_VEC (U8Vec, u8, Uint8, Null, Null);
ANV_MAKE_VEC (U16Vec, u16, Uint16, Null, Null);
ANV_MAKE_VEC (U32Vec, u32, Uint32, Null, Null);
ANV_MAKE_VEC (U64Vec, u64, Uint64, Null, Null);
ANV_MAKE_VEC (I8Vec, i8, Int8, Null, Null);
ANV_MAKE_VEC (I16Vec, i16, Int16, Null, Null);
ANV_MAKE_VEC (I32Vec, i32, Int32, Null, Null);
ANV_MAKE_VEC (I64Vec, i64, Int64, Null, Null);

#endif
