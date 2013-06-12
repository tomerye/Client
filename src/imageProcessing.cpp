
#include "imageProcessing.h"

using namespace std;

time_t getCurrTime() {
	time_t seconds = time(NULL);
	return seconds;
}

//get the avg,min and max from color image
void getMinMaxAvg(IplImage* img, double* min, double* max, double* avg) {

	int height, width, step, channels, i, j, k;
	double minl = 255, maxl = 0, val = 0, avgl, chover = 0, counter = 0,
			new_TH = 0;
	time_t sec = 0;
	uchar *data;

	// get the image data
	height = img->height;
	width = img->width;
	step = img->widthStep;
	channels = img->nChannels;
	data = (uchar *) img->imageData;

	// invert the image
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			for (k = 0; k < channels; k++) {
				val = data[i * step + j * channels + k];
				chover += val;
				counter++;
				if (val < minl) {
					minl = val;
				}
				if (val > maxl) {
					maxl = val;
				}
			}
	avgl = chover / counter;
	printf_debug("Min:%f Max:%f\n Avg:%f\n", minl, maxl, avgl);
	*min = minl;
	*max = maxl;
	*avg = avgl;
	return;
}

//calc the new TH from the avg of the img with const MAX_FACTOR and TH
double calcFactor(double avg) {
	double factor = 200.0 / avg;
	if (factor > SCALE_FACTOR_MAX)
		factor = SCALE_FACTOR_MAX;
	return TH / factor;
}

