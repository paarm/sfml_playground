#pragma once
#include <SFML/System.hpp>

class Clock {
private:
    sf::Clock clock;
    sf::Time mDelta;

public:
    Clock() {
        tick();
    }

    void tick() {
        mDelta=clock.restart();
    }
    float getDelta() {
        return (float)mDelta.asMilliseconds();
    }
};
