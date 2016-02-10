#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>

using namespace cv ;
using namespace std;

void createAlphaMat(Mat &mat)
{
	for(int i=0; i< mat.rows; ++i) {
		for(int j=0; j<mat.cols; ++j){
			Vec4b &rgba = mat.at<Vec4b>(i,j);
			rgba[0] = UCHAR_MAX;
			rgba[1] = saturate_cast<uchar>((float (mat.cols -j) )/((float) mat.cols)*UCHAR_MAX);
			rgba[2] = saturate_cast<uchar>((float (mat.cols -i) )/((float) mat.rows)*UCHAR_MAX);
			rgba[3] = saturate_cast<uchar>(0.5*(rgba[1]+rgba[2]));
		}
	}
}

int main()
{
	// 建立帶Alpha色版的Mat
	Mat mat(480, 640, CV_8UC4);
	createAlphaMat(mat);

	vector<int>compression_params;

	//此句程式碼的OpenCv 2版為
	//compression_param.push_back(CV_IMWRITE_PNG_COMPRESSION);
	//此句程式碼為OpenCv 3版
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	try {
		imwrite("透明Alpha值圖.png", mat, compression_params);
		imshow("產生的PNG圖",mat);
		fprintf( stdout, "PNG圖片擋的alpha資料儲存完畢~\n");
		cout << "可在專案目錄下查看由imwrite函數產生的圖片\n";
	}
	catch(runtime_error & ex) {
		cerr << "影像轉換成PNG格式發生錯誤: %s\n" << ex.what();
		return 1;
	}

	return 0;
}
