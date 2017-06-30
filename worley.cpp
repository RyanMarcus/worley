
#include "worley.h"
#include "color_funcs.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "lodepng.h"
#include <chrono>

#define MAX_DIST (sqrt(2.0))

WorleyGrid::WorleyGrid() {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

void WorleyGrid::addRandomPoint() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::unique_ptr<Point> ptr(new Point());
    ptr->x = dist(generator);
    ptr->y = dist(generator);

    points.push_back(std::move(ptr));
}

void WorleyGrid::addRandomPoints(unsigned int n) {
    for (unsigned int i = 0; i < n; i++)
        addRandomPoint();
}

void WorleyGrid::resetPoints() {
    points.clear();
}


void WorleyGrid::getNearestPoints(double x, double y,
                                  std::vector<double>& outVal) {
    std::unique_ptr<std::vector<double>> toR(new std::vector<double>);

    // sort the points by their distance to (x, y)
    for (size_t i = 0; i < points.size(); i++) {
        double d = sqrt(pow(points[i]->x - x, 2)
                        + pow(points[i]->y - y, 2));
        outVal[i] = d;
    }
    
    std::sort(outVal.begin(), outVal.end());
   
}

template <typename WF>
std::unique_ptr<std::vector<unsigned char>>
WorleyGrid::toImage(size_t dim, WF& wf) {
    std::unique_ptr<std::vector<unsigned char>> toR(
        new std::vector<unsigned char>()
        );

    std::vector<double> dists(points.size());

    for (size_t y = 0; y < dim; y++) {
        double ly =  (double)y / (double)dim;
        for (size_t x = 0; x < dim; x++) {
            // RGBA channels
            double lx = (double)x / (double)dim;

            getNearestPoints(lx, ly, dists);

            unsigned int r = 0;
            unsigned int g = 0;
            unsigned int b = 0;
            unsigned int a = 0;
            wf.getPixelValues(dists, r, g, b, a);
            toR->push_back(std::min((unsigned int) 255, r));
            toR->push_back(std::min((unsigned int) 255, g));
            toR->push_back(std::min((unsigned int) 255, b));
            toR->push_back(std::min((unsigned int) 255, a));
        }
    }

    return toR;
}

int main(int argc, char** argv) {
    WorleyGrid wg;
    char buf[100];
    memset(buf, sizeof(char), 100);

    Icebergs wf;
    RandomLinearCombination wf2;
    EachChannel wf3;
    FourthDiff wf4;

    auto selected = wf4;
    
    for (int i = 0; i < 1; i++) {
        wg.resetPoints();
        wg.addRandomPoints(10);
        auto img = wg.toImage<decltype(selected)>(10000, selected);

        snprintf(buf, 100, "test%d.png", i);
        
        lodepng::encode(buf, *img, 10000, 10000);
    }
}
