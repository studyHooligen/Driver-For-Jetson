#include <iostream>
#include <vector>
#include <time.h>

#include "opencv.hpp"   //OpenCV（主）头文件
#include <Dense>

#include "IMX219.hpp"

using namespace std;    //引入命名空间
using namespace cv;

// #define __USER__DEBUG__

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

    Mat grayImg,fltImg,edgeImg;

    auto timePre = clock();
    auto timeNow = timePre;

    Eigen::Vector3d detectPre(0,0,0);

    while(1)
    {
        cam >> capImg;   //读取视频流
        timeNow = clock();  //获取系统时间
        #ifdef __USER__DEBUG__
        imshow("camera",capImg);   //显示图像
        waitKey(1); //按键更新
        #endif

        cvtColor(capImg,grayImg,COLOR_BGR2GRAY);    //转灰度图
        #ifdef __USER__DEBUG__
        imshow("gray Image", grayImg);
        waitKey(0);
        destroyWindow("gray Image");
        #endif

        GaussianBlur(grayImg,fltImg,Size(9,9),4,4); //高斯滤波
        #ifdef __USER__DEBUG__
        imshow("filter Image", fltImg);
        waitKey(0);
        destroyWindow("filter Image");
        #endif

        Canny(fltImg,edgeImg,20,20);    //边缘检测
        #ifdef __USER__DEBUG__
        imshow("edge detect",edgeImg);
        waitKey(0);
        destroyWindow("edge detect");
        #endif

        vector<Vec3f> circles;
        HoughCircles(fltImg,circles,
                HOUGH_GRADIENT,
                1,fltImg.rows/4,
                10,20,0,fltImg.rows/4); //霍夫圆检测

        if(circles.empty())
        {
            cout<<"didn't detect circle"<<endl;
            return -1;
        }

        #ifdef __USER__DEBUG__
        for(auto i = 0;i < circles.size(); i++)
        {
            Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle(capImg, center, 3, Scalar(0, 255, 0), -1, 8, 0);

            circle(capImg, center, radius, Scalar(155, 50, 255), 3, 8, 0);
        }

        imshow("resault",capImg);
        destroyAllWindows();
        #endif

        Eigen::Vector3d position(circles[0][0],circles[0][1],1);    //齐次像素坐标系[x,y,1]^T
        Eigen::Matrix3d cameraKmatrix;  //内参矩阵K
        double z = 1;   //物距z
        Eigen::Vector3d res = z*cameraKmatrix * position;   //求解相机坐标系[X,Y,Z]^T

        auto timeDiv = timeNow - timePre;   //时间差
        Eigen::Vector3d distance = res - detectPre; //求解位移
        auto velocity = distance.norm()/timeDiv;    //求解速度
    }

    return 0;
}