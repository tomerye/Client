

#include "cv.h" 
#include "highgui.h"
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <time.h>
#include "Client.h"
#include "PacketForServer.h"

#ifndef IMAGEPROFESSING_H_
#define IMAGEPROFESSING_H_

#define DEBUG

#ifdef DEBUG 
#define printf_debug printf 
#else 
#define printf_debug while(0)printf 
#endif 

//when start the program first we take NUM_FIRST_IMAGES
#define NUM_FIRST_IMAGES 5

//THE TIME WE WAIT BETWEEN CAPTURE AN IMAGE
#define CAPTURE_TIME_MILISEC 100

// ALPHA PARAM FOR MOVING AVG
#define ALPHA 0.01

//CV_GAUSSIAN (gaussian blur) - convolving image with param1�param2 Gaussian kernel
#define CV_GAUSSIAN_PARAM1 11
#define CV_GAUSSIAN_PARAM2 11


//when start the program first we take NUM_FIRST_IMAGES
#define NUM_FIRST_IMAGES 5

//changes under TH will not considered
#define TH_MIN_PARAM 10  //will not be used -we use diffrent method
#define TH 67.0

#define DilatePARAM 18
#define ErodePARAM 10


//we use scalling for TH
#define SCALE_FACTOR_MAX 5

#define SAVE_IMAGE_PATH "c:\\"


//size of contour under CONTOUR_SIZE_MIN will not considered
#define CONTOUR_SIZE_MIN 10000

//Function
time_t getCurrTime();
void getMinMaxAvg(IplImage* img,double* min,double* max,double* avg);
double calcFactor(double avg);
int imageProcessing(Client *client);

#endif /* IMAGEPROFESSING_H_ */
