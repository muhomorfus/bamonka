#ifndef HISTORY_H
#define HISTORY_H

#include <vector>

enum event {
    EVENT_EMPTY,
    EVENT_ADD_POINT,
    EVENT_DELETE_POINT,
};

class history
{
public:
    history();

    void addEvent(enum event e);

    enum event getLastEvent();
    void cancelLastEvent();

    bool empty();

private:
    std::vector<enum event> events;
};

#endif // HISTORY_H
