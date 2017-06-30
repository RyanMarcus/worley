#include "color_funcs.h"
#include <chrono>

void Icebergs::getPixelValues(std::vector<double>& dists,
                              unsigned int& r,
                              unsigned int& g,
                              unsigned int& b,
                              unsigned int& a) {
    r = 700.0 * (dists[1] - dists[0]);
    g = 700.0 * (dists[1] - dists[0]);
    b = 250.0 * (dists[1] - dists[0]);
    a = 255;
}

RandomLinearCombination::RandomLinearCombination() {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dist(-1.0, 3.0);
    c1 = dist(generator);
    c2 = dist(generator);
    c3 = dist(generator);
}

void RandomLinearCombination::getPixelValues(std::vector<double>& dists,
                              unsigned int& r,
                              unsigned int& g,
                              unsigned int& b,
                              unsigned int& a) {
    double f1 = dists[0];
    double f2 = dists[1];
    double f3 = dists[2];

    r = 100*(c1*f1 + c2*f2 + c3*f3);
    g = 100*(c2*f1 + c3*f2 + c1*f3);
    b = 100*(c3*f1 + c1*f2 + c2*f3);
    a = 255;
        
}

void EachChannel::getPixelValues(std::vector<double>& dists,
                              unsigned int& r,
                              unsigned int& g,
                              unsigned int& b,
                              unsigned int& a) {
    r = 550.0 * dists[0];
    g = 550.0 * dists[1];
    b = 550.0 * dists[2];
    a = 255;
}


void FourthDiff::getPixelValues(std::vector<double>& dists,
                              unsigned int& r,
                              unsigned int& g,
                              unsigned int& b,
                              unsigned int& a) {

    double f1 = dists[0];
    double f2 = dists[1];
    double f3 = dists[2];
    double f4 = dists[3];
    r = 400.0 * (f4 - f1);
    g = 400.0 * (f3 - f1);
    b = 400.0 * (f2 - f1);
    a = 255;
}
