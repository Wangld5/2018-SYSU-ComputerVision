#include "canny.h"

int main() {
	CImg<unsigned char> coin("13.bmp");
	Canny c;
	CImg<unsigned char> coin_canny = c.canny(coin, coin.width(), coin.height());
	coin_canny.save("13-line.bmp");

	return 0;
}