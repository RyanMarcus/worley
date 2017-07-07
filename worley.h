// < begin copyright > 
// Copyright Ryan Marcus 2017
// 
// This file is part of worley.
// 
// worley is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// worley is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with worley.  If not, see <http://www.gnu.org/licenses/>.
// 
// < end copyright > 

#include <vector>
#include <random>
#include <memory>
#include <time.h>
#include "color_funcs.h"
#include "trajectory.h"

struct Point {
    double x;
    double y;
};


class WorleyGrid {
public:
    WorleyGrid();

    void addRandomPoint();
    void addRandomPoints(unsigned int n);

    void resetPoints();
    void getNearestPoints(double x, double y,
                          std::vector<double>& dists,
                          std::vector<unsigned int>& pointIDs, 
                          size_t numPoints);

    void createTrajectories();
    void setTime(double t);

    std::unique_ptr<std::vector<unsigned char>> toImage(size_t width,
                                                        size_t height,
                                                        ColorFunc& wf);
    
private:
    std::default_random_engine generator;

    
    std::vector<std::unique_ptr<Point>> points;
    std::vector<std::unique_ptr<Trajectory>> trajectories;
    
};


