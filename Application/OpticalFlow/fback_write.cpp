#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap,
	int step, double, const Scalar & color)
{
	for(int y=0; y<cflowmap.rows; y+=step) {
		for(int x=0; x<cflowmap.cols; x+=step) {
			const Point2f& fxy = flow.at<Point2f>(y,x);

			//移動方向
			line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);

			//綠色綠色固定點
			circle(cflowmap, Point(x,y), 2, color, -1);
		}
	}
}

int main(int, char**)
{
	VideoCapture cap(0);
	if( !cap.isOpened()) {
		return -1;
	}

	Mat prevgray, gray, flow, cflow, frame;

	//取得影像影格大小
	Size S =Size( (int) cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	//建立並初始影片儲存物件
	VideoWriter put("fback_write.mpg", CV_FOURCC('M', 'P', 'E', 'G') ,30 ,S);
	namedWindow("flow", 1);
	for(;;) {
		cap >> frame;

		cvtColor(frame, gray, COLOR_BGR2GRAY);

		if(prevgray.data) {
			//使用Gunnar Farneback演算法計算密集的光流(option flow)
			calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2,0);

			cvtColor(prevgray, cflow, COLOR_GRAY2BGR);

			//繪製綠點
			drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
			imshow("flow", cflow);
			put << cflow;
		}

		if(waitKey(30)>=0) {
			break;
		}

		//影像互換
		swap(prevgray, gray);
	}
	return 0;
}
