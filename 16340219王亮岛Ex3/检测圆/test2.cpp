#include "EdgeDetect2.hpp"

int main() {
	string result, source;
	cin>>source>>result;
	int point;
	cin>>point;
    int minR, maxR;
    cin>>minR>>maxR;
	EdgeDetect2 *edgeDetect2 = new EdgeDetect2(source, result, point, minR, maxR);
	system("pause");
	return 0;
}