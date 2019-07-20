
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "CImg.h"
using namespace std;
using namespace cimg_library;



class cannyImg{
    public:
        cannyImg(string infileName);
    private:
        CImg<unsigned char> GetGrayScale(CImg<unsigned char>&);
	    vector<vector<double>> createFilter(int row, int col, double sigmaIn); //Creates a gaussian filter
	    CImg<unsigned char> useFilter(CImg<unsigned char>&, vector<vector<double>>&); //Use some filter
        CImg<unsigned char> sobel(); //Sobel filtering
        CImg<unsigned char> nonMaxSupp(); //Non-maxima supp.
        CImg<unsigned char> threshold(CImg<unsigned char>& imageIn, int low, int high); //Double threshold and finalize picture
        CImg<unsigned char> linkEdge(CImg<unsigned char>& imageIn, int low, int high);
    private:
        CImg<unsigned char> img;
        CImg<unsigned char> grayscaled;
        CImg<unsigned char> angles;
        CImg<unsigned char> non;
        CImg<unsigned char> thres;
        CImg<unsigned char> link;
        CImg<unsigned char> gFiltered;
        CImg<unsigned char> sFiltered;
};
