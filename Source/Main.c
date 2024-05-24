#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Otf/Otf.h>

#define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceBold-zmP0.ttf"
// #define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceOblique-5meB.ttf"
// #define TTF_FONT_PATH "/home/misra/Documents/Olivia-Regular.otf"

Int32 main() {
    XfOtfFile otf_file;
    GOTO_HANDLER_IF (
        !xf_otf_file_open (&otf_file, TTF_FONT_PATH),
        INIT_FAILED,
        "Failed to open OTF file\n"
    );
    PRINT_ERR ("%zu\n", sizeof (XfOtfHead));

    xf_otf_file_pprint (&otf_file);

    xf_otf_file_close (&otf_file);
    return EXIT_SUCCESS;

INIT_FAILED:
    xf_otf_file_close (&otf_file);
    return EXIT_FAILURE;
}
