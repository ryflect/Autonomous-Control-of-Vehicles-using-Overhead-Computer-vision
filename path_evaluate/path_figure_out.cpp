#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

void path_calculate(Mat inputimage, int startX, int startY)
{
	int x,y,i,j,k;
	int numneighbors;
	int totalpathpoints = 0;
	int xsingleneighbour[50],ysingleneighbour[50];
	int singleneighbourindex = 0;

	Mat Path = Mat_<int>(30,2);
	Path = 0;

	for (x = 1; x<inputimage.cols - 1; x++)
	{	
		for (y = 1; y<inputimage.rows - 1; y++)
		{	
			//cout<<"x:"<<x<<" y:"<<y<<" "<<int(inputimage.at<uchar>(y,x))<<"\n";
			//cout<<x<<" "<<y<<" "<<inputimage.at<uchar>(y,x)<<"\n";
			if (int(inputimage.at<uchar>(y,x)) == 255)
			{	
				cout<<"x:"<<x<<" y:"<<y<<" "<<int(inputimage.at<uchar>(y,x))<<"\n";
				totalpathpoints ++;
				numneighbors = 0;
				for (i = -1; i<= 1 ; i++)
				{
					for (j = -1; j<=1 ; j++)
					{	
						if((i == 0)&&(j == 0))
							continue;

						if(int(inputimage.at<uchar>(y+j,x+i)) == 255)
							numneighbors++;
					}
				}
				cout<<"NUMNEIGH:"<<numneighbors<<"\n";
				if (numneighbors == 1)
				{
					xsingleneighbour[singleneighbourindex] = x;
					ysingleneighbour[singleneighbourindex] = y;
					singleneighbourindex++;
				}
			}

		}
	}

	double minimum = 1000000, distance = 0;
	int minimumindex = -1;
	cout<<"snbi :"<<singleneighbourindex<<"\n";
	for (i = 0;i<singleneighbourindex; i++)
	{
		distance = pow(double(startX) - double(xsingleneighbour[i]), 2) + pow(double(startY) - double(ysingleneighbour[i]),2);
		cout<<"distance: "<<distance<<"\n";
		if (distance < minimum)
		{
			minimum = distance;
			minimumindex = i;
		}
	}

	//Start Point
	cout<<"minindex: "<<minimumindex<<".\n";
	int currentX = xsingleneighbour[minimumindex];
	int currentY = ysingleneighbour[minimumindex];

	Path.at<int>(0,0) = currentX;
	Path.at<int>(0,1) = currentY;

	int newcurrentX = -1, newcurrentY = -1, prevX = -1, prevY = -1;

	//cout<<"tot:"<<totalpathpoints<<"\n";
	for (k = 1; k < totalpathpoints ; k++)
	{
		for (i = -1; i<= 1 ; i++)
		{
			for (j = -1; j<=1 ; j++)
			{	
				if((i == 0)&&(j == 0))
					continue;
				if(int(inputimage.at<uchar>(currentY+j,currentX+i)) == 255)
				{	
					if(!((prevY == (currentY+j))&&(prevX == (currentX+i))))
					{
						newcurrentY = currentY+j;
						newcurrentX = currentX+i;
					}
				}
			}
		}

		prevX = currentX;
		prevY = currentY;

		currentX = newcurrentX;
		currentY = newcurrentY;

		Path.at<int>(k,0) = currentX;
		Path.at<int>(k,1) = currentY;

	}

	cout<<"Path array:\n"<<Path;

}

int main()
{	
	Mat inputimage = Mat::zeros(20,20, CV_8UC1);

	inputimage.at<uchar>(5,5) = 255;
	inputimage.at<uchar>(5,6) = 255;
	inputimage.at<uchar>(5,7) = 255;
	inputimage.at<uchar>(5,8) = 255;
	inputimage.at<uchar>(6,9) = 255;
	inputimage.at<uchar>(7,10) = 255;
	inputimage.at<uchar>(8,11) = 255;
	inputimage.at<uchar>(9,11) = 255;
	inputimage.at<uchar>(10,11) = 255;
	inputimage.at<uchar>(11,12) = 255;

	imshow("window",inputimage);
	waitKey(0);

	path_calculate(inputimage, 10, 10);
	
	return 0;
}