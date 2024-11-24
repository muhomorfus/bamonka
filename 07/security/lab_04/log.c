#include <stdio.h>

#include "log.h"

void Errorln(char *msg) {
    printf("[ERROR] %s\n", msg);
}

void Infoln(char *msg) {
    printf("[INFO] %s\n", msg);
}

void Info(char *msg) {
    printf("[INFO] %s", msg);
}

void NewLine() {
    printf("\n");
}
