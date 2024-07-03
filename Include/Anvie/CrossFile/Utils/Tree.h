/**
 * @file BinTree.h
 * @date 1st July 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) Siddharth Mishra. All Rights Reserved.
 * @copyright Copyright (c) Anvie Labs. All Rights Reserved.
 * */

#ifndef ANVIE_CROSSFILE_UTILS_TREE_H
#define ANVIE_CROSSFILE_UTILS_TREE_H

#include <Anvie/Common.h>
#include <Anvie/CrossFile/Utils/Vec.h>

/**
 * Generic tree does not expose data and only defines structure of
 * of binary tree. This definition is required for tree traversals.
 * */
typedef struct GenericBinTree {
    struct GenericBinTree* left;
    struct GenericBinTree* right;
} GenericBinTree;

/* This vector type is only to aid in tree traversals.
 * The following traversal methods return a static vector of this type.
 * 
 * This is kind-of a hacky way to implement backtracking without stack inside a
 * macro. Since macros don't have their own stack, functions are used to flatten
 * out the whole tree by using backtracking and recursion.
 *
 * When the tree is flattened by either of the traversal methods we can easily 
 * execute the traversal macro as if it's a function with a stack and recursion
 * capability.
 *
 * Also, one very important point here is that none of the traversal methods
 * return a `TO_##GenericBinTreeVec` means no transfer-of-ownership of vector
 * must be assumed. The returned vector is actually a static vector that's
 * allocated when first required and is kept in memory throughout the program
 * lifetime once created. In other words, a lazily allocated static vector
 * without transfer-of-ownership.
 *
 * Also, since no cloning (init, deinit) methods are provided, none of the items
 * inserted will be owned by this vector.
 *
 * WARN: This solution is not thread-safe!
 * */
ANV_MAKE_VEC (GenericBinTreeVec, generic_bin_tree, GenericBinTree, Null, Null);

PUBLIC GenericBinTreeVec* anv_generic_bin_tree_inorder_traversal (GenericBinTree* root);
PUBLIC GenericBinTreeVec* anv_generic_bin_tree_preorder_traversal (GenericBinTree* root);
PUBLIC GenericBinTreeVec* anv_generic_bin_tree_postorder_traversal (GenericBinTree* root);

typedef void* (*AnvGenericBinTreeDataCloneInit) (void* dst, void* src);
typedef void* (*AnvGenericBinTreeDataCloneDeinit) (void* data);

/** 
 * @b Inorder traversal
 *
 * @p tree
 * @p iter
 * @p body
 * */
#define ANV_BINTREE_FOREACH_INORDER(tree, iter, body)                                              \
    do {                                                                                           \
        if (tree) {                                                                                \
            /* get tree flattened out vector following inorder traversal */                        \
            GenericBinTreeVec* ____traversal_vec____ = Null;                                       \
            if (!(____traversal_vec____ =                                                          \
                      anv_generic_bin_tree_inorder_traversal ((GenericBinTree*)tree))) {           \
                PRINT_ERR ("Failed to initialize inorder tree traversal vector.\n");               \
                break;                                                                             \
            }                                                                                      \
                                                                                                   \
            /* traverse each element */                                                            \
            GenericBinTree* ____tree_iter____ = Null;                                              \
            ANV_VEC_FOREACH (____traversal_vec____, ____tree_iter____, {                           \
                iter = ____tree_iter____;                                                          \
                {body};                                                                            \
            });                                                                                    \
        }                                                                                          \
    } while (0)

/** 
 * @b Preorder traversal
 *
 * @p tree
 * @p iter
 * @p body
 * */
#define ANV_BINTREE_FOREACH_PREORDER(tree, iter, body)                                             \
    do {                                                                                           \
        if (tree) {                                                                                \
            /* get tree flattened out vector following preorder traversal */                       \
            GenericBinTreeVec* ____traversal_vec____ = Null;                                       \
            if (!(____traversal_vec____ =                                                          \
                      anv_generic_bin_tree_preorder_traversal ((GenericBinTree*)tree))) {          \
                PRINT_ERR ("Failed to initialize preorder tree traversal vector.\n");              \
                break;                                                                             \
            }                                                                                      \
                                                                                                   \
            /* traverse each element */                                                            \
            GenericBinTree* ____tree_iter____ = Null;                                              \
            ANV_VEC_FOREACH (____traversal_vec____, ____tree_iter____, {                           \
                iter = ____tree_iter____;                                                          \
                {body};                                                                            \
            });                                                                                    \
        }                                                                                          \
    } while (0)

/** 
 * @b Postorder traversal
 *
 * @p tree
 * @p iter
 * @p body
 * */
