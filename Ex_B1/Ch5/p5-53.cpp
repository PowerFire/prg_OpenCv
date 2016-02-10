#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;
using namespace std;

int main()
{
	//初始化
	FileStorage fs2("test.xml", FileStorage::READ);

	//第一種方法,對FileNode操作
	int frameCount = (int)fs2["frameCount"];
	
	std::string date;
	//第二種方法,使用FileNode運算子 >>
	fs2["calibrationDate"] >> date;

	Mat cameraMatrix2, distCoeffs2;
	fs2["cameraMatrix"] >> cameraMatrix2;
	fs2["distCoffs"] >> distCoeffs2;
	
	cout << "frameCount: " << frameCount << endl;
	cout << "calibration date: " << date << endl;
	cout << "camera matrix: " << cameraMatrix2 << endl;
	cout << "distortion coeffs: " << distCoeffs2 << endl;

	FileNode features = fs2["features"];
	FileNodeIterator it = features.begin(), it_end= features.end();
	int idx=0;
	std::vector<uchar> lbpval;

	//使用FileNodeIterator 瀏覽序列
	for (; it!=it_end; ++it, idx++) {
		cout << "feature #" << idx << ": ";
		cout << "x= " << (int)(*it)["x"] << ", y=" << (int)(*it)["y"];
		cout << ", lbp: (";
		//我們也可以使用filenode >> std::vector操作符號來輕易地讀數值陣列
		(*it)["lbp"] >> lbpval;
		for( int i= 0; i < (int)lbpval.size(); i++) {a
			cout << " " << (int)lbpval[i];
		}
		cout << ")" << endl;
	}

	fs2.release();

	//程式結束,輸出一些說明文字
	printf("\n read end\n");

	getchar();

	return 0;


}
