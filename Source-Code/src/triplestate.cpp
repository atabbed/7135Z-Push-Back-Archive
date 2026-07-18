#include "triplestate.h"
#include "pros/adi.hpp"
#include <string>

/*
    TripleState constructor definition

    Had to learn how to use the "initializer list" in C++ for this, as
    this isn't (quite) the same thing as the whole this.instance = param
    syntax from Java.

    Source:
    https://www.geeksforgeeks.org/cpp/when-do-we-use-initializer-list-in-c/

*/
TripleState::TripleState(
    pros::adi::Pneumatics& topPiston,
    pros::adi::Pneumatics& bottomPiston
) : topPiston(topPiston),
    bottomPiston(bottomPiston),
    currentState("LONG_GOAL")
{}

std::string TripleState::getState() {
    return this->currentState;
};

void TripleState::setState(std::string newState) {
    if (newState == "LONG_GOAL") {
        this->topPiston.retract();
        this->bottomPiston.retract();
    } else if (newState == "BALL_LOCK") {
        this->topPiston.extend();
        this->bottomPiston.retract();
    } else if (newState == "MID_GOAL") {
        this->topPiston.extend();
        this->bottomPiston.extend();
    }
};