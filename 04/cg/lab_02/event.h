#ifndef EVENT_H
#define EVENT_H

#include "point.h"

typedef enum eventType {
    EMPTY,
    EVENT_MOVE,
    EVENT_ROTATE,
    EVENT_RESIZE,
} eventType;

typedef struct eventInfo {
    eventType type;
    union {
        struct {
            int xShift;
            int yShift;
        } move;
        struct {
            point base;
            int angle;
        } rotate;
        struct {
            point base;
            double xScale;
            double yScale;
        } resize;
    } info;
} eventInfo;

#endif // EVENT_H
