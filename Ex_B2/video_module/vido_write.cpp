#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture cap(0);

	if( !cap.isOpened()) {
		cout << "無法啟動攝影機" << endl;
		return -1;
	}

	//取得影像格(frames)大小
	Size S = Size( (int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
			(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT) );
	
	//建立並初始影片儲存物件
	VideoWriter put("output.mpg", CV_FOURCC('M', 'P', 'E', 'G'), 30, S);

	if( !put.isOpened() ) {
		cout << "無法產生影片擋" << endl;
		return -1;
	}

	namedWindow("Video");

	//開始攝影
	while(char(waitKey(1)) != 'q' && cap.isOpened()) {
		Mat frame;
		cap >> frame;

		//檢查是否攝影機結束攝影
		if(frame.empty()) {
			break;
		}

		imshow("Video", frame);

		put << frame;
	}

	return 0;
}
