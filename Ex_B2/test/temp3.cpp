#include "opencv2/highgui/highgui.hpp"

using namespace cv;

int main()
{
	Mat image;

	VideoCapture cap;      //capture的宣告
	cap.open(0);           //0 為預設camera
	
	printf("test \n");
	while(cap.isOpened())  //確認camera能開啓
	{
		printf("test2 \n");
		cap>>image;			//截取影像到image裡方法1
							//cap.read(image); //截取影像到image裡方法2
							//以上兩種方法都可以用，實測過沒問題!
													        
		imshow("Webcam live", image);
		
		waitKey(33);//避免CPU負荷，給點delay時間
					//實際上一般webcam的framerate差不多33ms
	}
	return 0;
}
