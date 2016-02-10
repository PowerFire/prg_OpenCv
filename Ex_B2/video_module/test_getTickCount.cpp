#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{    
    double t1, t2;
    t1 = getTickCount();
    sleep(1);
	t2 = getTickCount();
	printf("Lasting Time: %lf\n",(t2-t1)/1000.0);
	printf("TickFrequency: %lf\n",getTickFrequency());
	printf("Lasting Time: %lf\n",(t2-t1)/ getTickFrequency());
	return 0;
}
