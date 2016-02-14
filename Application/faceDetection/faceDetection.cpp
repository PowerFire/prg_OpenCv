#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// 宣告函式
void detectAndDisplay(Mat frame);

// 全域變數
String face_cascade_name = "haarcascade_frontalface_alt.xml";

// 階梯式類器別宣告
CascadeClassifier face_cascade;

String window_name = "臉部偵測";

// 主程式
int main(void)
{
	VideoCapture cap;
	Mat frame;

	// 1. 載入全域變數
	if( !face_cascade.load(face_cascade_name) ) {
		printf("!! 無法載入臉部 !!\n");
		getchar();
		return -1;
	}

	// 2. 開啟攝影機並讀取攝像
	cap.open(0);
	if( !cap.isOpened()) {
		printf( "無法開啟攝影機\n" );
		getchar();
		return -1;
	}

	//產生影片擋- step 1 取得影像影格
	Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT) );

	//產生影片擋- step 2 建立並初始影片儲存物件
	VideoWriter put("Face.mpg", CV_FOURCC('M', 'P', 'E', 'G'), 30, S);
	
	// 讀取攝像
	while (cap.read(frame)) {
		if( frame.empty()) {
			printf("無法讀取攝像\n");
			getchar();
			break;

		}

		// 3. 用階梯式類別做攝像處理
		detectAndDisplay(frame);

		int c = waitKey(10);
		//按esc鍵退出
		if( (char)c==27) {
			break;
		}

		//產生影片擋- step 3 存檔
		put << frame;
	}
	return 0;
}

// 函式 detectAndDisplay 實體
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	//將攝像轉成灰階
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	//灰階圖作值條式的平等化
	equalizeHist(frame_gray, frame_gray);

	//偵測臉
	face_cascade.detectMultiScale(frame_gray,faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2,faces[i].height / 2), 0, 0, 360,Scalar(255, 0, 255), 4, 8, 0);
		printf("繪臉, 臉的大小 %d..............\n", faces.size());
	}
	//顯示結果
	imshow(window_name, frame);

}

