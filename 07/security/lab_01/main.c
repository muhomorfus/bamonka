#include "enigma/enigma.h"
#include "file_io/file_io.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("invalid args count, must be 3\n");
        return EXIT_FAILURE;
    }

    enigma_t e = { 0 };
    if (e_read(&e)) {
        printf("e_read() failed\n");
        return EXIT_FAILURE;
    }

    e_print(&e);

    unsigned char* data;
    int size;
    if (f_read(argv[1], &data, &size)) {
        e_free(&e);
        printf("f_read() failed\n");
        return EXIT_FAILURE;
    }

    unsigned char* trans = e_encode(&e, data, size);

    if (f_write(argv[2], trans, size)) {
        e_free(&e);
        free(trans);
        printf("f_write() failed\n");
        return EXIT_FAILURE;
    }

    e_free(&e);
    free(trans);

    return EXIT_SUCCESS;
}
