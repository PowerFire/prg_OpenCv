/*===============================================
 *
 *			¿¿¿¿¿¿¿¿¿¿¿
 *
 *===============================================
 */

#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void ContrastAndBright(int, void *);

int g_nContrastValue;		//¿¿¿
int g_nBrightValue;			//¿¿¿
Mat g_srcImage, g_dstImage;

int main(   )
{

	system("color 2F");

	// step 1

	g_srcImage = imread( "1.jpg");
	if( !g_srcImage.data ) { printf("read error~¡I \n"); return false; }
	g_dstImage = Mat::zeros( g_srcImage.size(), g_srcImage.type() );


	g_nContrastValue=80;
	g_nBrightValue=80;


	namedWindow("temp1", 1);

	
	createTrackbar("c¡G", "temp1",&g_nContrastValue, 300,ContrastAndBright );
	createTrackbar("b¡G", "temp1",&g_nBrightValue, 200,ContrastAndBright );

	
	ContrastAndBright(g_nContrastValue,0);
	ContrastAndBright(g_nBrightValue,0);

	
	while(char(waitKey(1)) != 'q') {}
	return 0;
}



static void ContrastAndBright(int, void *)
{

	namedWindow("temp1", 1);

	for( int y = 0; y < g_srcImage.rows; y++ )
	{
		for( int x = 0; x < g_srcImage.cols; x++ )
		{
			for( int c = 0; c < 3; c++ )
			{
				g_dstImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( (g_nContrastValue*0.01)*( g_srcImage.at<Vec3b>(y,x)[c] ) + g_nBrightValue );
			}
		}
	}

	imshow("temp2", g_srcImage);
	imshow("temp3", g_dstImage);
}



