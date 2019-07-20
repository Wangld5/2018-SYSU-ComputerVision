#include "EdgeDetect2.hpp"
#define PI 3.141592653
#define gFilterX 5
#define gFilterY 5
#define sigma 1
#define thresholdLow 120
#define thresholdHigh 140
#define thetaSize 360
#define windowSize 60


EdgeDetect2::EdgeDetect2(string input, string output, int number, int minR, int maxR){
    for (int i = 0; i < thetaSize; i++) {
		setSin.push_back(sin(2 * PI * i / thetaSize));
		setCos.push_back(cos(2 * PI * i / thetaSize));
	}
    thresholdImage.load(input.c_str());

    circleNumber = (maxR-minR)/5;
	minRadius = minR;
	maxRadius = maxR;

	outputImage = CImg<float>(thresholdImage._width, thresholdImage._height, 1, 3, 0);
	cimg_forXY(thresholdImage, x, y){
		if(thresholdImage(x, y) == 255){
			outputImage(x, y, 0) = 255;
			outputImage(x, y, 1) = 255;
			outputImage(x, y, 2) = 255;
		}
	}
	cout<<"change success"<<endl;
	
    houghCirclesTransform(thresholdImage, minRadius, maxRadius); // 霍夫圆变换
	outputImage.save(output.c_str());

}

void EdgeDetect2::houghCirclesTransform(CImg<float>& img, int minR, int maxR) {
	int width = img._width, height = img._height, max = 0;

	for (int r = minR; r < maxR; r += 5) {
		max = 0;
		houghImage = CImg<float>(width, height);
		houghImage.fill(0);
		cimg_forXY(img, x, y) {
			int value = img(x, y);
			if (value != 0) {
				for (int i = 0; i < thetaSize; i++) {
					int x0 = x - r * setCos[i];
					int y0 = y - r * setSin[i];
					if (x0 > 0 && x0 < width && y0 > 0 && y0 < height) {
						houghImage(x0, y0)++;
					}
				}
			}
		}
		//选择最满足的圆的r
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				if (houghImage(x, y) > max) {
					max = houghImage(x, y);
				}
			}
		}
		voteSet.push_back(make_pair(max, r));
	}
	houghImage.display("houghSpace");
	sort(voteSet.begin(), voteSet.end(), [](const pair<int, int>& x, const pair<int, int>& y) -> int {
		return x.first > y.first;
	});
    int AllCircleNumber = 0;
	for (int i = 0; i < circleNumber; i++) {
		if(voteSet[i].second > maxR || voteSet[i].second < minR) continue;
		houghImage = CImg<float>(width, height);
		houghImage.fill(0);
		cimg_forXY(img, x, y) {
			int value = img(x, y);
			if (value != 0) {
				for (int j = 0; j < thetaSize; j++) {
					int x0 = x - voteSet[i].second * setCos[j];
					int y0 = y - voteSet[i].second * setSin[j];
					/*进行voting投票*/
					if (x0 > 0 && x0 < width && y0 > 0 && y0 < height) {
						houghImage(x0, y0)++;
					}
				}
			}
		}
		cout << "The radius is " << voteSet[i].second << endl;
        AllCircleNumber++;
		houghCirclesDetect();
		drawCircle(voteSet[i].second);
	}
	outputImage.display("I2");
    cout<<"circles number is "<<AllCircleNumber<<endl;

    cimg_forXY(outputImage, x, y){
		if(outputImage(x, y, 0) == 0 && outputImage(x, y, 1) == 0 && outputImage(x, y, 2) == 255 && thresholdImage(x, y) == 255){
			outputImage(x, y, 0) = 255;
			outputImage(x, y, 1) = 0;
			outputImage(x, y, 2) = 0;
		}
	}
	outputImage.display("I3");
}

void EdgeDetect2::houghCirclesDetect() {
	/*将霍夫图像中所有不为0的点对应圆心的坐标存入数组*/
	cimg_forXY(houghImage, x, y) {
		if (houghImage(x, y) != 0) {
			circles.push_back(make_pair(x, y));
			circleWeight.push_back(houghImage(x, y));
		}
	}
}

void EdgeDetect2::drawCircle(int r) {


	int width = thresholdImage._width, height = thresholdImage._height, count = 0;
	unsigned char blue[] = { 0, 0, 255 };

	sortCircleWeight = circleWeight;
	sort(sortCircleWeight.begin(), sortCircleWeight.end(), greater<int>()); // 将累加矩阵从大到小进行排序

	while (1) {
		int weight = sortCircleWeight[count], index;
		vector<int>::iterator iter = find(circleWeight.begin(), circleWeight.end(), weight);
		index = iter - circleWeight.begin();
		int a = circles[index].first, b = circles[index].second;
		count++;

		int i;
		for (i = 0; i < center.size(); i++) {
			if (sqrt(pow((center[i].first - a), 2) + pow((center[i].second - b), 2)) < minRadius) {
				break; 
			}
		}
		if (i == center.size()) {
			center.push_back(make_pair(a, b));
			outputImage.draw_circle(a, b, r, blue, 5.0f, 1);
			break;
		}
	}		
}