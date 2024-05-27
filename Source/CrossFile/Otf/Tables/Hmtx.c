#include <Anvie/Common.h>

/* crossfile */
#include <Anvie/CrossFile/EndiannessHelpers.h>
#include <Anvie/CrossFile/Otf/Tables/Hhea.h>
#include <Anvie/CrossFile/Otf/Tables/Hmtx.h>
#include <Anvie/CrossFile/Otf/Tables/Maxp.h>

/* libc */
#include <memory.h>

/* private method declarations */

static inline XfOtfHmtxLongHorMetric *
    long_hor_metric_init (XfOtfHmtxLongHorMetric *lhm, Uint8 *data, Size size);
static inline XfOtfHmtxLongHorMetric *
    long_hor_metric_pprint (XfOtfHmtxLongHorMetric *lhm, Uint8 indent_leve);

#define LONG_HOR_METRIC_DATA_SIZE (sizeof (Uint16) * 2)

#define PPRINT_ARR_I2(arr, last_idx)                                                               \
    putchar ('[');                                                                                 \
    for (Int64 s = 0; s < MIN (6, last_idx); s++) {                                                \
        printf ("%d, ", arr[s]);                                                                   \
    }                                                                                              \
    printf ("...]\n");

/**************************************************************************************************/
/*********************************** PUBLIC METHOD DEFINITIONS ************************************/
/**************************************************************************************************/

/**
 * @b Initialize given Hmtx table object from given data buffer.
 *
 * @param hmtx Hmtx table object to be initialized.
 * @param hhea Required for getting number of horizontal metrics.
 * @param maxp Required for getting number of glyphs in the font file.
 * @param data Data buffer containing data to be used to initialize hmtx table.
 * @param size Size in bytes of given data buffer.
 *
 * @return @c hmtx on success.
 * @return @c Null otherwise.
 * */
XfOtfHmtx *
    xf_otf_hmtx_init (XfOtfHmtx *hmtx, XfOtfHhea *hhea, XfOtfMaxp *maxp, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!hmtx || !hhea || !maxp || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        maxp->num_glyphs < hhea->number_of_h_metrics,
        Null,
        "Number of glyphs in maxp profile table cannot be less than number of horizontal metrics "
        "in horizontal header table\n"
    );
    hmtx->num_left_side_bearings = maxp->num_glyphs - hhea->number_of_h_metrics;

    hmtx->num_h_metrics = hhea->number_of_h_metrics;

    if (hmtx->num_h_metrics) {
        RETURN_VALUE_IF (
            size < LONG_HOR_METRIC_DATA_SIZE * hmtx->num_h_metrics,
            Null,
            "Data buffer size not sufficient to initialize hmtx table\n"
        );

        RETURN_VALUE_IF (
            !(hmtx->h_metrics = ALLOCATE (XfOtfHmtxLongHorMetric, hmtx->num_h_metrics)),
            Null,
            ERR_OUT_OF_MEMORY
        );

        for (Size s = 0; s < hmtx->num_h_metrics; s++) {
            GOTO_HANDLER_IF (
                !long_hor_metric_init (hmtx->h_metrics + s, data, size),
                INIT_FAILED,
                "Failed to read a long horizontal metric entry in hhea table\n"
            );

            data += LONG_HOR_METRIC_DATA_SIZE;
            size -= LONG_HOR_METRIC_DATA_SIZE;
        }
    }

    if (hmtx->num_left_side_bearings) {
        GOTO_HANDLER_IF (
            size < sizeof (Int16) * hmtx->num_left_side_bearings,
            INIT_FAILED,
            "Data buffer size not sufficient to initialize hmtx table\n"
        );

        GOTO_HANDLER_IF (
            !(hmtx->left_side_bearings = ALLOCATE (Int16, hmtx->num_left_side_bearings)),
            INIT_FAILED,
            ERR_OUT_OF_MEMORY
        );

        GET_ARR_AND_ADV_I2 (hmtx->left_side_bearings, 0, hmtx->num_left_side_bearings);
    }

    return hmtx;

INIT_FAILED:
    xf_otf_hmtx_deinit (hmtx);
    return Null;
}

/**
 * @b De-initialize the contents of given @c XfOtfHmtx table object.
 *
 * @param hmtx Hmtx table object to be de-initialized.
 * 
 * @return @c hmtx on success.
 * @return @c Null otherwise.
 * */
XfOtfHmtx *xf_otf_hmtx_deinit (XfOtfHmtx *hmtx) {
    RETURN_VALUE_IF (!hmtx, Null, ERR_INVALID_ARGUMENTS);

    if (hmtx->left_side_bearings) {
        FREE (hmtx->left_side_bearings);
    }

    if (hmtx->h_metrics) {
        FREE (hmtx->h_metrics);
    }

    memset (hmtx, 0, sizeof (XfOtfHmtx));

    return hmtx;
}

XfOtfHmtx *xf_otf_hmtx_pprint (XfOtfHmtx *hmtx, Uint8 indent_level) {
    RETURN_VALUE_IF (!hmtx, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Horizontal Metrics Table (Hmtx):\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent
    );

    for (Size s = 0; s < MIN (hmtx->num_h_metrics, 6); s++) {
        long_hor_metric_pprint (hmtx->h_metrics + s, indent_level + 1);
    }

    if (hmtx->num_h_metrics > 6) {
        printf (
            "|%s|.\n"
            "|%s|. (probably many more entries like this here)\n"
            "|%s|.\n",
            indent,
            indent,
            indent
        );

        long_hor_metric_pprint (hmtx->h_metrics + hmtx->num_h_metrics - 1, indent_level + 1);
    }

    printf ("|%s|left_side_bearings = ", indent);
    PPRINT_ARR_I2 (hmtx->left_side_bearings, hmtx->num_left_side_bearings - 1);

    return hmtx;
}

/**************************************************************************************************/
/*********************************** PRIVATE METHOD DEFINITIONS ***********************************/
/**************************************************************************************************/

static inline XfOtfHmtxLongHorMetric *
    long_hor_metric_init (XfOtfHmtxLongHorMetric *lhm, Uint8 *data, Size size) {
    RETURN_VALUE_IF (!lhm || !data, Null, ERR_INVALID_ARGUMENTS);

    RETURN_VALUE_IF (
        size < LONG_HOR_METRIC_DATA_SIZE,
        Null,
        "Data buffer size not sufficient to initialize long horizontal metric.\n"
    );

    lhm->advance_width     = GET_AND_ADV_U2 (data);
    lhm->left_side_bearing = GET_AND_ADV_I2 (data);

    return lhm;
}

static inline XfOtfHmtxLongHorMetric *
    long_hor_metric_pprint (XfOtfHmtxLongHorMetric *lhm, Uint8 indent_level) {
    RETURN_VALUE_IF (!lhm, Null, ERR_INVALID_ARGUMENTS);

    Char indent[indent_level + 1];
    memset (indent, '\t', indent_level);
    indent[indent_level] = 0;

    printf (
        "|%.*s|OTF Hmtx Long Horizontal Metric :\n"
        "|%s|advance_width = %u\n"
        "|%s|left_side_bearing = %d\n",
        indent_level - 1 ? indent_level - 1 : 1,
        indent,
        indent,
        lhm->advance_width,
        indent,
        lhm->left_side_bearing
    );

    return lhm;
}
