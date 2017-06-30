
#include "worley.h"
#include "color_funcs.h"
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "lodepng.h"
#include <chrono>
#include "unistd.h"
#include <stdlib.h>

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

std::unique_ptr<std::vector<unsigned char>>
WorleyGrid::toImage(size_t width, size_t height, ColorFunc& wf) {
    std::unique_ptr<std::vector<unsigned char>> toR(
        new std::vector<unsigned char>()
        );

    std::vector<double> dists(points.size());

    for (size_t y = 0; y < height; y++) {
        double ly =  (double)y / (double)height;
        for (size_t x = 0; x < width; x++) {
            // RGBA channels
            double lx = (double)x / (double)width;

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

void printHelp() {
    printf("worley [-t type] [-w width] [-h height] [-n numImages] [-m]\n");
    printf("\t-t type, 1 - 4, for different coloring functions (default: 1)\n");
    printf("\t-w width, the width of the image (default: 200)\n");
    printf("\t-h height, the height of the image (default: 200)\n");
    printf("\t-n numImages, the number of images to generate (default: 1)\n");
    printf("\t-m, if given, makes a collage/montage of the resulting images\n");
}
              
int main(int argc, char** argv) {
    WorleyGrid wg;
    char buf[100];
    memset(buf, sizeof(char), 100);

    int type = 1;
    size_t width = 200;
    size_t height = 200;
    int num = 1;
    bool doMontage = false;
    
    int c;
    while ((c = getopt(argc, argv, "mt:w:h:n:")) != -1) {
        switch (c) {
        case 't':
            type = atoi(optarg);
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 'n':
            num = atoi(optarg);
            break;
        case 'm':
            doMontage = true;
            break;
        case '?':
        default:
            printHelp();
            exit(-1);
            
        }
    }

    if (doMontage && num == 1) {
        fprintf(stderr, "Cannot create a montage from one image! Please specify a number of images with the -n flag, or do not specify -m.\n");
        exit(-1);
    }

 

    for (int i = 0; i < num; i++) {
        Icebergs wf1;
        RandomLinearCombination wf2;
        EachChannel wf3;
        FourthDiff wf4;
    
        ColorFunc* cf = NULL;
        switch (type) {
        case 1:
            cf = &wf1;
            break;
        case 2:
            cf = &wf2;
            break;
        case 3:
            cf = &wf3;
            break;
        case 4:
            cf = &wf4;
            break;
        default:
            abort();
        }
    
        wg.resetPoints();
        wg.addRandomPoints(10);
        auto img = wg.toImage(width, height, *cf);

        snprintf(buf, 100, "test%d.png", i);
        
        lodepng::encode(buf, *img, width, height);
    }

    if (doMontage) {
        system("montage test*.png montage.png");
    }
}
