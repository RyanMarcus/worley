#include <vector>
#include <random>

#ifndef COLOR_FUNCS_H
#define COLOR_FUNCS_H


class Icebergs {
public:
    const char* getName() { return "Icebergs"; }
    const char* getDescription() {
        return "Orange \"icebergs\", representing the difference "
            "in distance from the two closest points.";
    }

    void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);
};

class RandomLinearCombination {
private:
    double c1;
    double c2;
    double c3;
    std::default_random_engine generator;
    
public:
    RandomLinearCombination();
    const char* getName() { return "Linear combination"; }
    const char* getDescription() {
        return "A random linear combination of the first three distances.";
    }

    void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);
};


class EachChannel {
public:
    const char* getName() { return "Independent channels"; }
    const char* getDescription() {
        return "Use the first, second, and third nearest points for the R, B, and G values.";
    }

    void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);
};


class FourthDiff {
public:
    const char* getName() { return "4th difference"; }
    const char* getDescription() {
        return "Difference from the fourth, third, and second component with the first are used for the RGB values.";
    }

    void getPixelValues(std::vector<double>& dists,
                        unsigned int& r,
                        unsigned int& g,
                        unsigned int& b,
                        unsigned int& a);
};



#endif
