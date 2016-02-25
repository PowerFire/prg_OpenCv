#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	//載入原圖
	Mat image = imread("1.jpg");

	//建立原圖
	namedWindow("Box Filter - Source Fig");
	namedWindow("Box Filter - Change Fig");

	//顯示原圖
	imshow("Box Filter - Source Fig", image);
 
	//進行方框濾波操作
	Mat out;
	boxFilter( image, out, -1, Size(5,5));

	//顯示效果圖
	imshow("Box Filter - Change Fig", out);

	waitKey(0);
}
