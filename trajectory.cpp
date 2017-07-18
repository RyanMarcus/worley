#include "trajectory.h"
#include <random>
#include "math.h"

Trajectory::Trajectory(double& sX, double& sY,
               std::default_random_engine& generator) {
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    sX = dist(generator);
    sY = dist(generator);

    startX = sX;
    startY = sY;
    
    endX = dist(generator);
    endY = dist(generator);
}

void Trajectory::getPosition(double t, double& x, double& y) {
    if (t == 0.0 || t == 1.0) {
        x = startX;
        y = startY;
        return;
    }

    double scaled = sin(3.1415 * t);
    x = startX + ((startX - endX) * scaled);
    y = startY + ((startY - endY) * scaled);
}
