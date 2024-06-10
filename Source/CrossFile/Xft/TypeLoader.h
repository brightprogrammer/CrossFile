/**
 * @file TypeLoader.h
 * @date Mon, 10th June 2024
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

#ifndef ANVIE_SOURCE_CROSSFILE_XFT_TYPE_LOADER_H
#define ANVIE_SOURCE_CROSSFILE_XFT_TYPE_LOADER_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* proper renaming to make this compatible with public opaque declarations */
typedef struct XftTypeLoader XftTypeLoader;
typedef XftTypeLoader        TypeLoader;

/* this is never exposed locally so can be typedef-ed directly like this. */
typedef struct InsnBlock InsnBlock;

/**
 * @b Type loaders are like methods that get called when VM wants to load
 * another type.
 *
 * Each type laoder stores reference to all the type loader that it can
 * call in future. This is just for quick finding of type loader, because a file
 * may have many types and finding the loader in all those arrays "may" be slow.
 * */
struct XftTypeLoader {
    CString type_name;              /**< @b Name of type (same as struct name/typedef) */
    CString type_doc;               /**< @b Description of given type */
    Size    alloc_size;             /**< @b Allocation size for a single element of this struct */

    InsnBlock *insn_blocks;         /**< @b All the basic blocks in this type loader */
    Size       insn_block_count;    /**< @b Number of basic blocks in this loader. */
    Size       insn_block_capacity; /**< @b Capacity of insn_blocks array */

    TypeLoader **type_loader_refs;  /**< @b References to all type loaders this loader can call. */
    Size         type_loader_ref_count;    /**< @b Number of references in the references array */
    Size         type_loader_ref_capacity; /**< @b Capacity of references array */
};

#endif                                     // ANVIE_SOURCE_CROSSFILE_XFT_TYPE_LOADER_H
