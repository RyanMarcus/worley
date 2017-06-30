#include <vector>
#include <random>

#ifndef COLOR_FUNCS_H
#define COLOR_FUNCS_H

class ColorFunc {
public:
    virtual const char* getName() = 0;
    virtual const char* getDescription() = 0;

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
};



#endif
