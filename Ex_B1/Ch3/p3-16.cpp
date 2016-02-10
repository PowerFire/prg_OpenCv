//------------------------------------------------------
//			建立滑動軸
//------------------------------------------------------
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#define WINDOW_NAME "Linear Intermix Example"

const int g_nMaxAlphaValue = 100;	//Alpha最大值
int g_nAlphaValueSlider; 			//滑動軸對應的變數
double g_dAlphaValue;
double g_dBetaValue;

//宣告儲存影像的變數
Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;

//------------------------------------------------------
//			回應滑動軸的回呼函數
//------------------------------------------------------
void on_Trackbar(int ,void* )
{
	//求出現在alpha值相對於最大值的比例
	g_dAlphaValue = (double) g_nAlphaValueSlider/g_nMaxAlphaValue;
	//則beta值為1減去alpha值
	g_dBetaValue = (1.0-g_dAlphaValue);

	//根據alpha和beta值進行線性混合
	addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2, g_dBetaValue, 0.0, g_dstImage);

	//顯示效果圖
	imshow(WINDOW_NAME, g_dstImage);
}

//------------------------------------------------------
//			控制台應用程式的入口函數
//------------------------------------------------------
int main(int argc, char** argv)
{
	//載入影像(兩影像的尺寸需相同)
	g_srcImage1 = imread("1.jpg");
	g_srcImage2 = imread("2.jpg");

	if(!g_srcImage1.data) {
		cout << "讀取第一幅圖片錯誤，請確定目錄下是否有imread函數指定圖片存在\n";
	}
	if(!g_srcImage2.data) {
		cout << "讀取第二幅圖片錯誤，請確定目錄下是否有imread函數指定圖片存在\n";
	}
	
	//設定滑動軸初值為70
	g_nAlphaValueSlider = 70;

	//建立表單
	namedWindow(WINDOW_NAME, 1);

	//在建立的表單中建立一個滑動軸控制項
	char TrackbarName[50];
	sprintf(TrackbarName, "Alpha value = %d", g_nMaxAlphaValue);

	//建立軌跡
	createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider,
		g_nMaxAlphaValue, on_Trackbar);
	
	//結果在回呼函數中顯示
	on_Trackbar( g_nAlphaValueSlider, 0);
	
	//按任義鍵退出
	waitKey(0);

	return 0;
}
