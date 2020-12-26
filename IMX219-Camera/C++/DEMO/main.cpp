#include <iostream>
#include <vector>
#include <time.h>

#include "opencv.hpp"   //OpenCV（主）头文件

#include "IMX219.hpp"

using namespace std;    //引入命名空间
using namespace cv;

#define __USER__DEBUG__

#define WIDTH 1280
#define HEIGHT 720
#define FPS 30

int main()
{
    Mat capImg;    //像素矩阵变量

    std::string pipeline = get_tegra_pipeline(WIDTH, HEIGHT, FPS);
    cv::VideoCapture cam(pipeline, cv::CAP_GSTREAMER);

    if(!cam.isOpened()) //检查是否成功打开相机
    {
        std::cout<<"Open Failed!"<<std::endl;
        return -1;
    }
    std::cout<<"open Success!"<<std::endl;

    while(1)
    {
        cam >> capImg;   //读取视频流
        #ifdef __USER__DEBUG__
        imshow("camera",capImg);   //显示图像
        waitKey(1); //按键更新
        #endif

    }

    return 0;
}