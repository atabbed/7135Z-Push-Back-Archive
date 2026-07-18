#pragma once

#include "pros/distance.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep

class DistanceReset {
public:
    DistanceReset(lemlib::Chassis& chassis,
                  pros::Distance& frontSensor,
                  pros::Distance& rightSensor,
                  double frontOffset,
                  double rightOffset);

    double resetFront(bool telemetryOverride = false);
    double resetRight(bool telemetryOverride = false);

private:
    lemlib::Chassis& chassis;

    pros::Distance& frontSensor;
    pros::Distance& rightSensor;

    double frontOffset;
    double rightOffset;

    const double FIELD_HALF = 72.0;
};