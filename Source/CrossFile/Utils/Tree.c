/**
 * @file BinTree.h
 * @date 1st July 2024
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) Siddharth Mishra. All Rights Reserved.
 * @copyright Copyright (c) Anvie Labs. All Rights Reserved.
 * */

#include <Anvie/CrossFile/Utils/Tree.h>

/**
 * Static, lazily allocated vector to emulate stack.
 * This vector will contain flattened out tree. None of the items inside
 * it are owned by the vector.
 * */
static GenericBinTreeVec* traversal_vec = Null;

/**
 * This method enusres that traversal_vec is destroyed before program exits.
 * */
PRIVATE void traversal_vec_atexit_handler() {
    if (traversal_vec) {
        anv_generic_bin_tree_vec_destroy (traversal_vec);
        traversal_vec = Null;
    }
}

PRIVATE GenericBinTreeVec* traversal_vec_get() {
    if (!traversal_vec) {
        RETURN_VALUE_IF (
            !(traversal_vec = anv_generic_bin_tree_vec_create()),
            Null,
            "Failed to create generic bin tree vector.\n"
        );

        atexit (traversal_vec_atexit_handler);
    }

    return traversal_vec;
}

/**
 * @b Reset the contents of traversal vector without freeing it's memory.
 * This will make sure the vector is like a new one but with possibly some extra memory.
 * */
PRIVATE GenericBinTreeVec* traversal_vec_reset() {
    RETURN_VALUE_IF (
        !traversal_vec,
        Null,
        "Generic bin tree traversal vector is not initialized, please initialize it first\n"
    );

    /* capacity remains same, just size is reset. */
    memset (traversal_vec->items, 0, sizeof (GenericBinTree) * traversal_vec->size);
    traversal_vec->size = 0;

    return traversal_vec;
}

/**
 * @b Flatten out tree into a vector by inorder traversal.
 *
 * @param root Root node to start traversing from.
 * @param vec Vector to append traversals to.
 *
 * @return @c GenericBinTreeVec On success.
 * @return @c Null otherwise.
 * */
PRIVATE GenericBinTreeVec* inorder (GenericBinTree* root, GenericBinTreeVec* vec) {
    RETURN_VALUE_IF (!root || !vec, Null, ERR_INVALID_ARGUMENTS);

    /* left -> root -> right */

    if (root->left) {
        RETURN_VALUE_IF (
            !anv_generic_bin_tree_init_inorder_traversal (root->left),
            Null,
            "Failed to visit left subtree"
        );
    }

    RETURN_VALUE_IF (
        !anv_generic_bin_tree_vec_append (vec, root),
        Null,
        "Failed to visit root node"
    );

    if (root->right) {
        RETURN_VALUE_IF (
            !anv_generic_bin_tree_init_inorder_traversal (root->right),
            Null,
            "Failed to visit right subtree"
        );
    }

    return vec;
}

/**
 * @b Flatten out tree into a vector by preorder traversal.
 *
 * @param root Root node to start traversing from.
 * @param vec Vector to append traversals to.
 *
 * @return @c GenericBinTreeVec On success.
 * @return @c Null otherwise.
 * */
PRIVATE GenericBinTreeVec* preorder (GenericBinTree* root, GenericBinTreeVec* vec) {
    RETURN_VALUE_IF (!root || !vec, Null, ERR_INVALID_ARGUMENTS);

    /* root -> left -> right */

    RETURN_VALUE_IF (
        !anv_generic_bin_tree_vec_append (vec, root),
        Null,
        "Failed to visit root node"
    );

    if (root->left) {
        RETURN_VALUE_IF (!preorder (root->left, vec), Null, "Failed to visit left subtree");
    }

    if (root->right) {
        RETURN_VALUE_IF (!preorder (root->right, vec), Null, "Failed to visit right subtree");
    }

    return vec;
}

/**
 * @b Flatten out tree into a vector by postorder traversal.
 *
 * @param root Root node to start traversing from.
 * @param vec Vector to append traversals to.
 *
 * @return @c GenericBinTreeVec On success.
 * @return @c Null otherwise.
 * */
PRIVATE GenericBinTreeVec* postorder (GenericBinTree* root, GenericBinTreeVec* vec) {
    RETURN_VALUE_IF (!root || !vec, Null, ERR_INVALID_ARGUMENTS);

    /* left -> right -> root */

    if (root->left) {
        RETURN_VALUE_IF (!postorder (root->left, vec), Null, "Failed to visit left subtree");
    }

    if (root->right) {
        RETURN_VALUE_IF (!postorder (root->right, vec), Null, "Failed to visit right subtree");
    }

    RETURN_VALUE_IF (
        !anv_generic_bin_tree_vec_append (vec, root),
        Null,
        "Failed to visit root node"
    );

    return vec;
}

/**
 * @b Flatten out tree into a vector by inorder traversal.
 *
 * @param root Root node to start traversing from.
 *
 * @return @c GenericBinTreeVec On success.
 * @return @c Null otherwise.
 * */
#define TREE_ORDER_TRAVERSAL(order)                                                                \
    PUBLIC GenericBinTreeVec* anv_generic_bin_tree_##order##_traversal (GenericBinTree* root) {    \
        RETURN_VALUE_IF (!root, Null, ERR_INVALID_ARGUMENTS);                                      \
                                                                                                   \
        GenericBinTreeVec* vec = Null;                                                             \
        RETURN_VALUE_IF (                                                                          \
            !(vec = traversal_vec_get()),                                                          \
            Null,                                                                                  \
            "Failed to get generic bin tree traversal vec.\n"                                      \
        );                                                                                         \
                                                                                                   \
        return order (root, traversal_vec_reset());                                                \
    }

TREE_ORDER_TRAVERSAL (inorder);
TREE_ORDER_TRAVERSAL (preorder);
TREE_ORDER_TRAVERSAL (postorder);
