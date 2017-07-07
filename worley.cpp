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

void WorleyGrid::createTrajectories() {
    for (auto& point : points) {
        std::unique_ptr<Trajectory> trj(new Trajectory(point->x,
                                                       point->y,
                                                       generator));
        trajectories.push_back(std::move(trj));
    }
}

void WorleyGrid::setTime(double t) {
    for (size_t i = 0; i < points.size(); i++) {
        trajectories[i]->getPosition(t,
                                     points[i]->x,
                                     points[i]->y);
    }
}

void WorleyGrid::getNearestPoints(double x, double y,
                                  std::vector<double>& outVal,
                                  std::vector<unsigned int>& pointIDs,
                                  size_t numPoints) {
    std::unique_ptr<std::vector<double>> toR(new std::vector<double>);

    // sort the points by their distance to (x, y)
    for (size_t i = 0; i < points.size(); i++) {
        double d = pow(points[i]->x - x, 2)
            + pow(points[i]->y - y, 2);
        outVal[i] = d;
    }

    // first, sort the pointIDs vector by the values in outVal
    std::sort(pointIDs.begin(), pointIDs.end(),
              [outVal](auto a, auto b) {
                  return outVal[a] < outVal[b];
              });

    // next sort the outVals themselves.
    // TODO we could recover the sorted order from pointIDs
    std::sort(outVal.begin(), outVal.end());

    for (size_t i = 0; i < numPoints; i++) {
        outVal[i] = sqrt(outVal[i]);
    }
   
}

std::unique_ptr<std::vector<unsigned char>>
WorleyGrid::toImage(size_t width, size_t height, ColorFunc& wf) {
    std::unique_ptr<std::vector<unsigned char>> toR(
        new std::vector<unsigned char>()
        );

    std::vector<double> dists(points.size());
    std::vector<unsigned int> pointIDs;

    for (size_t i = 0; i < points.size(); i++) {
        pointIDs.push_back(i);
    }
    
    size_t neededPoints = wf.getNumPointsNeeded();
    if (neededPoints == 0)
        neededPoints = points.size();
    
    for (size_t y = 0; y < height; y++) {
        double ly =  (double)y / (double)height;
        for (size_t x = 0; x < width; x++) {
            // RGBA channels
            double lx = (double)x / (double)width;

            getNearestPoints(lx, ly, dists, pointIDs, neededPoints);

            unsigned int r = 0;
            unsigned int g = 0;
            unsigned int b = 0;
            unsigned int a = 0;
            wf.getPixelValues(dists, pointIDs, r, g, b, a);
            toR->push_back(std::min((unsigned int) 255, r));
            toR->push_back(std::min((unsigned int) 255, g));
            toR->push_back(std::min((unsigned int) 255, b));
            toR->push_back(std::min((unsigned int) 255, a));
        }
    }

    return toR;
}

void printHelp() {
    printf("worley [-t type] [-w width] [-h height] [-n numImages] [-m] [-a]\n");
    printf("\t-t type, 1 - 6, for different coloring functions (default: 1)\n");
    printf("\t-w width, the width of the image (default: 200)\n");
    printf("\t-h height, the height of the image (default: 200)\n");
    printf("\t-n numImages, the number of images to generate (default: 1)\n");
    printf("\t-m, if given, makes a collage/montage of the resulting images\n");
    printf("\t-a, if given, create an animation. Uses -n as the number of frames.\n");
}
              
int main(int argc, char** argv) {
    WorleyGrid wg;
    char buf[1000];
    memset(buf, sizeof(char), 1000);

    int type = 1;
    size_t width = 200;
    size_t height = 200;
    int num = 1;
    bool doMontage = false;
    bool doAnimation = false;
    
    int c;
    while ((c = getopt(argc, argv, "mt:w:h:n:a")) != -1) {
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
        case 'a':
            doAnimation = true;
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

    if (doMontage && doAnimation) {
        fprintf(stderr, "Cannot create a montage and an animation at the same time. Please specify only one of -a or -m.\n");
    }

 

    for (int i = 0; i < num; i++) {
        Cellular wf1;
        Icebergs wf2;
        RandomLinearCombination wf3;
        EachChannel wf4;
        FourthDiff wf5;
        NearestPoint wf6;
    
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
        case 5:
            cf = &wf5;
            break;
        case 6:
            cf = &wf6;
            break;
        default:
            abort();
        }
    
        wg.resetPoints();
        wg.addRandomPoints(10);
        wg.createTrajectories();

        for (unsigned int f = 0; f < (double)num; f++) {
            double t = (double)f / (double)num;
            wg.setTime(t);
            auto img = wg.toImage(width, height, *cf);
            snprintf(buf, 1000, "out_t%d_n%05d_f%05d.png", type, i, f);
            lodepng::encode(buf, *img, width, height);
        }

        if (doAnimation) {
            snprintf(buf, 1000, "convert -loop 0 out_t%d_*.png out_t%d.mp4",
                     type, type);
            system(buf);
            break;

        }
    }

    if (doMontage) {
        system("montage out*.png montage.png");
        system("rm out*.png");
    }
}
