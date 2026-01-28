#ifndef KMEANS_H
#define KMEANS_H

#include <stdbool.h>

typedef struct{
    int ctrId;
    int dx;
    int dy;
    int dz;
} point_3d;

typedef struct {
    int id;
    float dx;
    float dy;
    float dz;
    float lastDx;
    float lastDy;
    float lastDz;
    bool converged;
} centroid_3d;

void k_mean_3d(int cNum, int pNum, centroid_3d centroids[], point_3d points[]);

#endif
