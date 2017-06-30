
#include "worley.h"
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

std::unique_ptr<std::vector<double>>
WorleyGrid::getNearestPoints(double x, double y, unsigned int k) {
    std::unique_ptr<std::vector<double>> toR(new std::vector<double>);

    // sort the points by their distance to (x, y)

    for (auto& p : points) {
        double d = sqrt(pow(p->x - x, 2) + pow(p->y - y, 2));
        toR->push_back(d);
    }
    
    std::sort(toR->begin(), toR->end());
    
    if (k < toR->size())
        toR->resize(k);

    return toR;
}

std::unique_ptr<std::vector<unsigned char>> WorleyGrid::toImage(size_t dim) {
    std::unique_ptr<std::vector<unsigned char>> toR(
        new std::vector<unsigned char>()
        );

    std::uniform_real_distribution<double> dist(-1.0, 3.0);
    double a = dist(generator);
    double b = dist(generator);
    double c = dist(generator);
    
    for (size_t y = 0; y < dim; y++) {
        double ly =  (double)y / (double)dim;
        for (size_t x = 0; x < dim; x++) {
            // RGBA channels
            double lx = (double)x / (double)dim;

            auto dists = getNearestPoints(lx, ly, 4);
            double f1 = (*dists)[0] / MAX_DIST;
            double f2 = (*dists)[1] / MAX_DIST;
            double f3 = (*dists)[2] / MAX_DIST;
            double f4 = (*dists)[3] / MAX_DIST;


            /* random linear combo
            toR->push_back(std::min(255.0, 100.0 * (a*f1 + b*f2 + c*f3)));
            toR->push_back(std::min(255.0, 100.0 * (b*f1 + c*f2 + a*f3)));
            toR->push_back(std::min(255.0, 100.0 * (c*f1 + b*f2 + a*f3)));
            */
            
            // blue with neat things
            toR->push_back(std::min(255.0, 550.0 * f1));
            toR->push_back(std::min(255.0, 550.0 * f2));
            toR->push_back(std::min(255.0, 550.0 * f3));

            
            // cool blue purple things
            /*
            toR->push_back(std::min(255.0, 700.0 * (f2 - f1)));
            toR->push_back(std::min(255.0, 250.0 * (f2 - f1)));
            toR->push_back(std::min(255.0, 700.0 * (f3 - f1)));
            */

            /* cool thing with colors
            toR->push_back(std::min(255.0, 400.0 * (f4 - f1)));
            toR->push_back(std::min(255.0, 400.0 * (f3 - f1)));
            toR->push_back(std::min(255.0, 400.0 * (f2 - f1)));
            */
            
            // thing from the paper
            /*
            toR->push_back(std::min(255.0, 700.0 * (f2 - f1)));
            toR->push_back(std::min(255.0, 700.0 * (f2 - f1)));
            toR->push_back(std::min(255.0, 250.0 * (f2 - f1)));
            */
            
            toR->push_back(255);
            
        }
    }

    return toR;
}

int main(int argc, char** argv) {
    WorleyGrid wg;
    char buf[100];
    memset(buf, sizeof(char), 100);
    
    for (int i = 0; i < 56; i++) {

        wg.resetPoints();
        wg.addRandomPoints(10);
        auto img = wg.toImage(256);

        snprintf(buf, 100, "test%d.png", i);
        
        lodepng::encode(buf, *img, 256, 256);
    }
}
