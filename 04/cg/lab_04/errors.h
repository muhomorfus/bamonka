//
// Created by muhomorfus on 4/12/22.
//

#ifndef ERRORS_H
#define ERRORS_H

enum Error {
    OK,
    INVALID_RADIUS_ERROR,
    INVALID_STEP_ERROR,
    INVALID_NUMBER_ERROR,
    INVALID_COORDINATES_ERROR,
    UNKNOWN_ERROR,
};

bool processError(Error err);

#endif //ERRORS_H
