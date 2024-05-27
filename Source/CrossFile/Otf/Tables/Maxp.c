/**
 * @file Maxp.c
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

#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Maxp.h>

/* libc */
#include <memory.h>

#define MAXP_VERSION_10 0x00010000
#define MAXP_VERSION_05 0x00005000

XfOtfMaxp* xf_otf_maxp_init (XfOtfMaxp* max_prof, Uint8* data, Size size) {
    RETURN_VALUE_IF (!max_prof || !data, Null, ERR_INVALID_ARGUMENTS);

    Uint32 version = GET_AND_ADV_U4 (data);

    if (version == MAXP_VERSION_10) {
        RETURN_VALUE_IF (
            size < XF_OTF_MAXP_VERSION_10_DATA_SIZE,
            Null,
            "Data buffer size not sufficient to initialize max profile table \"maxp\".\n"
        );

        max_prof->version                  = version;
        max_prof->num_glyphs               = GET_AND_ADV_U2 (data);
        max_prof->max_points               = GET_AND_ADV_U2 (data);
        max_prof->max_contours             = GET_AND_ADV_U2 (data);
        max_prof->max_composite_points     = GET_AND_ADV_U2 (data);
        max_prof->max_composite_contours   = GET_AND_ADV_U2 (data);
        max_prof->max_zones                = GET_AND_ADV_U2 (data);
        max_prof->max_twilight_points      = GET_AND_ADV_U2 (data);
        max_prof->max_storage              = GET_AND_ADV_U2 (data);
        max_prof->max_function_defs        = GET_AND_ADV_U2 (data);
        max_prof->max_instruction_defs     = GET_AND_ADV_U2 (data);
        max_prof->max_stack_elements       = GET_AND_ADV_U2 (data);
        max_prof->max_size_of_instructions = GET_AND_ADV_U2 (data);
        max_prof->max_component_elements   = GET_AND_ADV_U2 (data);
        max_prof->max_component_depth      = GET_AND_ADV_U2 (data);
    } else if (version == MAXP_VERSION_05) {
        RETURN_VALUE_IF (
            size < XF_OTF_MAXP_VERSION_05_DATA_SIZE,
            Null,
            "Data buffer size not sufficient to initialize max profile table \"maxp\".\n"
        );

        max_prof->version    = version;
        max_prof->num_glyphs = GET_AND_ADV_U2 (data);
    } else {
        PRINT_ERR ("Invalid version in Max Profile \"maxp\" table\n");
        return Null;
    }

    return max_prof;
}

XfOtfMaxp* xf_otf_maxp_pprint (XfOtfMaxp* max_prof, Uint8 indent_level) {
    RETURN_VALUE_IF (!max_prof, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    if (max_prof->version == MAXP_VERSION_10) {
        printf (
            "|%.*s|OTF Maximum Profile :\n"
            "|%s|version = %08x (%s)\n"
            "|%s|num_glyphs = %u\n"
            "|%s|max_points = %u\n"
            "|%s|max_contours = %u\n"
            "|%s|max_composite_points = %u\n"
            "|%s|max_composite_contours = %u\n"
            "|%s|max_zones = %u\n"
            "|%s|max_twilight_points = %u\n"
            "|%s|max_storage = %u\n"
            "|%s|max_function_defs = %u\n"
            "|%s|max_instruction_defs = %u\n"
            "|%s|max_stack_elements = %u\n"
            "|%s|max_size_of_instructions = %u\n"
            "|%s|max_component_elements = %u\n"
            "|%s|max_component_depth = %u\n",
            indent_level - 1 ? indent_level - 1 : 1,
            indent,
            indent,
            max_prof->version,
            "Version 1.0 (TTF)",
            indent,
            max_prof->num_glyphs,
            indent,
            max_prof->max_points,
            indent,
            max_prof->max_contours,
            indent,
            max_prof->max_composite_points,
            indent,
            max_prof->max_composite_contours,
            indent,
            max_prof->max_zones,
            indent,
            max_prof->max_twilight_points,
            indent,
            max_prof->max_storage,
            indent,
            max_prof->max_function_defs,
            indent,
            max_prof->max_instruction_defs,
            indent,
            max_prof->max_stack_elements,
            indent,
            max_prof->max_size_of_instructions,
            indent,
            max_prof->max_component_elements,
            indent,
            max_prof->max_component_depth
        );
    } else if (max_prof->version == MAXP_VERSION_05) {
        printf (
            "|%.*s|OTF Maximum Profile :\n"
            "|%s|version = %08x (%s)\n"
            "|%s|num_glyphs = %u\n",
            indent_level - 1 ? indent_level - 1 : 1,
            indent,
            indent,
            max_prof->version,
            "Version 0.5 (CFF/CFF2)",
            indent,
            max_prof->num_glyphs
        );
    } else {
        PRINT_ERR ("Invalid Max Profile version.\n");
        return Null;
    }

    return max_prof;
}
