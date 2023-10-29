#ifndef HISTORY_H
#define HISTORY_H

#include <vector>

#include "event.h"
#include "point.h"

class history
{
public:
    history();

    void addEvent(eventInfo e);
    void addMove(int xShift, int yShift);
    void addRotate(point base, int angle);
    void addResize(point base, double xScale, double yScale);

    eventInfo getLastEvent();
    void cancelLastEvent();

    bool empty();

private:
    std::vector<eventInfo> events;
};

#endif // HISTORY_H
