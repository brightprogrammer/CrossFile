/**
 * @file Hmtx.h
 * @date Sun, 26th May 2024
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

#ifndef ANVIE_CROSSFILE_OTF_TABLES_HMTX_H
#define ANVIE_CROSSFILE_OTF_TABLES_HMTX_H

#include <Anvie/Types.h>

/* fwd-declarations */
typedef struct XfOtfHhea XfOtfHhea;
typedef struct XfOtfMaxp XfOtfMaxp;

/* REF : https://learn.microsoft.com/en-us/typography/opentype/spec/hmtx */

typedef struct XfOtfHmtxLongHorMetric {
    Uint16 advance_width;
    Int16  left_side_bearing;
} XfOtfHmtxLongHorMetric;

typedef struct XfOtfHmtx {
    /**
     * @b Custom added field, not in binary file. Dedcued from Hhea table. 
     * */
    Uint16 num_h_metrics;

    XfOtfHmtxLongHorMetric *h_metrics; /**< @b Array of horizontal metrics present in binary file */

    /**
     * @b Custom added field, not in binary file. Deduced from Maxp and Hhea tables.
     * */
    Uint16 num_left_side_bearings;

    /**
     * @b Array of left side bearings for glyph ids greater than or equal to
     * number of horizontal metrics (num_h_metrics).
     * This is present in binary file.
     * */
    Int16 *left_side_bearings;
} XfOtfHmtx;

XfOtfHmtx *
    xf_otf_hmtx_init (XfOtfHmtx *hmtx, XfOtfHhea *hhea, XfOtfMaxp *maxp, Uint8 *data, Size size);
XfOtfHmtx *xf_otf_hmtx_deinit (XfOtfHmtx *hmtx);
XfOtfHmtx *xf_otf_hmtx_pprint (XfOtfHmtx *hmtx, Uint8 indent_level);

#endif // ANVIE_CROSSFILE_OTF_TABLES_HMTX_H
