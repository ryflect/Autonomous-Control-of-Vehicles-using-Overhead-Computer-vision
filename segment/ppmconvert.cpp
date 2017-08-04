#include "opencv2/highgui.hpp"
#include <string.h>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{

	if (argc!=2)
	{
		printf("Usage: ppmconvert inputimg.jpg\n");
		return -1;		
	}

	cv::Mat image;
	image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
	int filenamelength = strlen(argv[1]);

	if(!image.data)
	{
		printf("Usage: ppmconvert inputimg.jpg\n");
    	return -1;
    }
    char filename[200];
    char dummy[] = ".ppm";
    strcpy(filename, argv[1]);
    strcpy(filename + filenamelength - 4, dummy);

    cv::imwrite(filename, image);

    return 0;


}