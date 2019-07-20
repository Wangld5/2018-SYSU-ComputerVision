#include "cannycimg.h"
#include <stack>
cannyImg::cannyImg(string filename){
	img.load(filename.c_str());
	if (!img.data()){
		cout << "Could't open or find the image" << endl;
		exit(1);
	}

	vector<vector<double>> filter = createFilter(3, 3, 1);
	for (size_t i = 0; i<filter.size(); i++){
		for (size_t j = 0; j<filter[i].size(); j++){
			cout << filter[i][j] << " ";
		}
	}

	grayscaled = GetGrayScale(img);
	gFiltered = useFilter(grayscaled, filter);
	sFiltered = sobel();
	non = nonMaxSupp();
	thres = threshold(non, 20, 40);

	
    img.display("Original");
    grayscaled.display("GrayScaled");
    gFiltered.display("Gaussian Blur");
    sFiltered.display("Sobel Filtered");
    non.display("Non-Maxima Supp");
    thres.display("Final");
    link = linkEdge(non, 20, 40);
    link.display("link");

}
//获取灰度图
CImg<unsigned char> cannyImg::GetGrayScale(CImg<unsigned char> & img){
	CImg<unsigned char> res(img.width(), img.height(), 1, 1);
	cimg_forXY(img, x, y){
		int r = img(x,y,0);
		int g = img(x,y,1);
		int b = img(x,y,2);

		int newValue = int(r * 0.2126 + g * 0.7152 + b * 0.0722);
		res(x, y) = newValue;

	}
	return res;
}
//高斯滤波
vector<vector<double>> cannyImg::createFilter(int row, int col, double sigmaIn)
{
    //初始化一个二维数组
	vector<vector<double>> filter(row, vector<double>(col, -1));

	float coordSum = 0;
	float constant = 2.0 * sigmaIn * sigmaIn;

	// Sum is for normalization
	float sum = 0.0;
    //卷积过程
	for (int x = - row/2; x <= row/2; x++)
	{
		for (int y = -col/2; y <= col/2; y++)
		{
			coordSum = (x*x + y*y);
			filter[x + row/2][y + col/2] = (exp(-(coordSum) / constant)) / (M_PI * constant);//二维高斯卷积核
			sum += filter[x + row/2][y + col/2];
		}
	}

	// 归一化
	for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            filter[i][j] /= sum;
    //生成滤波
	return filter;
}
//使用高斯滤波矩阵对图像处理
CImg<unsigned char> cannyImg::useFilter(CImg<unsigned char>& img_in, vector<vector<double>> & filterIn){
    int size = (int)filterIn.size()/2;
	CImg<unsigned char> filteredImg(img_in.width() - 2 * size, img_in.height() - 2 * size);

	for (int i = size; i < img_in.width() - size; i++){
		for (int j = size; j < img_in.height() - size; j++){
			double sum = 0;
			for (size_t x = 0; x < filterIn.size(); x++){
				for (size_t y = 0; y < filterIn.size(); y++){
					sum += filterIn[x][y] * (double)img_in(i + x - size, j + y - size);
				}
			}
            filteredImg(i - size, j - size) = sum;
		}
	}
	return filteredImg;
}
//sobel算子
CImg<unsigned char> cannyImg::sobel(){

    //Sobel X Filter
    double x1[] = {-1.0, 0, 1.0};
    double x2[] = {-2.0, 0, 2.0};
    double x3[] = {-1.0, 0, 1.0};

    vector<vector<double>> xFilter(3);
    xFilter[0].assign(x1, x1+3);
    xFilter[1].assign(x2, x2+3);
    xFilter[2].assign(x3, x3+3);
    
    //Sobel Y Filter
    double y1[] = {1.0, 2.0, 1.0};
    double y2[] = {0, 0, 0};
    double y3[] = {-1.0, -2.0, -1.0};
    
    vector<vector<double>> yFilter(3);
    yFilter[0].assign(y1, y1+3);
    yFilter[1].assign(y2, y2+3);
    yFilter[2].assign(y3, y3+3);
    
    //Limit Size
    int size = (int)xFilter.size()/2;

	int w = gFiltered.width() - 2 * size;
	int h = gFiltered.height() - 2 * size;

	CImg<unsigned char> filteredImg(w, h);
	angles = CImg<unsigned char>(w, h);
    
	for (int i = size; i < gFiltered.width() - size; i++)
	{
		for (int j = size; j < gFiltered.height() - size; j++)
		{
			double sumx = 0;
            double sumy = 0;
            
			for (size_t x = 0; x < xFilter.size(); x++)
				for (size_t y = 0; y < xFilter.size(); y++)
				{
                    sumx += xFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_X Filter Value
                    sumy += yFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_Y Filter Value
				}
            double sumxsq = sumx*sumx;
            double sumysq = sumy*sumy;
            
            double sq2 = sqrt(sumxsq + sumysq);
            
            if(sq2 > 255) //Unsigned Char Fix
                sq2 =255;
            filteredImg(i-size, j-size) = sq2;
 
            if(sumx==0) //Arctan Fix
                angles(i-size, j-size) = 90;
            else
                angles(i-size, j-size) = atan(sumy/sumx);
		}
	}
    
    return filteredImg;
}
//非最大化抑制
CImg<unsigned char> cannyImg::nonMaxSupp(){
    CImg<unsigned char> nonMaxSupped(sFiltered.width() - 2 , sFiltered.height() - 2);
    for (int i=1; i< sFiltered.width() - 1; i++) {
        for (int j=1; j<sFiltered.height() - 1; j++) {
            float Tangent = angles(i,j);

            nonMaxSupped(i-1, j-1) = sFiltered(i,j);
            //Horizontal Edge，东西方向即边缘在南北方向
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered(i,j) < sFiltered(i,j+1)) || (sFiltered(i,j) < sFiltered(i,j-1)))
                    nonMaxSupped(i-1, j-1) = 0;
            }
            //Vertical Edge，南北方向即边缘在东西
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered(i,j) < sFiltered(i+1,j)) || (sFiltered(i,j) < sFiltered(i-1,j)))
                    nonMaxSupped(i-1, j-1) = 0;
            }
            
            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered(i,j) < sFiltered(i-1,j+1)) || (sFiltered(i,j) < sFiltered(i+1,j-1)))
                    nonMaxSupped(i-1, j-1) = 0;
            }
            
            //45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered(i,j) < sFiltered(i+1,j+1)) || (sFiltered(i,j) < sFiltered(i-1,j-1)))
                    nonMaxSupped(i-1, j-1) = 0;
            }
        }
    }
    return nonMaxSupped;
}
//双阈值
CImg<unsigned char> cannyImg::threshold(CImg<unsigned char>& imgin, int low, int high){
    if(low > 255)
        low = 255;
    if(high > 255)
        high = 255;
    
    CImg<unsigned char> EdgeMat(imgin.width(), imgin.height());
    
    for (int i=0; i<imgin.width(); i++) 
    {
        for (int j = 0; j<imgin.height(); j++) 
        {
            EdgeMat(i,j) = imgin(i,j);
            if(EdgeMat(i,j) > high)
                EdgeMat(i,j) = 255;
            else if(EdgeMat(i,j) < low)
                EdgeMat(i,j) = 0;
            else
            {
                bool anyHigh = false;
                bool anyBetween = false;
                //检测周围9个像素
                for (int x=i-1; x < i+2; x++) 
                {
                    for (int y = j-1; y<j+2; y++) 
                    {
                        if(x <= 0 || y <= 0 || EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
                            continue;
                        else
                        {
                            //检测出强边缘
                            if(EdgeMat(x,y) > high)
                            {
                                EdgeMat(i,j) = 255;
                                anyHigh = true;
                                break;
                            }
                            else if(EdgeMat(x,y) <= high && EdgeMat(x,y) >= low)
                                anyBetween = true;
                        }
                    }
                    if(anyHigh)
                        break;
                }
                //如果是弱边缘或者是基于两者之间
                if(!anyHigh && anyBetween)
                //检测周围20个像素
                    for (int x=i-2; x < i+3; x++) 
                    {
                        for (int y = j-1; y<j+3; y++) 
                        {
                            if(x < 0 || y < 0 || x > EdgeMat.width() || y > EdgeMat.height()) //Out of bounds
                                continue;
                            else
                            {
                                if(EdgeMat(x,y) > high)
                                {
                                    EdgeMat(i,j) = 255;
                                    anyHigh = true;
                                    break;
                                }
                            }
                        }
                        if(anyHigh)
                            break;
                    }
                if(!anyHigh)
                    EdgeMat(i,j) = 0;
            }
        }
    }
    return EdgeMat;

}
//使用深度搜索连接边缘点
CImg<unsigned char> cannyImg::linkEdge(CImg<unsigned char>& imageIn, int low, int high){
    CImg<unsigned char> finalEdge = CImg<unsigned char>(imageIn.width(), imageIn.height());
    vector<vector<int>> flag(imageIn.width(), vector<int>(imageIn.height(), 0));

    cimg_forXY(imageIn, x, y){
        finalEdge(x, y) = imageIn(x, y);
    }
    cout<<"initialize finish"<<endl;
    int connect = 0;
    stack<pair<int, int>> possiblePoint;
    stack<pair<int, int>> linkPoint;

    cimg_forXY(imageIn, i, j){
        if(non(i, j) > low && non(i, j) < high){
            possiblePoint.push(make_pair(i,j));
            linkPoint.push(make_pair(i, j));
            flag[i][j] = 1;
            //8邻域检测是否有弱边缘点
            while(!possiblePoint.empty()){
                pair<int, int> temp = possiblePoint.top();
                possiblePoint.pop();
                for(int x=temp.first-1; x<=temp.first+1; x++){
                    for(int y=temp.second-1; y<=temp.second+1; y++){
                        if(x<0 || y<0 || x>=imageIn.width() || y>=imageIn.height()){
                            continue;
                        }
                        if(non(x, y) > low && non(x, y) < high && flag[x][y] == 0){
                            possiblePoint.push(make_pair(x,y));
                            linkPoint.push(make_pair(x, y));
                            flag[x][y] = 1;
                        }
                        if(finalEdge(x, y) == 255){
                            connect = 1;
                        }
                    }
                }
            }

            if(connect == 1){
                while(!linkPoint.empty()){
                    pair<int, int> temp = linkPoint.top();
                    linkPoint.pop();
                    finalEdge(temp.first, temp.second) = 255;
                }
                connect = 0;
            }
            else{
                while(!linkPoint.empty()){
                    pair<int, int> temp = linkPoint.top();
                    linkPoint.pop();
                    flag[temp.first][temp.second] = 0;
                }
            }
        }
    }
    for(int i=0; i<imageIn.width(); i++){
        for(int j=0; j<imageIn.height(); j++){
            flag[i][j] = 0;
        }
    }
    cimg_forXY(imageIn, i, j){
        if(finalEdge(i, j) == 255){
            if(flag[i][j] == 1){
                continue;
            }
            flag[i][j] = 1;
            possiblePoint.push(make_pair(i,j));
            linkPoint.push(make_pair(i, j));
            int length = 1;
            while(!possiblePoint.empty()){
                pair<int, int> temp = possiblePoint.top();
                possiblePoint.pop();
                for(int x=temp.first-1; x<=temp.first+1; x++){
                    for(int y=temp.second-1; y<=temp.second+1; y++){
                        if(x<0 || y<0 || x>imageIn.width() || y>imageIn.height()){
                            continue;
                        }
                        else if(finalEdge(x, y) == 255 && flag[x][y] == 0){
                            possiblePoint.push(make_pair(x,y));
                            linkPoint.push(make_pair(x, y));
                            flag[x][y] = 1;
                            length++;
                        }
                    }
                }
            }
            if(length < 20){
                while(!linkPoint.empty()){
                    pair<int, int> temp = linkPoint.top();
                    linkPoint.pop();
                    finalEdge(temp.first, temp.second) = 0;
                }
            }
            else{
                while(!linkPoint.empty()){
                    linkPoint.pop();
                }
            }
        }
    }
    return finalEdge;
}