#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>

using namespace cv;
using namespace std;

#define WINDOW_NAME "Program Window"

void on_MouseHandle(int event, int x, int y, int flags, void * param);
void DrawRectangle( Mat & img, Rect box);
void ShowHelpText();

Rect g_rectangle;
bool g_bDrawingBox = false; //是否進行繪製
RNG g_rng(12345);

int main( int argc, char** argv)
{
	//++++++++++++++++++++++++++++++
	//	[一] 準備參數
	//++++++++++++++++++++++++++++++
	g_rectangle = Rect(-1, -1, 0, 0);
	Mat srcImage( 600, 800, CV_8UC3), tempImage;
	srcImage.copyTo(tempImage);
	g_rectangle = Rect(-1,-1,0,0);
	srcImage = Scalar::all(0);

	//++++++++++++++++++++++++++++++
	//	[二] 設定滑鼠操作回呼函數
	//++++++++++++++++++++++++++++++
	namedWindow( WINDOW_NAME);
	setMouseCallback( WINDOW_NAME, on_MouseHandle, (void*) &srcImage);
	
	//++++++++++++++++++++++++++++++
	//	[三] 設定滑鼠操作回呼函數
	//++++++++++++++++++++++++++++++
	while(1) {
		srcImage.copyTo(tempImage);
		if(g_bDrawingBox) {
			//當進行繪製的識別字為真，則進行繪製
			DrawRectangle(tempImage, g_rectangle);
		}
		imshow( WINDOW_NAME, tempImage);

		//按下Esc鍵，退出程式
		if( waitKey(10) == 27) {
			break;
		}
	}
	
	return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	滑鼠回呼函數，根據不同的滑鼠事件進行不同的操作
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void on_MouseHandle( int event, int x, int y, int flag, void * param)
{
	Mat & image = *(Mat *) param;
	switch(event) {
		//滑鼠移動訊息
		case EVENT_MOUSEMOVE:
			//如果是否進行繪製的識別字為真，
			//則記錄下長和寬到RECT型變數中
			if	(g_bDrawingBox) {
				g_rectangle.width = x-g_rectangle.x;
				g_rectangle.height = y-g_rectangle.y;
			}
			break;
		//左鍵按下訊息
		case EVENT_LBUTTONDOWN:
			g_bDrawingBox = true;

			//記錄起始點
			g_rectangle = Rect( x, y, 0, 0);
			break;
		case EVENT_LBUTTONUP:
			g_bDrawingBox = false;

			//對寬和高小於0的處理
			if(g_rectangle.width < 0) {
				g_rectangle.x += g_rectangle.width;
				g_rectangle.width *= -1;
			}

			if(g_rectangle.height < 0) {
				g_rectangle.y += g_rectangle.height;
				g_rectangle.height *= -1;
			}
			//使用函數進行繪製
			DrawRectangle( image, g_rectangle);
			break;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//	自訂的矩行繪製函數
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DrawRectangle( Mat& img, cv::Rect box)
{
	//隨機顏色
	rectangle(img, box.tl(), box.br(), Scalar(g_rng.uniform(0,255),
		g_rng.uniform(0,255), g_rng.uniform(0,255)));
}
