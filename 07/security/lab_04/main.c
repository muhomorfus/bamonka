#include <stdio.h>
#include <string.h>

#include "log.h"
#include "crypto.h"

int genCmd(int argc, char **argv) {
    if (argc != 3) {
        Errorln("need 3 arguments");
        return -1;
    }

    if (MakeKeyPair(argv[2])) {
        Errorln("cant gen rsa key pair");
        return -1;
    }

    return 0;
}

int signCmd(int argc, char **argv) {
    if (argc != 4) {
        Errorln("need 4 arguments");
        return -1;
    }

    if (Sign(argv[3], argv[2])) {
        return -1;
    }

    return 0;
}

int checkCmd(int argc, char **argv) {
    if (argc != 4) {
        printf("Invalid number of arguments, must be 4.\n");
        return -1;
    }

    if (Check(argv[3], argv[2])) {
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        Errorln("need 2 arguments");
        return -1;
    }

    if (strcmp(argv[1], "gen") == 0) {
        return genCmd(argc, argv);
    }

    if (strcmp(argv[1], "sign") == 0) {
        return signCmd(argc, argv);
    }

    if (strcmp(argv[1], "check") == 0) {
        return checkCmd(argc, argv);
    }

    Errorln("invalid command");
    return -1;
}
