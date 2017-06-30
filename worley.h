
#include <vector>
#include <random>
#include <memory>
#include <time.h>
#include "color_funcs.h"

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
                          std::vector<double>& outVal);

    std::unique_ptr<std::vector<unsigned char>> toImage(size_t width,
                                                        size_t height,
                                                        ColorFunc& wf);
    
private:
    std::default_random_engine generator;

    
    std::vector<std::unique_ptr<Point>> points;
    
};


