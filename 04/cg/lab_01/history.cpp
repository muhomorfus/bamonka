#include "history.h"

history::history() {

}

void history::addEvent(enum event e) {
    events.push_back(e);
}

enum event history::getLastEvent() {
    if (events.empty())
        return EVENT_EMPTY;

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
