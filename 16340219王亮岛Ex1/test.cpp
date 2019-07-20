#include "draw.hpp"


int main(){
	Draw test1;
	test1.test_change();
	test1.draw_circle_myself(50,50,30,0, 0, 255);
	test1.draw_circle_myself(50,50,3,200, 155, 0);
	test1.draw_line_myself();
	test1.display();
	CImg<unsigned char> temp = test1.getSrcImg();
	temp.save("2.bmp");
	Draw test2;
	test2.test_change();
	test2.draw_circle_auto(50, 50, 30, 0, 0, 255);
	test2.draw_circle_auto(50, 50, 3, 200, 155, 0);
	test2.draw_line_auto(0,0,0,0,255);
	test2.display();
	test2.getSrcImg().save("2.1.bmp");
}
	
