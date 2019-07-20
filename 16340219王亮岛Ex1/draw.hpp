
#include <iostream>
#include <string>
#include <cmath>
using namespace std;
#include "CImg.h"
using namespace cimg_library;
const double pi = 3.1415926; 
class Draw{
	private:
		CImg<unsigned char> SrcImg;	
	public:
		Draw(){
			SrcImg.load_bmp("1.bmp");
		}
		void display(){
			SrcImg.display();
		}
		void test_change(){
			cimg_forXY(SrcImg, x, y){//白色变红 
				if(SrcImg(x, y, 0) == 255 && SrcImg(x, y, 1) == 255 && SrcImg(x, y, 2) == 255){
					SrcImg(x, y, 0) = 255;
					SrcImg(x, y, 1) = 0;
					SrcImg(x, y, 2) = 0;
				}
			}
			cimg_forXY(SrcImg, x, y){//黑色变绿 
				if(SrcImg(x, y, 0) == 0 && SrcImg(x, y, 1) == 0 && SrcImg(x, y, 2) == 0){
					SrcImg(x, y, 0) = 0;
					SrcImg(x, y, 1) = 255;
					SrcImg(x, y, 2) = 0;
				}
			}
			SrcImg.display();
		}
		void draw_circle_myself(int x0, int y0, int radius, int R, int G, int B){
			cimg_forXY(SrcImg, x, y){
				if(sqrt(pow((x-x0), 2) + pow((y-y0), 2)) <= radius){
					SrcImg(x, y, 0) = R;
					SrcImg(x, y, 1) = G;
					SrcImg(x, y, 2) = B;
				}
			}
			SrcImg.display();
		}
		void draw_circle_auto(int x0, int y0, int radius, int R, int G, int B){
			unsigned char color[3];
			color[0] = R, color[1] = G, color[2] = B;
			SrcImg.draw_circle(x0, y0, radius, color);
			SrcImg.display();
		}
		int cmp(double x, double y){
			if(abs(x-y) <= 0.5){
				return 1;
			}
			return 0;
		}
		void draw_line_myself(){
			int x0 = 100*cos(35*pi/180);
			int y0 = 100*sin(35*pi/180);
			cimg_forXY(SrcImg, x, y){
				if(x == 0 && y == 0){
					SrcImg(x, y, 0) = 0;
					SrcImg(x, y, 1) = 0;
					SrcImg(x, y, 2) = 255;
				}
				else if(cmp((double)y, (double)x*tan(35*pi/180)) && (double)x <= x0 && (double)y <= y0){
					SrcImg(x, y, 0) = 0;
					SrcImg(x, y, 1) = 0;
					SrcImg(x, y, 2) = 255;
				}
			}
		}
		void draw_line_auto(int x0, int y0, int R, int G, int B){
			unsigned char color[3];
			color[0] = R, color[1] = G, color[2] = B;
			SrcImg.draw_line(x0, y0, 100*cos(35*pi/180), 100*sin(35*pi/180), color);
		}
		CImg<unsigned char> getSrcImg(){
			return SrcImg;
		}
};
