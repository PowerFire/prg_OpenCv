/*++++++++++++++++++++++++++++++
 *
 *    Blur 影像模糊
 *
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstring>

using namespace cv;
using namespace std;

int main()
{
	// 載入原圖
	Mat srcImage = imread("1.jpg");

	// 顯示原圖
	imshow("均值濾波原圖", srcImage);

	//進行均值濾波操作
	Mat dstImage;
	blur( srcImage, dstImage, Size(7,7));

	//顯試圖效果
	imshow("均值濾波效果圖", dstImage);

	waitKey(0);

	return 0;
}
