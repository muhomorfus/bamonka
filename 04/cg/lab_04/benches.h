//
// Created by muhomorfus on 4/11/22.
//

#ifndef BENCHES_H
#define BENCHES_H

#include "task.h"

#define ITERATIONS 10

struct XY {
    int x;
    double y;
};

void bench(Object object, char *filename);

#endif //BENCHES_H
