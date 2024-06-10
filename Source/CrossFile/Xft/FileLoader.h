/**
 * @file FileLoader.h
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


#ifndef ANVIE_SOURCE_CROSSFILE_XFT_FILE_LOADER_H
#define ANVIE_SOURCE_CROSSFILE_XFT_FILE_LOADER_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* proper renaming to make this definition compatible with public
 * opaque struct declaration */
typedef struct XftFileLoader XftFileLoader;
typedef XftFileLoader        FileLoader;

typedef struct XftTypeLoader XftTypeLoader;
typedef XftTypeLoader        TypeLoader;

/**
 * @b One needs to create a file loader to load the whole file.
 * 
 * A file loader contains information about all the types and type laoders,
 * that are required to load the file.
 * */
struct XftFileLoader {
    Size main_type_loader_index;

    TypeLoader* type_loaders;         /**< @b Vector of all type loaders. */
    Size        type_loader_count;    /**< @b Number of type loaders */
    Size        type_loader_capacity; /**< @b Maximum capacity of type loader vector */
};

PRIVATE FileLoader* file_loader_init (FileLoader* floader);
PRIVATE FileLoader* file_loader_deinit (FileLoader* floader);
PRIVATE TypeLoader* file_loader_create_new_type_loader (FileLoader* floader);


#endif // ANVIE_SOURCE_CROSSFILE_XFT_FILE_LOADER_H
