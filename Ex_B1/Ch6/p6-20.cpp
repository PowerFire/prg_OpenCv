#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	//載入原圖
	Mat srcImage = imread("1.jpg");

	//顯示原圖
	imshow(" blur function source fig", srcImage);

	//進行均值濾波操作
	Mat dstImage;
	blur(srcImage, dstImage, Size(7,7));

	imshow("blur funcion change fig", dstImage);

	waitKey(0);
}
