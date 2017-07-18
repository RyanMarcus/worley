#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <random>

class Trajectory {
public:
    Trajectory(double& startX, double& startY,
               std::default_random_engine& generator);


    void getPosition(double time, double& x, double& y);
private:

    double startX, startY, endX, endY;
};

#endif
