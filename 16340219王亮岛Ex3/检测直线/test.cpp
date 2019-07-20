#include "EdgeDetect.hpp"

int main() {
	string result, source, link;
	cin>>source>>result;
	int point, wind;
	cin>>point>>wind;
	EdgeDetect *edgeDetect1 = new EdgeDetect(source, result, point, wind);
	system("pause");
	return 0;
}