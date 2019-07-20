#include <iostream>
#include <string>
#include <vector>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

class EdgeDetect2{
    private:
        int circleNumber; // 检测圆个数
        int minRadius; // 圆周最小半径
        int maxRadius; // 圆周最大半径
        vector<pair<int, int>> circles; // 检测到的圆心集合
        vector<pair<int, int>> voteSet; // 投票集合
        vector<pair<int, int>> center; // 存放累加值最大的圆心对应坐标
        vector<int> circleWeight; // 累加矩阵
        vector<int> sortCircleWeight; // 从大到小排序的累加矩阵

        CImg<float> thresholdImage;
        CImg<float> houghImage; // 霍夫空间图像
        CImg<float> outputImage; // 霍夫变换检测出来的图像
        vector<float> setSin; // sin集合
        vector<float> setCos; // cos集合
    public:
        void houghCirclesTransform(CImg<float>&, int, int); // 霍夫圆变换
        void houghCirclesDetect(); // 霍夫圆检测
        void drawCircle(int); // 描绘检测出的圆形
        EdgeDetect2(string, string, int, int, int);
};