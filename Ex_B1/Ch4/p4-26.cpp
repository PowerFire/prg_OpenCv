//+++++++++++++++++++++++++++++++
//
//	
//
//+++++++++++++++++++++++++++++++
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/improc.hpp>

using namespace cv;

#define WINDOW_NAME1 "Fig 1"//定義視窗標題
#define WINDOW_NAME2 "Fig 2"//定義視窗標題
#define WINDOW_WIDTH 600	//定義視窗大小

int main(void)
{
	//建立空白的Mat影像
	Mat atomImage = Mat::zeros( WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);
	Mat rookImage = Mat::zeros( WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3);
	
	//--------------------------------------------------------------
	//					繪製化學中的原子範例圖
	//--------------------------------------------------------------
	// step 1.1 先繪製出橢圓
	DrawEllipse( atomImage, 90);
	DrawEllipse( atomImage, 0);
	DrawEllipse( atomImage, 45);
	DrawEllipse( atomImage, -45);

	// step 1.2 繪製圓心
	DrawFilledCircle(atomImage, Point(WINDOW_WIDTH/2, WINDOW_WIDTH/2));
}
