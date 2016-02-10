#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

string intToString(int number) {
	
	std::stringstream ss;
	ss << number;
	return ss.str();
}


int main(int argc, char * argv[])
{
	//開啟編號0的攝影機
	//方法一
	VideoCapture cap(0);
	//方法二
	//VideoCapture cap;
	//cap.open(0);

	//開啟失敗就結束程式
	if (!cap.isOpened()) {
		cout << "無法啟動攝影機" << endl;
		return -1;
	}

	const char* windowName = "Webcam Feed";

	//儲存圖檔的檔名
	char filename [50];
	//擋名序號起始
	int n = 0;

	//顯示攝影機內容的視窗
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	
//	printf("temp\n");
	while(1) {
		Mat frame;
		
//		printf("temp2\n");
		//從攝影機讀取影格(frame)
		bool bSuccess = cap.read(frame);

		//檢查是否讀取成功
		if (!bSuccess) {
			cout << "無法從攝影機讀取影格" << endl;
			break;
		}

		//顯示讀取的影格
		imshow(windowName, frame);
		
		//delay M millis, usually long enpugh to display and capture input
		int key =  waitKey(10);
//		printf("key = %d\n", key);

		//等待10豪秒是否按鍵
		switch(key) {

			// 'Esc' 鍵按下('esc' ASCII 27)
			case 27:
				//結束程式
//				printf("input esc\n");
				return 0;
				break;
			// 'Enter' 鍵按下('Enter' ASCII 10)
			case 10:
//				printf("input enter\n");
				sprintf(filename, "video%d.jpg", n++);
				imwrite(filename, frame);
				break;
		}
	}
	return 0;
}
