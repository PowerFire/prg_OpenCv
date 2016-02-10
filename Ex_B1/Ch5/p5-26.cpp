//+++++++++++++++++++++++++++++++++++++++++++++++++
//
//					多色版影像混合
//
//+++++++++++++++++++++++++++++++++++++++++++++++++
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool MultiChannelBlending();
void ShowHelpText();

int main()
{
	system("color 9F");
	if(MultiChannelBlending()) {
		cout << "\n 執行成功，得出需要的影像" << endl;
	}

	waitKey(0);
	return 0;
}

bool MultiChannelBlending()
{
	//定義變數
	Mat srcImage;
	Mat logoImage;
	vector<Mat> channels;
	Mat imageBlueChannel;

	//+++++++++++++++++++++++++++++++++++++++++++++++++
	//		多色版混合的實現函數 : 
	//				藍色部分
	//+++++++++++++++++++++++++++++++++++++++++++++++++
	
	// Step 1 讀入圖片
	logoImage = imread("dota_logo.jpg",0);
	srcImage =imread("dota_jugg.jpg");

	if( !logoImage.data) {
		printf("Oh, no!!讀取logoImage錯誤!!\n");
	}

	if( !srcImage.data) {
		printf("Oh, no!!讀取srcImage錯誤!!\n");
	}
	
	// Step 2 把一個三色版影像轉換成三個單色版影像
	split(srcImage, channels);//分離色彩色版
	
	// Stpe 3 將原圖的藍色色版引用返回給imageBlueChannel 
	//		  注意是引用，相當於兩者等價，修改其中一個另一個跟著改變
	imageBlueChannel = channels.at(0);

	// Step 4 將原圖的藍色色版的(500,250)座標處右下方的一塊區域和logo
	//		  圖進行加權操作，將得到的混合結果存到imageBlueChannel中
	addWeighted(imageBlueChannel(Rect(500, 250, logoImage.cols, logoImage.rows)), 1.0,
				logoImage,0.5,0,imageBlueChannel(Rect(500, 250, logoImage.cols, logoImage.rows)));

	// Step 5 將三個單色版重新合併成一個三色版
	merge(channels, srcImage);

	// Step 6 顯示效果圖
	namedWindow(" logo Blue");
	imshow("<1>logo blue",srcImage);
}
