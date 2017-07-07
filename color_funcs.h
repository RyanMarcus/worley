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

#ifndef COLOR_FUNCS_H
#define COLOR_FUNCS_H

class ColorFunc {
public:
    virtual const char* getName() = 0;
    virtual const char* getDescription() = 0;
    virtual size_t getNumPointsNeeded() {
        return 0;
    };

    virtual void getPixelValues(std::vector<double>& dists,
                                unsigned int& r,
                                unsigned int& g,
                                unsigned int& b,
                                unsigned int& a) = 0;
};

class Icebergs : public ColorFunc {
public:
    virtual const char* getName() { return "Icebergs"; }
    virtual const char* getDescription() {
        return "Orange \"icebergs\", representing the difference "
            "in distance from the two closest points.";
    }

    virtual void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);

    virtual size_t getNumPointsNeeded();
};

class RandomLinearCombination : public ColorFunc {
private:
    double c1;
    double c2;
    double c3;
    std::default_random_engine generator;
    
public:
    RandomLinearCombination();
    virtual const char* getName() { return "Linear combination"; }
    virtual const char* getDescription() {
        return "A random linear combination of the first three distances.";
    }

    virtual void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);

    virtual size_t getNumPointsNeeded();
};


class EachChannel : public ColorFunc {
public:
    virtual const char* getName() { return "Independent channels"; }
    virtual const char* getDescription() {
        return "Use the first, second, and third nearest points for the R, B, and G values.";
    }

    virtual void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);

    virtual size_t getNumPointsNeeded();
};


class FourthDiff : public ColorFunc {
public:
    virtual const char* getName() { return "4th difference"; }
    virtual const char* getDescription() {
        return "Difference from the fourth, third, and second component with the first are used for the RGB values.";
    }

    virtual void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);

    virtual size_t getNumPointsNeeded();
};



#endif
