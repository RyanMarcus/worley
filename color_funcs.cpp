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
 
#include "color_funcs.h"
#include <chrono>


size_t Cellular::getNumPointsNeeded() {
    return 1;
}

void Cellular::getPixelValues(std::vector<double>& dists,
                              std::vector<unsigned int>& pIDs,
                              unsigned int& r,
                              unsigned int& g,
                              unsigned int& b,
                              unsigned int& a) {
    r = 500.0 * dists[0]; 
    g = 500.0 * dists[0]; 
    b = 500.0 * dists[0]; 
    a = 255;
}

size_t Icebergs::getNumPointsNeeded() {
    return 2;
}

void Icebergs::getPixelValues(std::vector<double>& dists,
                              std::vector<unsigned int>& pIDs,
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


size_t RandomLinearCombination::getNumPointsNeeded() {
    return 3;
}

void RandomLinearCombination::getPixelValues(std::vector<double>& dists,
                                             std::vector<unsigned int>& pIDs,
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


size_t EachChannel::getNumPointsNeeded() {
    return 3;
}

void EachChannel::getPixelValues(std::vector<double>& dists,
                                 std::vector<unsigned int>& pIDs,
                                 unsigned int& r,
                                 unsigned int& g,
                                 unsigned int& b,
                                 unsigned int& a) {
    r = 550.0 * dists[0];
    g = 550.0 * dists[1];
    b = 550.0 * dists[2];
    a = 255;
}

size_t FourthDiff::getNumPointsNeeded() {
    return 4;
}

void FourthDiff::getPixelValues(std::vector<double>& dists,
                                std::vector<unsigned int>& pIDs,
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


size_t NearestPoint::getNumPointsNeeded() {
    return 1;
}

NearestPoint::NearestPoint() {
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dist(0, 255);
    
    for (size_t i = 0; i < numRandomColors; i++) {
        red.push_back(dist(generator));
        green.push_back(dist(generator));
        blue.push_back(dist(generator));
    }
    
}

void NearestPoint::getPixelValues(std::vector<double>& dists,
                                  std::vector<unsigned int>& pIDs,
                                  unsigned int& r,
                                  unsigned int& g,
                                  unsigned int& b,
                                  unsigned int& a) {
    
    int ptIdx = pIDs[0] % numRandomColors;

    r = red[ptIdx];
    g = green[ptIdx];
    b = green[ptIdx];
    a = 255;
    
}
