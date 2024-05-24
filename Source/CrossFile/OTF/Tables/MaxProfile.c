/**
 * @file MaxProfile.c
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
#include <Anvie/CrossFile/Otf/Tables/MaxProfile.h>

#define MAXP_VERSION_10 0x00010000
#define MAXP_VERSION_05 0x00005000

XfOtfMaxProfile* xf_otf_max_profile_init (XfOtfMaxProfile* max_prof, Uint8* data, Size size) {
    RETURN_VALUE_IF (!max_prof || !data, Null, ERR_INVALID_ARGUMENTS);

    Uint32 version = GET_AND_ADV_U4 (data);

    if (version == MAXP_VERSION_10) {
        RETURN_VALUE_IF (
            size < XF_OTF_MAX_PROFILE_VERSION_10_DATA_SIZE,
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
            size < XF_OTF_MAX_PROFILE_VERSION_05_DATA_SIZE,
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

XfOtfMaxProfile* xf_otf_max_profile_pprint (XfOtfMaxProfile* max_prof) {
    RETURN_VALUE_IF (!max_prof, Null, ERR_INVALID_ARGUMENTS);

    if (max_prof->version == MAXP_VERSION_10) {
        printf (
            "OTF Maximum Profile :\n"
            "\tversion = %08x (%s)\n"
            "\tnum_glyphs = %u\n"
            "\tmax_points = %u\n"
            "\tmax_contours = %u\n"
            "\tmax_composite_points = %u\n"
            "\tmax_composite_contours = %u\n"
            "\tmax_zones = %u\n"
            "\tmax_twilight_points = %u\n"
            "\tmax_storage = %u\n"
            "\tmax_function_defs = %u\n"
            "\tmax_instruction_defs = %u\n"
            "\tmax_stack_elements = %u\n"
            "\tmax_size_of_instructions = %u\n"
            "\tmax_component_elements = %u\n"
            "\tmax_component_depth = %u\n",
            max_prof->version,
            "Version 1.0 (TTF)",
            max_prof->num_glyphs,
            max_prof->max_points,
            max_prof->max_contours,
            max_prof->max_composite_points,
            max_prof->max_composite_contours,
            max_prof->max_zones,
            max_prof->max_twilight_points,
            max_prof->max_storage,
            max_prof->max_function_defs,
            max_prof->max_instruction_defs,
            max_prof->max_stack_elements,
            max_prof->max_size_of_instructions,
            max_prof->max_component_elements,
            max_prof->max_component_depth
        );
    } else if (max_prof->version == MAXP_VERSION_05) {
        printf (
            "OTF Maximum Profile :\n"
            "\tversion = %08x (%s)\n"
            "\tnum_glyphs = %u\n",
            max_prof->version,
            "Version 0.5 (CFF/CFF2)",
            max_prof->num_glyphs
        );
    } else {
        PRINT_ERR ("Invalid Max Profile version.\n");
        return Null;
    }

    return max_prof;
}
