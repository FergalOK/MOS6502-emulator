#include "clock.hpp"

#include "registers.hpp"

void Clock::run()
{
    for (auto sub : _subscribers) {
        if (sync) {
            _cycle = 0;
        }
        else {
            _cycle++;
        }
        sub->tick(_cycle);
    }
}

// TODO: pub/sub is overkill
void Clock::subscribe(ClockSubscriber *subscriber)
{
    _subscribers.push_back(subscriber);
}
