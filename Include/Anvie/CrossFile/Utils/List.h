/**
 * @file List.h
 * @date 29th June 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) Siddharth Mishra. All Rights Reserved.
 * @copyright Copyright (c) Anvie Labs. All Rights Reserved.
 * */

#ifndef ANVIE_CROSSFILE_UTIL_LIST_H
#define ANVIE_CROSSFILE_UTIL_LIST_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* libc */
#include <memory.h>

typedef void* (*AnvGenericListItemDataCloneInit) (void* dst, void* src);
typedef void* (*AnvGenericListItemDataCloneDeinit) (void* data);

/** @b Convinient foreach definition of list types */
#define ANV_LIST_FOREACH(list, iter, body)                                                         \
    do {                                                                                           \
        iter = list->head;                                                                         \
        while (iter) {                                                                             \
            void* ____nxt_itm = iter->next;                                                        \
            {body};                                                                                \
            iter = ____nxt_itm;                                                                    \
        }                                                                                          \
    } while (0);

/**
 * @b Use to define new list type.
 *
 * @param ListTypeName Name of new list type.
 * @param fn_infix Function name to place in between "anv" and "_list_<op>"
 * @param ListDataType List data type.
 * @param list_data_clone_init List data clone method (can be Null)
 * @param list_data_clone_deinit List data clone deinit method (can be Null)
 * */
