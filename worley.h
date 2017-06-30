
#include <vector>
#include <random>
#include <memory>
#include <time.h>

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
    std::unique_ptr<std::vector<double>> getNearestPoints(double x,
                                                          double y,
                                                          unsigned int k);

    std::unique_ptr<std::vector<unsigned char>> toImage(size_t dim);
private:
    std::default_random_engine generator;

    
    std::vector<std::unique_ptr<Point>> points;
    
};


