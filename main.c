// Command to compile the program
// The command is used to compile multiple c files that rely on each other

// "gcc main.c -o Program -lm"

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
// #include <stdlib.h>

// Adding the kmeans file
#include "k_means.h"

// Adding the stb image library to the program
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Preloads

void printOptions();
void printExitMessage();

void getPictureDetails();

int main(int argc, char **argv) {

  srand(time(NULL));
  bool run = true;
  int choice;

  printf("Welcome to the Color pallete extractor!\n");

  while (run) {

    printOptions();
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      getPictureDetails();
      break;
    case 2:
      run = false;
      printExitMessage();
      break;
    }
  }
}

void printOptions() {
  printf("Here are the options\n");
  printf("\t1 - Get colors from a picture\n");
  printf("\t2 - Exit\n");
}

void printExitMessage() { printf("The program will now exit\nGood Bye\n"); }

void loadPicture(int numOfColors, char *ptr) {
  printf("The number of colors you chose is %d\n", numOfColors);
  printf("The address you gave is %s\n", ptr);
  int width, height, channels;
  unsigned char *img = stbi_load(ptr, &width, &height, &channels, 3);
  if (img == NULL) {
    printf("The img could not be loaded\n");
    return;
  } else {
    printf("Img loaded successfully!\n");
    printf("Details:\nWidth: %dpx\nHeight: %dpx\nChannels: %d\n", width, height,
           channels);
    point_3d *points = malloc(sizeof(point_3d) * width * height);
    centroid_3d *centroids = malloc(sizeof(centroid_3d) * numOfColors);
    channels = 3;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        int pixelIndex = y * width + x;
        int byteIndex = pixelIndex * 3;
        points[pixelIndex].dx = img[byteIndex];
        points[pixelIndex].dy = img[byteIndex + 1];
        points[pixelIndex].dz = img[byteIndex + 2];
      }
    }
    k_mean_3d(numOfColors, (width * height), centroids, points);
    for (int i = 0; i < numOfColors; i++) {
      printf("Color :%d is r:%d g:%d b:%d\n", i, (int)round(centroids[i].dx),
             (int)round(centroids[i].dy), (int)round(centroids[i].dz));
    }

    stbi_image_free(img);
    free(points);
    free(centroids);
  }
}
void getPictureDetails() {

  printf("Please enter the path to the picture on the disk\n");
  char address[200];
  char garbage[5];
  // takes in an throws away the garbage from fgets
  fgets(garbage, sizeof(garbage), stdin);
  fgets(address, sizeof(address), stdin);
  address[strlen(address) - 1] = '\0';
  // printf("%s\n", address);

  char *ptr = address;
  printf("Please enter the amount of colors you want\n");
  printf("Choose between 4, 6 and 8 colors\n");
  int numOfColors;
  scanf("%d", &numOfColors);

  bool correctNum = false;

  if ((numOfColors == 4) || (numOfColors == 6) || (numOfColors == 8)) {
    correctNum = true;
  }

  while (correctNum == false) {
    printf("You did not input the correct number of colors you wanted.\n"
           "If you want to exit the selection process press 0 \n"
           "Try agian please: ");
    scanf("%d", &numOfColors);
    if ((numOfColors == 4) || (numOfColors == 6) || (numOfColors == 8)) {
      correctNum = true;
    } else if (numOfColors == 0) {
      return;
    }
  }

  loadPicture(numOfColors, ptr);
}
