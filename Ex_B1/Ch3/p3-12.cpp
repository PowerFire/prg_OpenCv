//---------------------------
//	影像的載入、顯示與輸出
//---------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	//------------------------------------
	//	[一]
	//	影像的載入與顯示
	//	以下三行，用於完成影像的載入和顯示
	//------------------------------------

//	Mat girl = imread("girl.jpg");	//載入影像到Mat
//	namedWindow("Animation Fig");	//建立一個名為"Animation Fig"的視窗
//	imshow("Animation Fig", girl);	//顯示名為"Animation Fig"的視窗

	//------------------------------------
	//	[二]
	//	初級影像混合
	//------------------------------------
	//載入圖片
	Mat image = imread("dota.jpg");	//199-->載入三色版的彩色影像
										//0-->載入灰階圖
										//2|4-->載入無損的來原圖
	Mat logo  = imread("dota_logo.jpg");

	//載入後先顯示
	namedWindow("Original Fig");
	imshow("Original Fig", image);
	cout << "Original rows: " << image.rows << "columns:" << image.cols << endl;

	namedWindow("Logo Fig");
	imshow("Logo Fig", logo);
	cout << "Logo rows: " << logo.rows << "columns:" << logo.cols << endl;
	//定義一個Mat類型，用於存放影像的ROI
	Mat imageROI;

	cout << "Method One \n" ;
	//Method One
	imageROI = image(Rect(800,350,logo.cols,logo.rows));
	/*Rect(800,350,logo.cols,logo,rows)
	 *左上點位置(800,350);大小(logo.cols,logo,rows)
	 */
	//Method Two
	//imageROI =image(Range(350,350+logo.rows),Range(800,800+logo.cols));
	
	//將logo加到原圖上
	addWeighted(imageROI ,0.5 ,logo ,0.3 ,0. ,imageROI);
	/*C++: void addWeighted(InputArray src1, double alpha,
	 *						InputArray src2, double beta, double gamma,
	 *						OutputArray	dst, int dtype=-1)
	 */

	//顯示結果
	namedWindow("Original+log Fig");
	imshow("Original+log Fig",image);

	//------------------------------------
	//	[三]
	//	將一個Mat影像輸出到影像擋
	//------------------------------------
	//輸出一張jpg圖片到專案目錄下
	imwrite("p3-12.jpg",image);

	waitKey();

	return 0;
}
