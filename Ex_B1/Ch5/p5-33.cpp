/*===============================================
 *
 *			影像對比度、亮度值調整
 *
 *===============================================
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

static void on_ContrastAndBright(int, void *);

int g_nContrastValue;		//對比度
int g_nBrightValue;			//亮度值
Mat g_srcImage, g_dstImage;

int main(   )
{

	system("color 2F");

	// step 1 讀取匯入影像
	g_srcImage = imread("1.jpg");
	if (!g_srcImage.data) {
		printf("圖取圖片錯誤!!\n");
		return -1;
	}
	g_dstImage = Mat::zeros( g_srcImage.size(), g_srcImage.type() );

	// step 2 設定對比度和亮度的值
	g_nContrastValue = 80;
	g_nBrightValue = 80;

	// step 3 建立效果圖視窗
	namedWindow("Future Fig", 1);

	// step 4 建立軌跡條
	createTrackbar("Contrast :", "Future Fig", &g_nContrastValue, 300, on_ContrastAndBright);
	createTrackbar("Brightness :", "Future Fig", &g_nBrightValue, 200, on_ContrastAndBright);

	// step 5 進行回呼函數初始化
	on_ContrastAndBright(g_nContrastValue, 0);
	on_ContrastAndBright(g_nBrightValue, 0);
	
	// step 6 按下q時，程式退出
	while(char(waitKey(1))!='q') {}

	return 0;
}

/*--------------------------------------
 *
 *	改變影像對比度和亮度值的回呼函數
 *
 *-------------------------------------
 */
static void on_ContrastAndBright( int, void *)
{
	//建立視窗
	namedWindow("Original Fig", 1);

	//三個迴圈，執行運算 g_dstImage(i,j) = a*g_srcImage(i,j)+b
	for (int y=0; y<g_srcImage.rows; y++) {
		for (int x=0; x< g_srcImage.cols; x++) {
			for(int c=0; c<3; c++) {
				g_dstImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(
					( g_nContrastValue*0.01)*(g_srcImage.at<Vec3b>(y,x)[c])+ g_nBrightValue);
			}
		}
	}
	// 顯示影像
	imshow("Orginal Fig", g_srcImage);
	imshow("Change  Fig", g_dstImage);
}
