#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstring>

using namespace cv;

using namespace std;

int main()
{
	//載入原圖
	Mat srcImage = imread("1.jpg");
	//顯示原圖
	imshow(" Make ", srcImage);
	//進行腐蝕操作
	Mat element= getStructuringElement(MORPH_RECT, Size(50,50));

	cout << "temp\n"	;
	
	Mat dstImage;
	erode(srcImage, dstImage, element);
	//顯示效果圖
	imshow("Solution", dstImage);
	waitKey(0);

	return 0;
}
