/**
 * @file Loca.c
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

#include <Anvie/Common.h>
#include <Anvie/CrossFile/Otf/Tables/Head.h>
#include <Anvie/CrossFile/Otf/Tables/Loca.h>
#include <Anvie/CrossFile/Otf/Tables/Maxp.h>

XfOtfLoca *
    xf_otf_loca_init (XfOtfLoca *loca, XfOtfHead *head, XfOtfMaxp *maxp, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!loca || !head || !maxp || !data, Null, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF (
        size < XF_OTF_LOCA_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize index to location table \"loca\".\n"
    );

    return loca;
}

XfOtfLoca *xf_otf_loca_pprint (XfOtfLoca *loca) {
    RETURN_VALUE_IF (!loca, Null, ERR_INVALID_ARGUMENTS);

    return loca;
}
