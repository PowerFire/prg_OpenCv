#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define Draw_OptFlowMap 0
#define Draw_Rectangle 1

#if Draw_OptFlowMap
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
#endif

#if Draw_Rectangle
int drawRectangle(Mat & inputMat, Mat& originalMat, float vx, float vy)
{
	   int largest_area=0;
       int largest_contour_index=0;
       Rect bounding_rect;
	   vector< vector<Point> > contours; // Vector for storing contour
       vector<Vec4i> hierarchy;

		//找輪廓
		findContours( inputMat, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
		       
		for( int i = 0; i< contours.size(); i++ ) { // iterate through each contour.
			double a=contourArea( contours[i],false);//  Find the area of contour
				if(a>largest_area) {
					largest_area=a;
						largest_contour_index=i;//Store the index of largest contour
						bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}
		}


		Scalar color( 255,255,255);
		//畫出輪廓
		drawContours( originalMat, contours,largest_contour_index, color, 1, 8, hierarchy ); // Draw the largest contour using previously stored index.
		//畫出矩形
		rectangle(originalMat, bounding_rect,Scalar(0,255,0),2, 8,0);
}

#endif



int main(int, char**)
{
	VideoCapture cap(0);
	if( !cap.isOpened()) {
		return -1;
	}

	Mat prevgray, gray, flow, cflow, frame;
	#if Draw_Rectangle
	Mat binaryPic,binaryPic2;
	#endif

	namedWindow("flow", 1);
	for(;;) {
		cap >> frame;

		cvtColor(frame, gray, COLOR_BGR2GRAY);

		if(prevgray.data) {
			//使用Gunnar Farneback演算法計算密集的光流(option flow)
			calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2,0);

			cvtColor(prevgray, cflow, COLOR_GRAY2BGR);

			//繪製綠點
			#if Draw_OptFlowMap
			drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
			#endif

			//繪製最小矩形
			#if Draw_Rectangle
			float diff_XY=0.0;
			int  numOfSelectedPoint = 0;
			float sumX = 0.0, sumY =0.0;
			
			binaryPic = Mat::zeros(prevgray.size(), prevgray.type());

			for(int y=0; y<cflow.rows; y+= 10) {
				for (int x = 0; x < cflow.cols; x += 10) {
					Point2f fxy = flow.at<Point2f>(y, x);
					//選取光流明顯的點
					diff_XY =abs(fxy.x)+abs(fxy.y);
					if (diff_XY > 6) {
						binaryPic.at<uchar> (y,x) = 128;

						numOfSelectedPoint++ ;
						sumX += fxy.x  ;
						sumY += fxy.y  ;
					}
				}
			}
			//OpenCV提供getStructuringElement()讓我們得到要進行侵蝕或膨脹的模板
			//http://monkeycoding.com/%E4%BE%B5%E8%9D%95%E3%80%81%E8%86%A8%E8%84%B9erode%E3%80%81dilate/
			Mat dilateElement = getStructuringElement( MORPH_RECT,Size(10,10));

			//膨脹函數
			dilate(binaryPic,binaryPic2,dilateElement);

			//畫矩形
			drawRectangle(binaryPic2, cflow, sumX/numOfSelectedPoint,sumY/numOfSelectedPoint);
			#endif
			imshow("flow", cflow);
		}

		if(waitKey(30)>=0) {
			break;
		}

		//影像互換
		swap(prevgray, gray);
	}
	return 0;
}