int imageProcessing(Client *client) {

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	double min = 255, max = 0, avg = 0, new_TH = 0;
	int i = 0, alarm = 0, num = 0;

	CvRect bndRect = cvRect(0, 0, 0, 0);
	CvPoint pt1, pt2;
	CvSeq* contour;

	while (!capture) {
		printf_debug("CANT CAPTURE CAM");
		capture = cvCaptureFromCAM(CV_CAP_ANY);
	}
	printf_debug("capture cam\n");

	IplImage* frame = cvQueryFrame(capture);
	IplImage* temp = cvCloneImage(frame);
	IplImage* temp2 = cvCloneImage(frame);
	IplImage* frameSmooth = cvCloneImage(temp);
	//IplImage* movingAVG= cvCloneImage(temp);
	//IplImage* frameDIFF= cvCloneImage(temp);
	CvSize imgSize;
	imgSize.width = frame->width;
	imgSize.height = frame->height;

	IplImage* movingAVG = cvCreateImage(imgSize, IPL_DEPTH_32F, 3);
	//IplImage* frameDIFF=cvCreateImage(imgSize, IPL_DEPTH_32F, 3);
	IplImage* frameGRAY = cvCreateImage(imgSize, IPL_DEPTH_8U, 1);
	IplImage* frameGRAY2 = cvCreateImage(imgSize, IPL_DEPTH_8U, 1);

	//To Smooth the Image
	cvSmooth(temp, frameSmooth, CV_GAUSSIAN, CV_GAUSSIAN_PARAM1,
			CV_GAUSSIAN_PARAM2);

	//a way to clone the Image
	cvConvertScale(frameSmooth, movingAVG, 1.0, 0.0);
	//cvConvertScale(temp, frameDIFF, 1.0, 0.0);

	//setup - get NUM_FIRST_IMAGES and calculate the moving Average
	for (i = 0; i < NUM_FIRST_IMAGES; i++) {
		cvWaitKey(CAPTURE_TIME_MILISEC);
		frame = cvQueryFrame(capture);
		temp = cvCloneImage(frame);
		cvSmooth(temp, frameSmooth, CV_GAUSSIAN, CV_GAUSSIAN_PARAM1,
				CV_GAUSSIAN_PARAM2);
		cvRunningAvg(frameSmooth, movingAVG, ALPHA, NULL);
		cvReleaseImage(&temp);
	}

	cvSaveImage("first_avg.jpg", movingAVG);

	printf_debug("first step done\n");

	IplImage* frameDIFF = cvCloneImage(frameSmooth);
	while (1) {
		//Sleep(200);
		frame = cvQueryFrame(capture);
		cvWaitKey(CAPTURE_TIME_MILISEC);
		temp = cvCloneImage(frame);
		cvSmooth(temp, frameSmooth, CV_GAUSSIAN, CV_GAUSSIAN_PARAM1,
				CV_GAUSSIAN_PARAM2);
		cvCvtColor(frameSmooth, frameGRAY, CV_RGB2GRAY);

		//cvSaveImage("frame.jpg",frameSmooth);

		getMinMaxAvg(frameGRAY, &min, &max, &avg);

		cvConvertScale(movingAVG, temp2, 1.0, 0.0);
		//cvSaveImage("avg2.jpg",temp2);
		cvAbsDiff(temp2, frameSmooth, frameDIFF);

		//cvSaveImage("diff.jpg",frameDIFF);

		cvCvtColor(frameDIFF, frameGRAY, CV_RGB2GRAY);
		new_TH = calcFactor(avg);
		printf_debug("TH:%f", new_TH);
		cvThreshold(frameGRAY, frameGRAY, new_TH, 255, CV_THRESH_BINARY);

		//cvSaveImage("TH.jpg",frameGRAY);

		cvDilate(frameGRAY, frameGRAY2, 0, DilatePARAM);
		//dst=dilate(src,element):  dst(x,y)=max((x',y') in element))src(x+x',y+y') 3X3

		cvErode(frameGRAY2, frameGRAY, 0, ErodePARAM);
		//dst=erode(src,element):  dst(x,y)=min((x',y') in element))src(x+x',y+y') 3X3

		cvSaveImage("TH_after.jpg", frameGRAY);

		CvMemStorage* storage = cvCreateMemStorage(0);
		contour = 0;
		cvFindContours(frameGRAY, storage, &contour, sizeof(CvContour),
				CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

		for (; contour != 0; contour = contour->h_next) {
			printf("contour\n");
			bndRect = cvBoundingRect(contour, 0);
			pt1.x = bndRect.x;
			pt1.y = bndRect.y;
			pt2.x = bndRect.x + bndRect.width;
			pt2.y = bndRect.y + bndRect.height;
			int s = bndRect.height * bndRect.width;
			//printf_debug("Size=%d\n,P1.x=%d P2.x=%d",s,pt1.x,pt2.x);
			if (s > 10000) {//CUTTING IMAGE PATCH:(s>10000&&((pt1.x!=1)||(pt2.x!=639))) {
				cvRectangle(temp, pt1, pt2, CV_RGB(255,0,0), 2);  //draw rect
				printf_debug("Size=%d\n,P1.x=%d P2.x=%d", s, pt1.x, pt2.x);
				alarm = 1;
			}
		}
		if (alarm > 0) {
			num++;
			alarm = 0;
		} else
			num = 0;

		if (num == 0)
			cvRunningAvg(frameSmooth, movingAVG, ALPHA, NULL);
		if (num == 1) {  //CUTTING IMAGE PATCH - CHANGE TO:  if(num==2)
			char* imageName = new char[50];
			//strftime(imageName, 20, "%Y-%m-%d %H:%M:%S", time(NULL)); 
			sprintf(imageName, "%lu\0", time(NULL));
			strcat(imageName, ".jpg");
			cvSaveImage(imageName, temp);

			PacketForServer *packet = new PacketForServer();
			packet->opcode_ = 1;
			packet->file_path_ = std::string(imageName);
			packet->priority_=3;
			client->sendPacket(packet);

			printf_debug("ALARM TIME:%s\n", imageName);
			//TODO:: sendALARM();
			delete[] imageName;
			num = 0;
		}
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&temp);
	}

	cvReleaseCapture(&capture);
	return 0;
}
