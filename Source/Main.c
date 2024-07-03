#include <Anvie/Common.h>
#include <Anvie/CrossFile/Stream.h>
#include <Anvie/Types.h>

// /**
//  * @b Skip whitespace starting from given position.
//  *
//  * @param begin Begin iterator.
//  * @param end End iterator.
//  *
//  * @return Iterator to character after when first non
//  *         whitespace character is found on success.
//  * @return @c Null otherwise.
//  * */
// PRIVATE Char* skip_whitespace (Char* begin, Char* end) {
//     RETURN_VALUE_IF (!begin || !end || begin > end, Null, ERR_INVALID_ARGUMENTS);
//
//     /* this means we've completed parsing or there's some error */
//     if (begin == end) {
//         return end;
//     }
//
//     Char* iter = begin;
//     while (iter < begin) {
//         Char c = *iter;
//         if (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\b') {
//             break;
//         }
//     }
//
//     return iter;
// }

int main (int argc, char** argv) {
    RETURN_VALUE_IF (argc != 2 || !argv[1], EXIT_FAILURE, "USAGE: %s <file.xf>", argv[0]);

    XfDataStream* stream = xf_data_stream_open_file (argv[1]);

    TO_CharVec* str = xf_data_stream_read_seq_char (stream, xf_data_stream_get_size (stream));
    RETURN_VALUE_IF (!str, EXIT_FAILURE, "Failed to read from file\n");
    // TODO: printing the charvec directly shows error, check taht please
    anv_char_vec_destroy (str);

    xf_data_stream_close (stream);

    return EXIT_SUCCESS;
}
