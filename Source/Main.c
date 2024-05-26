#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Otf/Otf.h>

// #define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceBold-zmP0.ttf"
// #define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceOblique-5meB.ttf"
#define TTF_FONT_PATH "/home/misra/Documents/Olivia-Regular.otf"

Int32 main (int argc, char** argv) {
    RETURN_VALUE_IF (argc < 2, EXIT_FAILURE, "USAGE : %s <ttf/otf font file path>\n", argv[0]);

    XfOtfFile otf_file;
    CString   font_path = argv[1];

    GOTO_HANDLER_IF (
        !xf_otf_file_open (&otf_file, font_path),
        SKIP_FONT,
        "Failed to open OTF file \"%s\"\n",
        font_path
    );

    xf_otf_file_pprint (&otf_file, 0);

    for (Size m = 0; m < otf_file.cmap.num_tables; m++) {
        if (otf_file.cmap.encoding_records[m].sub_table.format == 2) {
            PRINT_ERR ("ENCODING RECORD SUB TABLE FORMAT 2 FOUND!\n");
        }
    }

SKIP_FONT:
    xf_otf_file_close (&otf_file);

    return EXIT_SUCCESS;
}
