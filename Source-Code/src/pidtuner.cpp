#include "pidtuner.h"
#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/llemu.hpp"
#include <cstdio>

// -------- EXTERN OBJECTS FROM MAIN --------
extern pros::Controller master;

extern lemlib::Drivetrain diffDrive;
extern lemlib::OdomSensors localizeSensors;

extern lemlib::ControllerSettings latty;
extern lemlib::ControllerSettings angy;

extern lemlib::Chassis chassis;


// -------- PID VALUES (editing copy) --------
double kP = latty.kP;
double kI = latty.kI;
double kD = latty.kD;

int selected = 0;
double step = 0.1;


// -------- CHASSIS REBUILD --------
void rebuildChassis() {

    chassis.cancelMotion();

    lemlib::ControllerSettings newLatty(
        kP,
        kI,
        kD,
        0,
        0,
        0,
        0,
        0,
        0
    );

    chassis.~Chassis();

    new (&chassis) lemlib::Chassis(
        diffDrive,
        newLatty,
        angy,
        localizeSensors
    );
}


// -------- LCD DISPLAY (LINES 6-7 ONLY) --------
void updateDisplay() {

    pros::lcd::clear_line(6);
    pros::lcd::clear_line(7);

    const char* name =
        selected == 0 ? "kP" :
        selected == 1 ? "kI" : "kD";

    double value =
        selected == 0 ? kP :
        selected == 1 ? kI : kD;

    char line6[32];
    char line7[32];

    sprintf(line6, "Tune %s: %.3f", name, value);
    sprintf(line7, "P %.2f I %.2f D %.2f", kP, kI, kD);

    pros::lcd::set_text(6, line6);
    pros::lcd::set_text(7, line7);
}


// -------- TEST ROUTINES --------
void runAngularTest() {
    chassis.cancelAllMotions();
    chassis.setPose(0,0,0);
    chassis.turnToHeading(90, 10000);
}

void runLateralTest() {
    chassis.cancelAllMotions();
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0,20,10000);
}


// -------- CONTROLLER INPUT --------
void pidTunerLoop() {

    // change constant
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        selected++;
        if(selected > 2) selected = 0;
        updateDisplay();
    }

    // increase
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){

        if(selected == 0) kP += step;
        if(selected == 1) kI += step;
        if(selected == 2) kD += step;

        rebuildChassis();
        updateDisplay();
    }

    // decrease
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){

        if(selected == 0) kP -= step;
        if(selected == 1) kI -= step;
        if(selected == 2) kD -= step;

        rebuildChassis();
        updateDisplay();
    }

    // angular PID test
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
        runAngularTest();
    }

    // lateral PID test
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){
        runLateralTest();
    }

    //stop motion so aidan doesnt lose it
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
        chassis.cancelAllMotions();
    }
}


// -------- INIT --------
void pidTunerInit() {
    pros::lcd::initialize();

    // load current PID from your real settings
    kP = latty.kP;
    kI = latty.kI;
    kD = latty.kD;

    updateDisplay();
}