#include "history.h"
#include "point.h"

history::history() {

}

void history::addEvent(eventInfo e) {
    events.push_back(e);
}

eventInfo history::getLastEvent() {
    if (events.empty())
        return { .type = EMPTY };

    return events.back();
}

void history::cancelLastEvent() {
    if (events.empty())
        return;

    events.pop_back();
}

bool history::empty() {
    return events.empty();
}

void history::addMove(int xShift, int yShift) {
    eventInfo e = { .type = EVENT_MOVE };
    e.info.move.xShift = xShift;
    e.info.move.yShift = yShift;

    addEvent(e);
}

void history::addRotate(point base, int angle) {
    eventInfo e = { .type = EVENT_ROTATE };
    e.info.rotate.base = base;
    e.info.rotate.angle = angle;

    addEvent(e);
}

void history::addResize(point base, double xScale, double yScale) {
    eventInfo e = { .type = EVENT_RESIZE };
    e.info.resize.base = base;
    e.info.resize.xScale = xScale;
    e.info.resize.yScale = yScale;

    addEvent(e);
}
