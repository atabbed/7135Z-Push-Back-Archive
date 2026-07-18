#pragma once

#include "autoselector.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"


extern pros::adi::Pneumatics matchloader;
extern pros::adi::Pneumatics descore;

extern pros::MotorGroup leftSideDrive;
extern pros::MotorGroup rightSideDrive;

extern lemlib::Chassis chassis;
extern pros::Controller master;

namespace autoselector {

    class Selector {
        public:
            static void initialize();
            static void runAuto();
            static void autoDisplay();

        private:
            static int inputAuto;

            static void leftButton();
            static void rightButton();
    };

    class Routines {
        public:
            static void move_off_bar();
            static void mdsrc_prog_skills();
            static void mdsrc_quals_left();
            static void mdsrc_quals_right();
            static void mdsrc_quals_sawp();
            static void mdsrc_elims_left();
            static void mdsrc_elims_right();
    };

}