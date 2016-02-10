#include <iostream>
#include <fstream>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

inline bool isFlowCorrect(Point2f u)
{
	return !cvIsNaN(u.x) && !cvIsNaN(u.y) && fabs(u.x) < 1e9 && fabs(u.y) < 1e9;
}

static Vec3b computeColor( float fx, float fy)
{
	static bool first = true;

	//顏色轉移相對長度
	//這些顏色的選定基於認知的相似性(perceptual similarity)
	//例如：紅色黃色比黃與綠色好區分色度(shades)
	const int RY =15;
	const int YG =6;
	const int GC =4;
	const int CB =11;
	const int BM =13;
	const int MR =6;

	const int NCOLS = RY+ YG +GC +CB +BM +MR;
	static Vec3i colorWheel[NCOLS];

	if (first) {
		int k = 0;

        for (int i = 0; i < RY; ++i, ++k) {
            colorWheel[k] = Vec3i(255, 255 * i / RY, 0);
		}

		for (int i = 0; i < YG; ++i, ++k) {
			colorWheel[k] = Vec3i(255 - 255 * i / YG, 255, 0);
		}
		
		for (int i = 0; i < GC; ++i, ++k) {
			colorWheel[k] = Vec3i(0, 255, 255 * i / GC);
		}
		
		for (int i = 0; i < CB; ++i, ++k) {
			colorWheel[k] = Vec3i(0, 255 - 255 * i / CB, 255);
		}
		
		for (int i = 0; i < BM; ++i, ++k) {
			colorWheel[k] = Vec3i(255 * i / BM, 0, 255);
		}
		
		for (int i = 0; i < MR; ++i, ++k) {
			colorWheel[k] = Vec3i(255, 0, 255 - 255 * i / MR);
		}
		first = false;
	}

	const float rad =sqrt(fx*fx+fy*fy);
	const float a = atan2(-fy, -fx)/ (float)CV_PI;

	const float fk = (a+1.0f) / 2.0f* (NCOLS -1);
	const int k0 = static_cast<int>(fk);
	const int k1 = (k0+1)%NCOLS;
	const float f = fk -k0;

	Vec3b pix;

	for (int b =0 ; b <3; b++) {
		const float col0 =colorWheel[k0][b] /255.f;
		const float col1 =colorWheel[k1][b] /255.f;
		
		float col =(1-f)*col0+f*col1;
		if(rad <=1) {
			//以半徑增加飽和度
			col = 1 -rad *(1-col);
		}else {
			//超出範圍
			col *= .75;
		}
		pix[2-b] =static_cast<uchar>(255.f*col);
	}
	return pix;
}

static void drawOpticalFlow(const Mat_<Point2f>& flow, Mat & dst, float maxmotion= -1)
{
	dst.create(flow.size(), CV_8UC3);
	dst.setTo(Scalar::all(0));

	//determine motion range:
	float maxrad =maxmotion;

	if (maxmotion <=0) {
		maxrad = 1 ;
		for(int y=  0; y< flow.rows; ++y) {
			for (int x= 0; x< flow.cols; ++x) {
				Point2f u= flow(y,x);

				if (!isFlowCorrect(u)) {
					continue ;
				}
				maxrad =max(maxrad, sqrt(u.x*u.x+u.y*u.y));
			}
		}
	}
	
	for(int y=0; y<flow.rows; ++y){
		for (int x = 0; x < flow.cols; ++x) {
			Point2f u =flow(x,y);
			if(isFlowCorrect(u)) {
				dst.at<Vec3b>(y,x) = computeColor(u.x/maxrad, u.y/maxrad);
			}
		}
	}
}

//光流資料明細的二元擋案格式請參考
//http:://vision.middlebury.edu/flow/data
static void writeOpticalFlowToFile(const Mat_<Point2f>& flow, const string& fileName)
{
	static const char FLO_TAG_STRING[]= "PIEH";
	ofstream file(fileName.c_str(), ios_base::binary);

	file << FLO_TAG_STRING;
	file.write((const char * ) &flow.cols, sizeof(int));
	file.write((const char * ) &flow.rows, sizeof(int));

    for (int i = 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            const Point2f u = flow(i, j);

            file.write((const char*) &u.x, sizeof(float));
            file.write((const char*) &u.y, sizeof(float));
        }
    }
}

int main(int argc, const char* argv[])
{
	VideoCapture cap("car.mp4");
	if (!cap.isOpened())
		return -1;

	Mat frame0, frame1;

	// 設定影格位置
	unsigned int fIdx = 55;
	cap.set(CV_CAP_PROP_POS_FRAMES, fIdx);
	cap >> frame0;

	fIdx = 160;
	cap.set(CV_CAP_PROP_POS_FRAMES, fIdx);
	cap >> frame1;

	imshow("frame0", frame0);
	imshow("frame1", frame1);

	// 影格從BGR轉成灰階
	cvtColor(frame0, frame0, CV_BGR2GRAY);
	cvtColor(frame1, frame1, CV_BGR2GRAY);

    Mat_<Point2f> flow;
    Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();

    const double start = (double)getTickCount();
    tvl1->calc(frame0, frame1, flow);
    const double timeSec = (getTickCount() - start) / getTickFrequency();
    cout << "移動時間 : " << timeSec  << "秒" << endl;

    Mat out;
    drawOpticalFlow(flow, out);

	// 原程式用argv輸入圖檔, 所以才有argc
	// 若要設定，可指定argc = 4
    if (argc == 4)
        writeOpticalFlowToFile(flow, argv[3]);

    imshow("Flow", out);
    waitKey();
	
	return 0;
}
