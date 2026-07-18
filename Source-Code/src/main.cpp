#include "main.h"
#include "autoselector.h"
#include "distancereset.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "lemlib/pose.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "triplestate.h"

/*
	Hardware definitions
*/

/*
	motor brief:

	Front left motor: port 11 reversed
	Middle left motor: 14, reversed
	Rear left motor: 15, forward

	Front right motor: 20, forward
	Middle right motor: 17, forward
	Rear right motor: 19, reversed

*/

pros::MotorGroup leftSideDrive({-11,-14,15}, pros::v5::MotorGears::blue);
pros::MotorGroup rightSideDrive({20,19,-17}, pros::v5::MotorGears::blue);
pros::MotorGroup intakeMotors({9,-10}, pros::v5::MotorGears::blue);

pros::adi::Pneumatics topTripleState('A',false);
pros::adi::Pneumatics bottomTripleState('C',false);
TripleState tripleState(
	topTripleState, 
	bottomTripleState
);

pros::adi::Pneumatics matchloader('B',false);
pros::adi::Pneumatics descore('D', false);

pros::Imu nersh(8);
pros::Distance frontDistance(3); //PORT
pros::Distance rightDistance(4); //PORT

DistanceReset dsr(
	chassis,
	frontDistance,
	rightDistance,
	-0.5,
	5
);

pros::Controller master(pros::E_CONTROLLER_MASTER);
int leftY;
int rightX;

lemlib::Drivetrain diffDrive(
	&leftSideDrive,
	&rightSideDrive,
	9.75,
	lemlib::Omniwheel::NEW_325,
	450,
	2 //TUNE
);

lemlib::OdomSensors localizeSensors(
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&nersh
);

lemlib::ControllerSettings latty(
	10,
	0,
	3,
	0,
	.5,
	140,
	1,
	280,
	0
);

lemlib::ControllerSettings angy(
	6,
	0,
	12,
	0,
	.75,
	120,
	1.5,
	250,
	0
);

lemlib::Chassis chassis(
	diffDrive, //drivetrain params
	latty, //lateral PID controller tune
	angy, //angular PID controller tune
	localizeSensors //localization sensors
);

lemlib::Pose dtPose(0,0,0);

void printPose(void * ignore) {
	while (true) {
		dtPose = chassis.getPose();

		/*
			Telemetry for current lemlib pose

			Example display output:

			POSE: -16.12, 4.88, 269.91, 270.02
		*/
		pros::lcd::print(0,
			"POSE: %.2f, %.2f, %.2f, %.2f",
			dtPose.x, dtPose.y,
			dtPose.theta,
			nersh.get_heading()
		);

		//note gap in lines, this is for auto selector's lines.

		/*
			Telemetry for DSR class (shows anticipated pos if reset here)

			Example display output:

			DSR RAW  F:11.42  R:35.88
			DSR PRED X:-35.50 Y:5.02
		*/
		pros::lcd::print(
			4,
			"DSR RAW  F:%.2f  R:%.2f",
			frontDistance.get() / 25.4, //see dsr class for why div by 25.4
			rightDistance.get() / 25.4
		);

		pros::lcd::print(
			5,
			"DSR PRED X:%.2f Y:%.2f",
			dsr.resetRight(true),
			dsr.resetFront(true)
		);

		//delay so task doesn't bog down processor
		pros::delay(20);
	}
}

void initialize() {
	/*
		Callibrate IMU
	*/
	chassis.calibrate();

	/*
		Initialize auto selector LCD display
	*/
	autoselector::Selector::initialize();

	/*
		Spawn task to print current chassis pose to LCD display
	*/
	pros::task_t printPoseTask = pros::c::task_create(
		printPose, 
		(void*)"PROS", TASK_PRIORITY_MIN, 
		TASK_STACK_DEPTH_DEFAULT, 
		"printPoseTask"
	);

}

void disabled() {}

void competition_initialize() {
	/*
		Callibrate IMU
	*/
	chassis.calibrate();

	/*
		Initialize auto selector LCD display
	*/
	autoselector::Selector::initialize();

	/*
		Spawn task to print current chassis pose to LCD display
	*/
	pros::task_t printPoseTask = pros::c::task_create(
		printPose, 
		(void*)"PROS", TASK_PRIORITY_MIN, 
		TASK_STACK_DEPTH_DEFAULT, 
		"printPoseTask"
	);

}

void autonomous() {
	// autoselector::Selector::runAuto();

	//FIRST MID GOAL MOVE
	chassis.setPose(-48,0,90);
	chassis.moveToPoint(-35, 0, 1000);
	tripleState.setState("BALL_LOCK");
	intakeMotors.move(100);
	chassis.moveToPose(-27, 27, 0, 3000, {.maxSpeed=80});
	chassis.waitUntilDone();
	intakeMotors.brake();
	chassis.moveToPose(-11.5, 13.5, 315, 2000, {.forwards=false, .maxSpeed=80});
	chassis.waitUntilDone();
	tripleState.setState("MID_GOAL");
	intakeMotors.move(127);
	pros::delay(5000);
	tripleState.setState("BALL_LOCK");	
	chassis.moveToPoint(-33, 26, 2000);
	chassis.moveToPoint(-51,51,2000);
	chassis.turnToHeading(270, 700);
	chassis.waitUntilDone();
	chassis.tank(-100, -100);
	pros::delay(200);
	chassis.tank(0, 0);
	pros::delay(1000);
	intakeMotors.brake();

	chassis.setPose(-28,47,0);
	pros::delay(500); //FIXME
	matchloader.extend();


	//MOVE TO LOADER
	









}

void opcontrol() {
	chassis.setPose(-48,0,90);

	while (true) {
		/*
			Drivetrain Control (Split Arcade)
		*/
		leftY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		rightX = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		chassis.arcade(leftY, rightX);

		/*
			Controller Bindings
		*/
		
		//R1 
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
			tripleState.setState("BALL_LOCK");
			master.set_text(1,1,tripleState.getState());
			intakeMotors.move(90);
		} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R1)) {
			intakeMotors.brake();
		}

		//R2
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
			tripleState.setState("LONG_GOAL");
			master.set_text(1,1,tripleState.getState());
			intakeMotors.move(127);
		} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_R2)) {
			intakeMotors.brake();
		}

		//L1
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
			master.set_text(1,1,"OUTTAKE");
			intakeMotors.move(-127);
		} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_L1)) {
			intakeMotors.brake();
		}

		//L2
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
			descore.toggle();
		}

		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			tripleState.setState("MID_GOAL");
			master.set_text(1,1,tripleState.getState());
			intakeMotors.move(127);
		} else if (master.get_digital_new_release(pros::E_CONTROLLER_DIGITAL_A)) {
			intakeMotors.brake();
			tripleState.setState("LONG_GOAL");
		}

		//up/down buttons for matchloader
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
			matchloader.toggle();
		}
		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
			matchloader.extend();
		}

		pros::delay(20);

	}
}