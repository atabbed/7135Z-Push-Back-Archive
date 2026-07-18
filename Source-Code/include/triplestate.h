#pragma once

#include "pros/adi.hpp"
#include <string>

class TripleState {
    private:
        pros::adi::Pneumatics& topPiston;
        pros::adi::Pneumatics& bottomPiston;
        
        std::string currentState;

        
    public:


        /*
            Constructor for Triple State Class
            Takes REFERENCES of the two pistons
            (as denoted by the "&", like how it
            naturally behaves when I do this in
            Java.

            @param Pneumatics topPiston the piston which points up
            @param Pneumatics bottomPiston the piston which points down
        */
        TripleState(
            pros::adi::Pneumatics& topPiston,
            pros::adi::Pneumatics& bottomPiston
        );

        std::string getState(); 

        void setState(std::string newState);

};