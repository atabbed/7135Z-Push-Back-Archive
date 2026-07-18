#include "liblvgl/llemu.hpp"
#include "main.h" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include <cstdio>
#include <string>
#include "autoselector.h"
using namespace autoselector;

int Selector::inputAuto = 0;

void Selector::leftButton() {
    inputAuto--;
    autoDisplay();
    pros::lcd::set_text(1, std::to_string(inputAuto));
}

void Selector::rightButton() {
    inputAuto++;
    autoDisplay();
    pros::lcd::set_text(1, std::to_string(inputAuto));
}

void Selector::initialize() {
    pros::lcd::initialize();
    pros::lcd::register_btn0_cb(leftButton);
    pros::lcd::register_btn2_cb(rightButton);
    pros::lcd::set_text(1, std::to_string(inputAuto));

}

void Selector::autoDisplay() {
    switch (inputAuto) {
        //default option for if input is negative or past all autos: use this to disable auto if needed
        default:
            pros::lcd::set_text(2, "auto disabled: selected routine out of range");
            break;
        //match autos
        case 0:
            pros::lcd::set_text(2, "move off park for partner sawp");
            break;
        case 1:
            pros::lcd::set_text(2, "left side 4 + 3 quals auto");
            break;
        case 2:
            pros::lcd::set_text(2, "right side 4 + 3 quals");
            break;
        case 3:
            pros::lcd::set_text(2, "4 + 3 + 3 + 4 sig sawp for mdsrc");
            break;
        case 4:
            pros::lcd::set_text(2, "left side 7 block + push mdsrc elims");
            break;
        case 5:
            pros::lcd::set_text(2, "right side 7 block + push mdsrc elims");
            break;
        //skills autos
        case 7:
            pros::lcd::set_text(2, "mdsrc auto skills");
            break;
    }
}

void Selector::runAuto() {

    switch (inputAuto) {
        //default option for if input is negative or past all autos: use this to disable auto if needed
        default:
            pros::lcd::set_text(2, "auto disabled: selected routine out of range");
            break;
        //match autos
        case 0:
            Routines::move_off_bar();
            break;
        case 1:
            Routines::mdsrc_quals_left();
            break;
        case 2:
            Routines::mdsrc_quals_right();
            break;
        case 3:
            Routines::mdsrc_quals_sawp();
            break;
        case 4:
            Routines::mdsrc_elims_left();
            break;
        case 5:
            Routines::mdsrc_elims_right();
            break;
        //skills autos
        case 7:
            Routines::mdsrc_prog_skills();
            break;
    }

    master.set_text(2,1,("EXEC AUTO " + std::to_string(inputAuto)));
}

void Routines::mdsrc_prog_skills() {
    pros::lcd::set_text(2, "mdsrc_prog_skills");

    chassis.setPose(-48,0,90);

    
    chassis.moveToPose(-23, 23, 45, 2000);

};

void Routines::move_off_bar() {
    //move off bar default routine (NO PID, SHOULD BE SOMEWHAT FOOLPROOF)
    pros::lcd::set_text(2, "move_off_bar");
    leftSideDrive.move(60);
    rightSideDrive.move(60);
    pros::delay(500);
    leftSideDrive.move(0);
    rightSideDrive.move(0);
}

void Routines::mdsrc_quals_left() {
    pros::lcd::set_text(2, "mdsrc_quals_left");
}

void Routines::mdsrc_quals_right() {
    pros::lcd::set_text(2, "mdsrc_quals_right");
}

void Routines::mdsrc_quals_sawp() {
    pros::lcd::set_text(2, "mdsrc_quals_sawp");

}

void Routines::mdsrc_elims_left() {
    pros::lcd::set_text(2, "mdsrc_elims_left");

}

void Routines::mdsrc_elims_right() {
    pros::lcd::set_text(2, "mdsrc_elims_right");

}