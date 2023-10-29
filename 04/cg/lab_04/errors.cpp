//
// Created by muhomorfus on 4/12/22.
//

#include <QMessageBox>

#include "errors.h"

bool processError(Error err) {
    if (err == INVALID_COORDINATES_ERROR) {
        QMessageBox::warning(nullptr, "Параша", "Параша с вводом координат.");
        return false;
    }

    if (err == INVALID_NUMBER_ERROR) {
        QMessageBox::warning(nullptr, "Параша", "Параша с вводом количества.");
        return false;
    }

    if (err == INVALID_STEP_ERROR) {
        QMessageBox::warning(nullptr, "Параша", "Параша с вводом шага.");
        return false;
    }

    if (err == INVALID_RADIUS_ERROR) {
        QMessageBox::warning(nullptr, "Параша", "Параша с вводом радиуса.");
        return false;
    }

    if (err == UNKNOWN_ERROR) {
        QMessageBox::warning(nullptr, "Параша", "Параша.");
        return false;
    }

    return true;
}