/**
 * @file IndexToLocation.h
 * @date Wed, 22nd May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_INDEX_TO_LOCATION_H
#define ANVIE_CROSSFILE_OTF_TABLES_INDEX_TO_LOCATION_H

#include <Anvie/Common.h>
#include <Anvie/Types.h>

/**
 * @b Glyphs index into this table to get offset to their glyph
 * data.
 *
 * REF : https://learn.microsoft.com/en-us/typography/opentype/spec/loca
 * */
typedef struct XfOtfIndexToLocation {
    Uint64 num_glyphs; /**< @b Value copied from @c XfOtfMaxProfile struct. */

    /**
     * @b @c True if "loca" stores long version offsets. @c False otherwise. 
     * */
    Bool is_long_version;

    union {
        Uint16 *short_version; /**< @b Offsets divided by two is stored. */
        Uint32 *long_version;  /**< @b Actual offset is stored. */
    } offsets;
} XfOtfIndexToLocation;

XfOtfIndexToLocation *xf_otf_index_to_location_init (XfOtfIndexToLocation *loca, Uint8 *data);
XfOtfIndexToLocation *xf_otf_index_to_location_pprint (XfOtfIndexToLocation *loca);

#endif // ANVIE_CROSSFILE_OTF_TABLES_INDEX_TO_LOCATION_H
