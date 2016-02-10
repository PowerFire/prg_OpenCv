#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//差異比較敏感值
const static int SENSITIVITY_VALUE = 20;

//用於模糊平整的大小
const static int BLUR_SIZE = 10;

//物件位置
int theObject[2] = {0, 0};

//物件矩形框，其中新為物件位置
Rect objectBoundingRectangle = Rect( 0, 0, 0, 0);

//整數轉換成字串
string inToString(int number) {
	stringstream ss;
	ss << number;
	return ss.str();
}

void searchForMovement(Mat thresholdImage, Mat cameraFeed)
{
	bool objectDetected = false;
	Mat temp;
	thresholdImage.copyTo(temp);

	//尋找輪框用的向量值
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//尋找最外圍輪廓
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//如果輪框向量不為空，表示找到物件
	if(contours.size() >0) {
		objectDetected = true;
	} else {
		objectDetected = false;
	}

	if(objectDetected) {
		//最大輪廓是最後的輪廓向量值
		vector< vector<Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size()-1));

		//最大輪廓的矩形框，並尋找中心點
		objectBoundingRectangle = boundingRect(largestContourVec.at(0));

		int xpos= objectBoundingRectangle.x+objectBoundingRectangle.width/2;
		int ypos= objectBoundingRectangle.y+objectBoundingRectangle.height/2;
		
		//物件位置
		theObject[0]= xpos, theObject[1]=ypos;
	}
	
	//物件位置
	int x = theObject[0];
	int y = theObject[1];

	//垂直線，綠色
	line(cameraFeed, Point(x, y-10), Point(x, y+10), Scalar(255, 0, 0),2);

	//水平線，綠色
	line(cameraFeed, Point(x-10, y), Point(x+10, y), Scalar(255, 0, 0), 2);
}

int main()
{
	//按鍵觸發值
	bool objectDetected = false;
	bool debugMode = false;
	bool trackingEnabled = false;
	bool pause = false;

	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;

	VideoCapture capture;

	while(1) {
		//影片播完再重播
		capture.open("54649.mp4");
		if(!capture.isOpened()) {
			cout << "影片讀取失敗" << endl;
			getchar();
			return -1;
		}
		namedWindow("連續影像差", CV_WINDOW_NORMAL);
		namedWindow("臨界影像", CV_WINDOW_NORMAL);
		namedWindow("影像", CV_WINDOW_NORMAL);
		namedWindow("最終臨界影像", CV_WINDOW_NORMAL);

		//檢查是否最終影格
		while(capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(CV_CAP_PROP_FRAME_COUNT) -1) {
			//判斷是否暫停
			if(!pause) {
				//讀取影格
				capture.read(frame1);

				//轉成灰階影像
				cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);

				//讀取影格
				capture.read(frame2);

				//轉成灰階影像
				cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
			}

			//比較兩連續影像差異並產生強度(intensity)影像
			absdiff(grayImage1, grayImage2, differenceImage);

			//將強度影像進行臨界處理
			threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

			//判斷是否為除錯模式
			if (debugMode==true) {
				//除錯模式，顯示結果於視窗
				imshow("連續影像差", differenceImage);
				imshow("臨界影像", thresholdImage);
			} else {
				//非除錯模式，清除視窗
				destroyWindow("連續影像差");
				destroyWindow("臨界影像");
			}

			//用模糊處理去除雜訊
			blur(thresholdImage, thresholdImage, Size(BLUR_SIZE, BLUR_SIZE));
			
			// 判斷是否儲錯模式
			if(debugMode==true)
				// 除錯模式，顯示結果於視窗
				imshow("最終臨界攝像",thresholdImage);
			else
				// 非除錯模式，清除視窗
				destroyWindow("最終臨界攝像");
			
			// 判斷是否追蹤模式
			if(trackingEnabled) {
				searchForMovement(thresholdImage,frame1);
				searchForMovement(thresholdImage,frame1);
			}

			// 顯示攝像
			imshow("影像",frame1);

			// 檢查按鍵, 10ms 等待是必須的
			switch(waitKey(10))	{
				case 27: //'esc' 鍵
					capture.release();
					return 0;

				case 115: //'s' 鍵
					trackingEnabled = !trackingEnabled;
					break;

				case 100: //'d' 鍵
					debugMode = !debugMode;
					break;

				case 112: //'p' 鍵
					pause = !pause;
					break;
			}
		}
		// 影片播放結束
		capture.release();
	}
	return 0;
}