#define ANV_MAKE_LIST(                                                                             \
    ListTypeName,                                                                                  \
    fn_infix,                                                                                      \
    ListDataType,                                                                                  \
    list_data_clone_init,                                                                          \
    list_data_clone_deinit                                                                         \
)                                                                                                  \
    /* separate typedefs make sure different list types don't mix */                               \
    typedef struct ListTypeName##Item {                                                            \
        struct ListTypeName##Item* next;                                                           \
        ListDataType               data;                                                           \
    } ListTypeName##Item, TO_##ListTypeName##Item;                                                 \
                                                                                                   \
    typedef struct ListTypeName {                                                                  \
        ListTypeName##Item* head;                                                                  \
        ListTypeName##Item* tail;                                                                  \
        Size                size;                                                                  \
    } ListTypeName, TO_##ListTypeName;                                                             \
                                                                                                   \
    /* forward declarations */                                                                     \
    PRIVATE ListTypeName* anv_##fn_infix##_list_create();                                          \
    PRIVATE void          anv_##fn_infix##_list_destroy (ListTypeName* list);                      \
    PRIVATE ListTypeName* anv_##fn_infix##_list_init (ListTypeName* list);                         \
    PRIVATE ListTypeName* anv_##fn_infix##_list_deinit (ListTypeName* list);                       \
    PRIVATE ListTypeName* anv_##fn_infix##_list_append (ListTypeName* list, ListDataType* data);   \
    PRIVATE ListTypeName* anv_##fn_infix##_list_clone_create (ListTypeName* list);                 \
    PRIVATE ListTypeName* anv_##fn_infix##_list_clone_init (ListTypeName* dst, ListTypeName* src); \
                                                                                                   \
    /* create new list */                                                                          \
    PRIVATE ListTypeName* anv_##fn_infix##_list_create() {                                         \
        ListTypeName* list = NEW (ListTypeName);                                                   \
        RETURN_VALUE_IF (!list, Null, ERR_OUT_OF_MEMORY);                                          \
                                                                                                   \
        if (!anv_##fn_infix##_list_init (list)) {                                                  \
            PRINT_ERR (ERR_OBJECT_INITIALIZATION_FAILED);                                          \
            anv_##fn_infix##_list_destroy (list);                                                  \
            return Null;                                                                           \
        }                                                                                          \
        return list;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* destroy given list */                                                                       \
    PRIVATE void anv_##fn_infix##_list_destroy (ListTypeName* list) {                              \
        RETURN_IF (!list, ERR_INVALID_ARGUMENTS);                                                  \
        anv_##fn_infix##_list_deinit (list);                                                       \
        FREE (list);                                                                               \
    }                                                                                              \
                                                                                                   \
    /* initialize given list object */                                                             \
    PRIVATE ListTypeName* anv_##fn_infix##_list_init (ListTypeName* list) {                        \
        RETURN_VALUE_IF (!list, Null, ERR_INVALID_ARGUMENTS);                                      \
        return anv_##fn_infix##_list_deinit (list);                                                \
    }                                                                                              \
                                                                                                   \
    /* de-initialize given list object */                                                          \
    PRIVATE ListTypeName* anv_##fn_infix##_list_deinit (ListTypeName* list) {                      \
        RETURN_VALUE_IF (!list, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        AnvGenericListItemDataCloneDeinit deiniter =                                               \
            (AnvGenericListItemDataCloneDeinit)list_data_clone_deinit;                             \
                                                                                                   \
        /* deinit data if possible */                                                              \
        ListTypeName##Item* iter = Null;                                                           \
        if (deiniter) {                                                                            \
            ANV_LIST_FOREACH (list, iter, {                                                        \
                deiniter ((void*)&iter->data);                                                     \
                memset (iter, 0, sizeof (*iter));                                                  \
                FREE (iter);                                                                       \
            });                                                                                    \
        } else {                                                                                   \
            ANV_LIST_FOREACH (list, iter, {                                                        \
                memset (iter, 0, sizeof (*iter));                                                  \
                FREE (iter);                                                                       \
            });                                                                                    \
        }                                                                                          \
                                                                                                   \
        memset (list, 0, sizeof (ListTypeName));                                                   \
                                                                                                   \
        return list;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* append a new data to given list object */                                                   \
    PRIVATE ListTypeName* anv_##fn_infix##_list_append (ListTypeName* list, ListDataType* data) {  \
        RETURN_VALUE_IF (!list || !data, Null, ERR_INVALID_ARGUMENTS);                             \
                                                                                                   \
        ListTypeName##Item* item = NEW (ListTypeName##Item);                                       \
        RETURN_VALUE_IF (!item, Null, ERR_OUT_OF_MEMORY);                                          \
                                                                                                   \
        AnvGenericListItemDataCloneInit initer =                                                   \
            (AnvGenericListItemDataCloneInit)list_data_clone_init;                                 \
                                                                                                   \
        if (initer) {                                                                              \
            GOTO_HANDLER_IF (                                                                      \
                !initer ((void*)&item->data, (void*)data),                                         \
                APPEND_FAILED,                                                                     \
                "Failed to make data clone"                                                        \
            );                                                                                     \
        } else {                                                                                   \
            item->data = *data;                                                                    \
        }                                                                                          \
                                                                                                   \
        /* if tail exists then append and then update tail reference to this item */               \
        if (list->tail) {                                                                          \
            list->tail->next = item;                                                               \
        }                                                                                          \
        list->tail = item;                                                                         \
                                                                                                   \
        /* if there's no current head then tail and head are equal */                              \
        if (!list->head) {                                                                         \
            list->head = list->tail;                                                               \
        }                                                                                          \
                                                                                                   \
        return list;                                                                               \
                                                                                                   \
APPEND_FAILED:                                                                                     \
        FREE (item);                                                                               \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* create clone of given list object */                                                        \
    PRIVATE ListTypeName* anv_##fn_infix##_list_clone_create (ListTypeName* list) {                \
        RETURN_VALUE_IF (!list, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        ListTypeName* clone_list = anv_##fn_infix##_list_create();                                 \
        RETURN_VALUE_IF (                                                                          \
            !clone_list,                                                                           \
            Null,                                                                                  \
            "Failed to create new '%s' list to create clone\n",                                    \
            #ListTypeName                                                                          \
        );                                                                                         \
                                                                                                   \
        GOTO_HANDLER_IF (                                                                          \
            !anv_##fn_infix##_list_clone_init (clone_list, list),                                  \
            CLONE_FAILED,                                                                          \
            "Failed to init clone\n"                                                               \
        );                                                                                         \
                                                                                                   \
CLONE_FAILED:                                                                                      \
        anv_##fn_infix##_list_destroy (clone_list);                                                \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    /* init clone of given list object */                                                          \
    PRIVATE ListTypeName* anv_##fn_infix##_list_clone_init (                                       \
        ListTypeName* dst,                                                                         \
        ListTypeName* src                                                                          \
    ) {                                                                                            \
        RETURN_VALUE_IF (!src || !dst, Null, ERR_INVALID_ARGUMENTS);                               \
        anv_##fn_infix##_list_deinit (dst);                                                        \
                                                                                                   \
        ListTypeName##Item* iter = Null;                                                           \
        ANV_LIST_FOREACH (src, iter, {                                                             \
            GOTO_HANDLER_IF (                                                                      \
                !anv_##fn_infix##_list_append (dst, &iter->data),                                  \
                CLONE_FAILED,                                                                      \
                "Failed to append item to clone destination\n"                                     \
            );                                                                                     \
        });                                                                                        \
                                                                                                   \
CLONE_FAILED:                                                                                      \
        anv_##fn_infix##_list_deinit (dst);                                                        \
        return Null;                                                                               \
    }

ANV_MAKE_LIST (U8List, u8, Uint8, Null, Null);
ANV_MAKE_LIST (U16List, u16, Uint16, Null, Null);
ANV_MAKE_LIST (U32List, u32, Uint32, Null, Null);
ANV_MAKE_LIST (U64List, u64, Uint64, Null, Null);
ANV_MAKE_LIST (I8List, i8, Int8, Null, Null);
ANV_MAKE_LIST (I16List, i16, Int16, Null, Null);
ANV_MAKE_LIST (I32List, i32, Int32, Null, Null);
ANV_MAKE_LIST (I64List, i64, Int64, Null, Null);

#endif
