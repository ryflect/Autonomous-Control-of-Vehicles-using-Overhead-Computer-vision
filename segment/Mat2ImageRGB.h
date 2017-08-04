#ifndef MAT2IMAGERGB_H
#define MAT2IMAGERGB_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <image.h>
#include <misc.h>
#include <pnmfile.h>

using namespace cv;

image<rgb> * ConvertMatToImage(Mat * MatImagePtr)
{
	int numrows = (*MatImagePtr).rows;
	int numcols = (*MatImagePtr).cols;
	//printf("\n%d %d\n", numrows, numcols);

	image<rgb> * SegmentClassImagePtr = new image<rgb>(numcols,numrows);

	for (int rowiter = 0; rowiter < numrows; rowiter++)
	{
		for (int coliter = 0; coliter < numcols; coliter++)
		{
			*imPtr(SegmentClassImagePtr,coliter,rowiter).b = (*MatImagePtr).at<Vec3b>(rowiter,coliter)[0];
			*imPtr(SegmentClassImagePtr,coliter,rowiter).g = (*MatImagePtr).at<Vec3b>(rowiter,coliter)[1];
			*imPtr(SegmentClassImagePtr,coliter,rowiter).r = (*MatImagePtr).at<Vec3b>(rowiter,coliter)[2];
		}
	}

/*	imshow("1", *MatImagePtr);
	waitKey(0);*/
	return SegmentClassImagePtr;
}

Mat ConvertImageToMat(image<rgb> * SegmentClassImagePtr)
{
	int numrows = SegmentClassImagePtr->height();
	int numcols = SegmentClassImagePtr->width();
	//printf("\n%d %d\n", numrows, numcols);

	Mat MatImage(numrows,numcols,CV_8UC3);

	for (int rowiter = 0; rowiter < numrows; rowiter++)
	{
		for (int coliter = 0; coliter < numcols; coliter++)
		{
			MatImage.at<Vec3b>(rowiter,coliter)[0] = *imPtr(SegmentClassImagePtr,coliter,rowiter).b;
			MatImage.at<Vec3b>(rowiter,coliter)[1] = *imPtr(SegmentClassImagePtr,coliter,rowiter).g;
			MatImage.at<Vec3b>(rowiter,coliter)[2] = *imPtr(SegmentClassImagePtr,coliter,rowiter).r;
		}
	}
/*	imshow("2", MatImage);
	waitKey(0);*/
	return MatImage;
}

#endif