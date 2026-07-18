#include "distancereset.h"

/*
    7135Z Distance Sensor Reset (DSR) Implementation

    Heavily inspired by Richard Wang's "RW-Template" impl, credit to him.
    https://github.com/richardbwang/RW-Template/blob/main/src/motor-control.cpp

    Adapted for use with PROS & Lemlib stack
*/

DistanceReset::DistanceReset(lemlib::Chassis& chassis,
                             pros::Distance& frontSensor,
                             pros::Distance& rightSensor,
                             double frontOffset,
                             double rightOffset)
: chassis(chassis),
  frontSensor(frontSensor),
  rightSensor(rightSensor),
  frontOffset(frontOffset),
  rightOffset(rightOffset) {}


double DistanceReset::resetFront(bool telemetryOverride) {

    //pros::Distance returns mm, convert to inches
    double dist = frontSensor.get() / 25.4;

    //reject invalid readings
    if (dist < 0 || dist > 70) return NAN;

    lemlib::Pose pose = chassis.getPose();

    double wallToCenter = dist + frontOffset;
    double newY;

    if (pose.theta > 180) {
        newY = -FIELD_HALF + wallToCenter;
    } else {
        newY = FIELD_HALF - wallToCenter;
    }

    if (!telemetryOverride) {
        chassis.setPose(pose.x, newY, pose.theta);
    }

    return newY;
}


double DistanceReset::resetRight(bool telemetryOverride) {

    double dist = rightSensor.get() / 25.4;

    if (dist < 0 || dist > 70) return NAN;

    lemlib::Pose pose = chassis.getPose();

    double wallToCenter = dist + rightOffset;
    double newX;

    if (pose.theta > 90 && pose.theta < 270) {
        newX = -FIELD_HALF + wallToCenter;
    } else {
        newX = FIELD_HALF - wallToCenter;
    }

    if (!telemetryOverride) {
        chassis.setPose(newX, pose.y, pose.theta);
    }

    return newX;
}