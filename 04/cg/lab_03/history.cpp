#include "history.h"

History::History() {

}

void History::addEvent(Event e) {
    events.push_back(e);
}

Event History::getLastEvent() {
    if (events.empty())
        return EMPTY;

    return events.back();
}

void History::cancelLastEvent() {
    if (events.empty())
        return;

    events.pop_back();
}

bool History::empty() {
    return events.empty();
}
