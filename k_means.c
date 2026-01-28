#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struct declarations


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

typedef struct {
  int ctrId;
  int dx;
  int dy;
  int dz;
} point_3d;

// The grid is used a reference for my points, it doesnt actually need to exist
typedef struct {
  int low_x;
  int low_y;
  int high_x;
  int high_y;
} grid;

// Preloads
void assign_centroid_coords_3d(int len, centroid_3d centroids[]);
void assign_centroid_id_3d(int len, centroid_3d centroids[]);

// Functions

void assign_point_3d_coords(int num, point_3d points[]) {
  for (int i = 0; i < num; i++) {
    points[i].dx = (rand() % 201 - 100);
    points[i].dy = (rand() % 201 - 100);
    points[i].dz = (rand() % 201 - 100);
  }
}

centroid_3d *create_centroids_3d(int num) {
  centroid_3d *centroids = malloc(sizeof(centroid_3d) * num);
  //  assign_centroid_coords_3d(num, centroids);
  assign_centroid_id_3d(num, centroids);
  return centroids;
}

void initial_load_3d(point_3d points[], centroid_3d *centroid, int len) {
  for (int i = 0; i < len; i++) {
    points[i].ctrId = centroid->id;
  }
}

void assign_centroid_id_3d(int len, centroid_3d centroids[]) {
  for (int i = 0; i < len; i++) {
    centroids[i].id = i;
    centroids[i].converged = false;
  }
}

void assign_centroid_coords_3d(int len, centroid_3d centroids[]) {
  for (int i = 0; i < len; i++) {
    centroids[i].dx = (rand() % 256);
    centroids[i].dy = (rand() % 256);
    centroids[i].dz = (rand() % 256);
    centroids[i].lastDx = centroids[i].dx;
    centroids[i].lastDy = centroids[i].dy;
    centroids[i].lastDz = centroids[i].dz;
  }
}

void assign_ctr_coords_3d(centroid_3d *ctr) {
  ctr->lastDx = ctr->dx;
  ctr->lastDy = ctr->dy;
  ctr->lastDz = ctr->dz;
  ctr->dx = (rand() % 256);
  ctr->dy = (rand() % 256);
  ctr->dz = (rand() % 256);
};

void convergance_3d(int cNum, bool *convergence, centroid_3d centroids[]) {
  for (int i = 0; i < cNum; i++) {
    if (round(centroids[i].dx) == round(centroids[i].lastDx) &&
        round(centroids[i].dy) == round(centroids[i].lastDy) &&
        round(centroids[i].dz) == round(centroids[i].lastDz)) {
      centroids[i].converged = true;
    }
  }
}

float length_of_line_3d(centroid_3d ctr, point_3d ptr) {
  float dx = (ctr.dx - ptr.dx);
  float dy = (ctr.dy - ptr.dy);
  float dz = (ctr.dz - ptr.dz);
  return (dx * dx) + (dy * dy) + (dz * dz);
}

void reassign_centroid_3d(centroid_3d *centroid, float averageX, float averageY,
                          float averageZ) {
  centroid->lastDx = centroid->dx;
  centroid->lastDy = centroid->dy;
  centroid->lastDz = centroid->dz;
  centroid->dx = averageX;
  centroid->dy = averageY;
  centroid->dz = averageZ;
}

void calculate_average_3d(int ptrLen, int ctrLen, point_3d points[],
                          centroid_3d *centroid) {
  float averageX = 0, averageY = 0, averageZ = 0;
  int len = 0;
  for (int i = 0; i < ptrLen; i++) {
    if (centroid->id == points[i].ctrId) {
      len++;
      averageX += points[i].dx;
      averageY += points[i].dy;
      averageZ += points[i].dz;
    }
  }

  if (len == 0) {
    assign_ctr_coords_3d(centroid);
    return;
  }
  averageX = averageX / len;
  averageY = averageY / len;
  averageZ = averageZ / len;
  reassign_centroid_3d(centroid, averageX, averageY, averageZ);
}



void get_averages_3d(int ptrLen, int ctrLen, point_3d points[],
                     centroid_3d centroids[]) {
  for (int i = 0; i < ctrLen; i++) {
    calculate_average_3d(ptrLen, ctrLen, points, &centroids[i]);
  }
}



void k_mean_3d(int cNum, int pNum, centroid_3d centroids[], point_3d points[]) {

  assign_centroid_id_3d(cNum, centroids);
  assign_centroid_coords_3d(cNum, centroids);
  initial_load_3d(points, &centroids[0], pNum);
  bool convergence = false;

  int cap = 0;
  int max = 50;
  while (convergence == false && cap < max) {
    cap++;
    sort_points_3d(pNum, cNum, points, centroids);
    get_averages_3d(pNum, cNum, points, centroids);
    for (int i = 0; i < cNum; i++) {
      centroids[i].converged = false;
    }
    convergance_3d(cNum, &convergence, centroids);

    convergence = true;
    for (int i = 0; i < cNum; i++) {
      if (!centroids[i].converged) {
        convergence = false;
        break;
      }
    }
    printf("Convergence is %b\n", convergence);
  }
}

