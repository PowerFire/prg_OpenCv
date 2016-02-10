#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main()
{
	VideoCapture cap("Megamind.avi");

	if( !cap.isOpened() ) {
		cout << "無法啟動攝影機" << endl;
		return -1;
	}

	// 影格總數
	int frnb = cap.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "影格總數" << frnb << endl;

	//
	Mat frame;

	//起始影格數
	unsigned int fIdx = 10;

	//設定影片位置
	cap.set(CV_CAP_PROP_POS_FRAMES, fIdx);

	//讀取影格
	cap >> frame;

	//顯示影格
	namedWindow("frame", CV_WINDOW_AUTOSIZE);
	imshow("frame", frame);

	waitKey(3);

	cout << "請輸入 0 到" << frnb -1 << "的數字, 輸入0就結束\n";

	for(;;) {
		cout << "第幾個影格 ?" ;
		cin >> fIdx;
		
		if (fIdx > 0 && fIdx < frnb) {
			cap.set( CV_CAP_PROP_POS_FRAMES, fIdx);
			printf ("temp\n");
			cap >> frame;;
			imshow("frame", frame );
			waitKey(3);
		} else {
			return 0;
		}

	}
	return 0;
}
