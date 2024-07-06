#include <Anvie/Common.h>
#include <Anvie/CrossFile/Stream.h>
#include <Anvie/Types.h>

int main (int argc, char** argv) {
    RETURN_VALUE_IF (argc != 2 || !argv[1], EXIT_FAILURE, "USAGE: %s <file.xf>", argv[0]);

    IoStream* stream = io_stream_open_file (argv[1], False);

    Uint32 magic = 0;
    RETURN_VALUE_IF (
        !io_stream_read_be_u32 (stream, &magic),
        EXIT_FAILURE,
        "Failed to read magic bytes from Elf file.\n"
    );
    PRINT_ERR ("%x\n", magic);

    io_stream_close (stream);

    return EXIT_SUCCESS;
}
