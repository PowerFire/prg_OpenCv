#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//宣告全域變數
Mat img, templ, result;
const char* image_window = "Source Image";
const char* result_window = "Result Image";

int match_method;

//拉桿最大值
int max_Trackbar = 5;

//宣告函式
void MatchingMethod(int, void*);

int main(int, char** argv)
{
	// 載入圖擋
	img = imread("lena.jpg", 1);

	// 載入範本
	templ = imread("crop-lena.jpg", 1);

	// 建立視窗
	namedWindow(image_window , WINDOW_AUTOSIZE);
	namedWindow(result_window, WINDOW_AUTOSIZE);

	// 建立拉桿(指定比較的方式)
	// 0:SQDIFF           1:SQDIFF_NORMED 2: TM CCORR
	// 3:TM CCORR NORMED  4:TM COEDD      5: TM COEFF NORMED
	const char* trackbar_lavel = "Method: ";
	createTrackbar(trackbar_lavel, image_window, &match_method, max_Trackbar, MatchingMethod);

	//比對
	MatchingMethod(0,0);

	waitKey(0);
	return 0;
}

//因為傳遞參數但又不會使用到
void MatchingMethod( int, void*)
{
	//複製原圖
	Mat img_display;
	img.copyTo(img_display);

	//建立範本與原圖比對結果的矩陣
	int result_cols = img.cols -templ.cols +1;
	int result_rows = img.rows -templ.rows +1;

	result.create(result_cols, result_rows, CV_32FC1);

	//執行比對與正規化
	//比對
	matchTemplate(img, templ, result, match_method);
	//正規化
	//ps可以不用正規化也可以找出範圍
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	//尋找正規畫後的圖內之最大值與最小值及其位置
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());


	// SQIFF 與 SQDIFF_NORMED 是值愈低，比較結果愈好
	// 其他方法是越高越好
	// CV_TM_CCORR 不管取大或是取小都不准 是此範例的問題?
	if(match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) {
		matchLoc = minLoc;
	} else {
		matchLoc = maxLoc;
	}

	//繪製比對結果範圍(使用長方形)
	//原圖
	rectangle(img_display, matchLoc, Point(matchLoc.x +templ.cols, matchLoc.y +templ.rows), Scalar::all(0), 2, 8, 0);

	//正規化結果
	rectangle(result, matchLoc, Point(matchLoc.x +templ.cols, matchLoc.y +templ.rows), Scalar::all(0), 2, 8, 0);

	//顯示結果
	imshow(image_window, img_display);
	imshow(result_window, result);

	return;

}
