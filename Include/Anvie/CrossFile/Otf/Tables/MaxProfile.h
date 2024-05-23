/**
 * @file MaxProfile.h
 * @date Wed, 23rd May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_MAX_PROFILE_H
#define ANVIE_CROSSFILE_OTF_TABLES_MAX_PROFILE_H

#include <Anvie/Types.h>

typedef struct XfOtfMaxProfile {
    Uint16 major_version;
    Uint16 minor_version;
    Uint16 num_glyphs;
    Uint16 max_points;
    Uint16 max_contours;
    Uint16 max_composite_points;
    Uint16 max_composite_contours;
    Uint16 max_zones;
    Uint16 max_twilight_points;
    Uint16 max_storage;
    Uint16 max_function_defs;
    Uint16 max_instruction_defs;
    Uint16 max_stack_elements;
    Uint16 max_size_of_instructions;
    Uint16 max_component_elements;
    Uint16 max_component_depth;
} XfOtfMaxProfile;

XfOtfMaxProfile* xf_otf_max_profile_init (XfOtfMaxProfile* max_prof, Uint8* data);
XfOtfMaxProfile* xf_otf_max_profile_pprint (XfOtfMaxProfile* max_prof);

#endif // ANVIE_CROSSFILE_OTF_TABLES_MAX_PROFILE_H