#include <iostream>
#include <string>
#include <vector>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

class EdgeDetect{
    private:
        CImg<float> thresholdImage;
        CImg<float> houghImage; // 霍夫空间图像
	    CImg<float> outputImage; // 霍夫变换检测出来的图像
        vector<float> setSin; // sin集合
        vector<float> setCos; // cos集合

        int windowSize;
        int lineNumber; 
        vector<pair<int, int>> lines; // 检测到的直线集合
        vector<int> lineWeight; // 直线的集合对应参数在霍夫空间的权重
        vector<int> sortLineWeight; // 从大到小排序的权重
        CImg<float> findPoint; // 用于检测角点
    public:
        void houghLinesTransform(CImg<float>&); // 霍夫直线变换
        void houghLinesDetect(); // 霍夫直线检测
        int getMaxHough(CImg<float>&, int&, int&, int&); // 计算霍夫空间直线交点
        void drawEdge(); // 描绘检测出的边缘
        void drawPoint(); // 描绘检测出的角点
        EdgeDetect(string, string, int, int);
};