//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//				存取影像中像素的三種方法
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				全域函數宣告
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//方法 一：用指標存取像素
//		  使用 "指標存取:C操作符號" 方法版的顏色空間縮減函數
#define POINT_METHOD 1
void colorReduce_point(Mat & inputImage, Mat & outputImage, int div);
//方法 二：用反覆運算器操作像素
//		  使用 "反覆運算器" 方法版的顏色空間縮減函數
#define ITERATOR_METHOD 0
void colorReduce_iterator(Mat & inputImage, Mat & outputImage, int div);
//方法 三：動態位址計算
//		  使用 "動態位址運算配合at" 方法版的顏色空間縮減函數
#define DYNAMIC_MRTHOD 0
void colorReduce_dynamic(Mat & inputImage, Mat & outputImage, int div);

int main (void)
{
	// Step 1 建立原始圖並顯示
	Mat srcImage = imread("1.jpg");
	imshow("Original Fig", srcImage);

	// Step 2 按原圖的參數規格建立效果
	Mat dstImage;
	//效果圖的大小、類型與原始圖片相同
	dstImage.create( srcImage.rows, srcImage.cols, srcImage.type());

	// Step 3 記錄起使時間
	double time0 = static_cast<double> (getTickCount());

	// Step 4 使用顏色空間縮減函數
#if POINT_METHOD
	//使用指標存取方法的顏色空間縮減函數
	colorReduce_point(srcImage, dstImage, 10);
#elif ITERATOR_METHOD
	//使用反覆運算器操作像素
	colorReduce_iterator(srcImage, dstImage, 32);
#elif DYNAMIC_METHOD
	//使用動態位址運算
	colorReduce_dynamic(srcImage, dstImage, 32);
#endif
	// Step 5 計算執行時間並輸出
	time0 = ((double) getTickCount()-time0)/getTickFrequency();
	cout << "此方法執行時間為:" << time0 << "秒" <<endl;//輸出執行時間

	// Step 6 顯示效果圖
	imshow( "Reduce Fig", dstImage);
	waitKey(0);
}

//方法 一：用指標存取像素
//		  使用 "指標存取:C操作符號" 方法版的顏色空間縮減函數
#define POINT_METHOD 1
void colorReduce_point(Mat & inputImage, Mat & outputImage, int div) {
	//參樹準備
	outputImage = inputImage.clone();//複製實體參數到臨時變數

	int rowNumber = outputImage.rows; //行數
	int colNumber = outputImage.cols*outputImage.channels();//列數*色版數=每一行元素的個數

	//雙重迴圈
	for(int i = 0; i < rowNumber; i++) {//行迴圈
		uchar* data = outputImage.ptr<uchar>(i); //取得第i行的首地址
		for(int j = 0; j < colNumber; j++) { //列迴圈
			//+++++ 處理每個像素 +++++
			data[j] = data[j]/div*div+div/2;
			//----- 處理結束 -----
		} //列處理結束
	} //行處理結束
}
//方法 二：用反覆運算器操作像素
//		  使用 "反覆運算器" 方法版的顏色空間縮減函數
#define ITERATOR_METHOD 0
void colorReduce_iterator(Mat & inputImage, Mat & outputImage, int div)
{
	//參數準備
	outputImage = inputImage.clone(); //複製實體參數到臨時變數

	
	//取得反覆運算器
	Mat_<Vec3b>::iterator it = outputImage.begin<Vec3b> ();//初始位置的反覆運算器
	Mat_<Vec3b>::iterator itend = outputImage.end<Vec3b> ();//終止位置的反覆運算器
	
	//存取彩色影像像素
	for(; it!= itend; ++it) {
		// ++++++++++ 開始處理每個像素
		(*it)[0] = (*it)[0]/div*div+div/2; //藍色色版
		(*it)[1] = (*it)[1]/div*div+div/2; //綠色色版
		(*it)[2] = (*it)[2]/div*div+div/2; //紅色色版
		// ---------- 處理結束

	}
}
//方法 三：動態位址計算
//		  使用 "動態位址運算配合at" 方法版的顏色空間縮減函數
#define DYNAMIC_MRTHOD 0
void colorReduce_dynamic(Mat & inputImage, Mat & outputImage, int div)
{
	//參數準備
	outputImage = inputImage.clone(); //複製實體參數到臨時變數
	int rowNumber = outputImage.rows; //行數
	int colNumber = outputImage.cols; //列數

	//存取彩色影像像素
	for (int i = 0; i < rowNumber; i++) {
		for (int j= 0; j< colNumber; j++) {
			// ++++++++++ 開始處理每個像素
			outputImage.at<Vec3b>(i,j)[0] = outputImage.at<Vec3b>(i,j)[0]/div*div+div/2; //藍色色版
			outputImage.at<Vec3b>(i,j)[1] = outputImage.at<Vec3b>(i,j)[1]/div*div+div/2; //綠色色版
			outputImage.at<Vec3b>(i,j)[2] = outputImage.at<Vec3b>(i,j)[2]/div*div+div/2; //紅色色版
			// ---------- 處理結束
		}
	}
}

