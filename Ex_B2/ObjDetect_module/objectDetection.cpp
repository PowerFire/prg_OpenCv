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
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String mouth_cascade_name = "haarcascade_smile.xml";

// 階梯式類器別宣告
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier mouth_cascade;

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

	if( !eyes_cascade.load(eyes_cascade_name) ) {
		printf("!! 無法載入眼睛 !!\n");
		getchar();
		return -1;
	}

	if( !mouth_cascade.load(mouth_cascade_name) ) {
		printf("!! 無法載入嘴巴 !!\n");
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

		Mat faceROI = frame_gray(faces[i]);

		//在臉內偵測眼睛
		std::vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(20, 20));
		
		for (size_t j = 0; j < eyes.size(); j++) {
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);

			printf("繪眼, 眼睛數量 %d\n\n\n", eyes.size());
		}

		//在臉內偵測嘴巴
		std::vector<Rect> mouth;
		mouth_cascade.detectMultiScale(faceROI, mouth, 1.1, 2, 20 | CASCADE_SCALE_IMAGE, Size(30, 30));
		for (size_t j = 0; j < mouth.size(); j++) {
			Point mouth_center(faces[i].x + mouth[j].x + mouth[j].width / 2,faces[i].y + mouth[j].y + mouth[j].height / 2);
			int radius = cvRound((mouth[j].width + mouth[j].height)*0.25);
			circle(frame, mouth_center, radius, Scalar(0, 255, 0), 4, 8, 0);

			printf("繪嘴巴, 嘴巴數量 %d\n\n\n", eyes.size());
		}
	}
	//顯示結果
	imshow(window_name, frame);

}