#define ANV_BINTREE_FOREACH_POSTORDER(tree, iter, body)                                            \
    do {                                                                                           \
        if (tree) {                                                                                \
            /* get tree flattened out vector following postorder traversal */                      \
            GenericBinTreeVec* ____traversal_vec____ = Null;                                       \
            if (!(____traversal_vec____ =                                                          \
                      anv_generic_bin_tree_postorder_traversal ((GenericBinTree*)tree))) {         \
                PRINT_ERR ("Failed to initialize postorder tree traversal vector.\n");             \
                break;                                                                             \
            }                                                                                      \
                                                                                                   \
            /* traverse each element */                                                            \
            GenericBinTree* ____tree_iter____ = Null;                                              \
            ANV_VEC_FOREACH (____traversal_vec____, ____tree_iter____, {                           \
                iter = ____tree_iter____;                                                          \
                {body};                                                                            \
            });                                                                                    \
        }                                                                                          \
    } while (0)

/* default traversal method is inorder */
#define ANV_BINTREE_FOREACH ANV_BINTREE_FOREACH_INORDER

#define ANV_MAKE_BINTREE(                                                                          \
    BinTreeTypeName,                                                                               \
    fn_infix,                                                                                      \
    BinTreeDataType,                                                                               \
    item_data_clone_init,                                                                          \
    item_data_clone_deinit                                                                         \
)                                                                                                  \
    typedef struct BinTreeTypeName {                                                               \
        struct BinTreeTypeName* left;                                                              \
        struct BinTreeTypeName* right;                                                             \
        BinTreeDataType         data;                                                              \
    } BinTreeTypeName, TO_##BinTreeTypeName;                                                       \
                                                                                                   \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_create (BinTreeDataType* data);                \
    BinTreeTypeName*      anv_##fn_infix##_bin_tree_init (                                         \
        BinTreeTypeName* root,                                                                \
        BinTreeDataType* data                                                                 \
    );                                                                                        \
    BinTreeTypeName* anv_##fn_infix##_bin_tree_deinit (BinTreeTypeName* root);                     \
    void             anv_##fn_infix##_bin_tree_destroy (TO_##BinTreeTypeName* root);               \
    BinTreeTypeName* anv_##fn_infix##_bin_tree_append (                                            \
        BinTreeTypeName*      root,                                                                \
        TO_##BinTreeTypeName* left,                                                                \
        TO_##BinTreeTypeName* right                                                                \
    );                                                                                             \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_clone_create (BinTreeTypeName* root);          \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_clone_init (                                   \
        BinTreeTypeName* dst,                                                                      \
        BinTreeTypeName* src                                                                       \
    );                                                                                             \
                                                                                                   \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_create (BinTreeDataType* data) {               \
        RETURN_VALUE_IF (!data, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        TO_##BinTreeTypeName* root = NEW (BinTreeTypeName);                                        \
        RETURN_VALUE_IF (!root, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        GOTO_##HANDLER_IF (                                                                        \
            !anv_##fn_infix##_bin_tree_init (root, data),                                          \
            CREATE_FAILED,                                                                         \
            "Failed to initialize tree with given data.\n"                                         \
        );                                                                                         \
                                                                                                   \
        return root;                                                                               \
                                                                                                   \
CREATE_FAILED:                                                                                     \
        if (root) {                                                                                \
            anv_##fn_infix##_bin_tree_destroy (root);                                              \
        }                                                                                          \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    BinTreeTypeName* anv_##fn_infix##_bin_tree_init (                                              \
        BinTreeTypeName* root,                                                                     \
        BinTreeDataType* data                                                                      \
    ) {                                                                                            \
        RETURN_VALUE_IF (!root || !data, Null, ERR_INVALID_ARGUMENTS);                             \
                                                                                                   \
        AnvGenericBinTreeDataCloneInit initer =                                                    \
            (AnvGenericBinTreeDataCloneInit)item_data_clone_init;                                  \
                                                                                                   \
        if (initer) {                                                                              \
            RETURN_VALUE_IF (!initer (&root->data, data), Null, "Failed to create data clone.\n"); \
        } else {                                                                                   \
            memcpy (&root->data, data, sizeof (*data));                                            \
        }                                                                                          \
                                                                                                   \
        return root;                                                                               \
    }                                                                                              \
                                                                                                   \
    void anv_##fn_infix##_bin_tree_destroy (TO_##BinTreeTypeName* root) {                          \
        RETURN_IF (!root, ERR_INVALID_ARGUMENTS);                                                  \
        anv_##fn_infix##_bin_tree_deinit (root);                                                   \
        FREE (root);                                                                               \
    }                                                                                              \
                                                                                                   \
    BinTreeTypeName* anv_##fn_infix##_bin_tree_deinit (BinTreeTypeName* root) {                    \
        RETURN_VALUE_IF (!root, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        AnvGenericBinTreeDataCloneDeinit deiniter =                                                \
            (AnvGenericBinTreeDataCloneDeinit)item_data_clone_deinit;                              \
                                                                                                   \
        if (deiniter) {                                                                            \
            deiniter (&root->data);                                                                \
        }                                                                                          \
                                                                                                   \
        if (root->left) {                                                                          \
            anv_##fn_infix##_bin_tree_destroy (root->left);                                        \
        }                                                                                          \
        if (root->right) {                                                                         \
            anv_##fn_infix##_bin_tree_destroy (root->right);                                       \
        }                                                                                          \
                                                                                                   \
        memset (root, 0, sizeof (*root));                                                          \
        return root;                                                                               \
    }                                                                                              \
                                                                                                   \
    BinTreeTypeName* anv_##fn_infix##_bin_tree_append (                                            \
        BinTreeTypeName*      leaf,                                                                \
        TO_##BinTreeTypeName* left,                                                                \
        TO_##BinTreeTypeName* right                                                                \
    ) {                                                                                            \
        RETURN_VALUE_IF (!leaf || !left || !right, Null, ERR_INVALID_ARGUMENTS);                   \
                                                                                                   \
        RETURN_VALUE_IF (                                                                          \
            left->left || leaf->right,                                                             \
            Null,                                                                                  \
            "Given node is not a leaf node. Cannot append to non-leaf nodes.\n"                    \
        );                                                                                         \
                                                                                                   \
        leaf->left  = left;                                                                        \
        leaf->right = right;                                                                       \
                                                                                                   \
        return leaf;                                                                               \
    }                                                                                              \
                                                                                                   \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_clone_create (BinTreeTypeName* root) {         \
        if (!root) {                                                                               \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        TO_##BinTreeTypeName* cloned_root  = Null;                                                 \
        TO_##BinTreeTypeName* cloned_left  = Null;                                                 \
        TO_##BinTreeTypeName* cloned_right = Null;                                                 \
                                                                                                   \
        cloned_root = anv_##fn_infix##_bin_tree_create (&root->data);                              \
        GOTO_##HANDLER_IF (!cloned_root, CLONE_FAILED, "Failed to create node.\n");                \
                                                                                                   \
        cloned_left = anv_##fn_infix##_bin_tree_clone_create (root->left);                         \
        GOTO_##HANDLER_IF (!cloned_left, CLONE_FAILED, "Failed to create clone of subtree.\n");    \
                                                                                                   \
        cloned_right = anv_##fn_infix##_bin_tree_clone_create (root->right);                       \
        GOTO_##HANDLER_IF (!cloned_right, CLONE_FAILED, "Failed to create clone of subtrees.\n");  \
                                                                                                   \
        GOTO_##HANDLER_IF (                                                                        \
            !(anv_##fn_infix##_bin_tree_append (cloned_root, cloned_left, cloned_right)),          \
            CLONE_FAILED,                                                                          \
            "Failed to create tree clone.\n"                                                       \
        );                                                                                         \
        return cloned_root;                                                                        \
                                                                                                   \
CLONE_FAILED:                                                                                      \
        if (cloned_root) {                                                                         \
            anv_##fn_infix##_bin_tree_destroy (cloned_root);                                       \
        }                                                                                          \
        if (cloned_left) {                                                                         \
            anv_##fn_infix##_bin_tree_destroy (cloned_left);                                       \
        }                                                                                          \
        if (cloned_right) {                                                                        \
            anv_##fn_infix##_bin_tree_destroy (cloned_right);                                      \
        }                                                                                          \
                                                                                                   \
        return Null;                                                                               \
    }                                                                                              \
                                                                                                   \
    TO_##BinTreeTypeName* anv_##fn_infix##_bin_tree_clone_init (                                   \
        BinTreeTypeName* dst,                                                                      \
        BinTreeTypeName* src                                                                       \
    ) {                                                                                            \
        RETURN_VALUE_IF (!dst, Null, ERR_INVALID_ARGUMENTS);                                       \
        if (!src) {                                                                                \
            return Null;                                                                           \
        }                                                                                          \
                                                                                                   \
        /* clone data */                                                                           \
        dst->data = src->data;                                                                     \
                                                                                                   \
        /* clone left and right subtrees */                                                        \
        RETURN_VALUE_IF (                                                                          \
            !(dst = anv_##fn_infix##_bin_tree_append (                                             \
                  dst,                                                                             \
                  anv_##fn_infix##_bin_tree_clone_create (src->left),                              \
                  anv_##fn_infix##_bin_tree_clone_create (src->right)                              \
              )),                                                                                  \
            dst,                                                                                   \
            "Failed to create tree clone\n"                                                        \
        );                                                                                         \
                                                                                                   \
        return dst;                                                                                \
    }

ANV_MAKE_BINTREE (CharBinTree, char, Char, Null, Null);
ANV_MAKE_BINTREE (U8BinTree, u8, Uint8, Null, Null);
ANV_MAKE_BINTREE (U16BinTree, u16, Uint16, Null, Null);
ANV_MAKE_BINTREE (U32BinTree, u32, Uint32, Null, Null);
ANV_MAKE_BINTREE (U64BinTree, u64, Uint64, Null, Null);
ANV_MAKE_BINTREE (I8BinTree, i8, Int8, Null, Null);
ANV_MAKE_BINTREE (I16BinTree, i16, Int16, Null, Null);
ANV_MAKE_BINTREE (I32BinTree, i32, Int32, Null, Null);
ANV_MAKE_BINTREE (I64BinTree, i64, Int64, Null, Null);

#endif // ANVIE_CROSSFILE_UTILS_TREE_H
