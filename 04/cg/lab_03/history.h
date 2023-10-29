#ifndef HISTORY_H
#define HISTORY_H

#include <vector>

#include "event.h"

class History
{
public:
    History();

    void addEvent(Event e);

    Event getLastEvent();
    void cancelLastEvent();

    bool empty();

private:
    std::vector<Event> events;
};

#endif // HISTORY_H
