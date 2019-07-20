#include"EdgeDetect.hpp"
#define PI 3.141592653
#define gFilterX 5
#define gFilterY 5
#define sigma 1
#define thresholdLow 120
#define thresholdHigh 140
#define thetaSize 360

EdgeDetect::EdgeDetect(string input, string output, int number, int wind){
    for (int i = 0; i < thetaSize; i++) {
		setSin.push_back(sin(2 * PI * i / thetaSize));
		setCos.push_back(cos(2 * PI * i / thetaSize));
	}
    thresholdImage.load(input.c_str());

	windowSize = wind;
    lineNumber = number;
    houghLinesTransform(thresholdImage); 
    houghLinesDetect(); 


    drawEdge(); 
    drawPoint();
    outputImage.save(output.c_str());

}

void EdgeDetect::houghLinesTransform(CImg<float>& img) {
	int width = img._width, height = img._height, maxLength, row, col;
	maxLength = sqrt(pow(width / 2, 2) + pow(height / 2, 2)); 
	row = thetaSize;
	col = maxLength;

	houghImage = CImg<float>(col, row);
	houghImage.fill(0);

	cimg_forXY(img, x, y) {
		int value = img(x, y), p = 0;
		if (value != 0) {
			int x0 = x - width / 2, y0 = height / 2 - y;
			for (int i = 0; i < thetaSize; i++) {
				//投票
				p = x0 * setCos[i] + y0 * setSin[i];
				if (p >= 0 && p < maxLength) {
					houghImage(p, i)++;
				}
			}
		}
	}
	houghImage.display("houghspace");
}

/*霍夫直线检测*/
void EdgeDetect::houghLinesDetect() {
	int width = houghImage._width, height = houghImage._height, size = windowSize, max;
	//以窗口为单位检测边缘点
	for (int i = 0; i < height; i += size / 2) {
		for (int j = 0; j < width; j += size / 2) {
			max = getMaxHough(houghImage, size, i, j);//找到该窗口中最大的点，即为边缘点。
			for (int y = i; y < i + size; ++y) {
				for (int x = j; x < j + size; ++x) {
					if (houghImage.(x, y) < max) {
						houghImage.(x, y) = 0; // 把不是边缘点的点去掉
					}	
				}
			}
		}
	}
	/*将霍夫图像中所有不为0的点对应直线的斜率和截距存入数组*/
	cimg_forXY(houghImage, x, y) {
		if (houghImage(x, y) != 0) {
			lines.push_back(make_pair(y, x));//保存斜率和截距
			lineWeight.push_back(houghImage(x, y));//检测到的每个窗口中的最大的点，用于之后排序筛选边缘直线
		}
	}
}

/*计算霍夫空间直线交点*/
int EdgeDetect::getMaxHough(CImg<float>& img, int& size, int& y, int& x) {
	int width = (x + size > img._width) ? img._width : x + size;
	int height = (y + size > img._height) ? img._height : y + size;
	int max = 0;
	for (int j = x; j < width; j++) {
		for (int i = y; i < height; i++) {
			max = (img(j, i) > max) ? img(j, i) : max;
		}
	}
	return max;
}

/*描绘所检测出的边缘*/
void EdgeDetect::drawEdge() {
	outputImage = CImg<float>(thresholdImage._width, thresholdImage._height, 1, 3, 0);
	cimg_forXY(thresholdImage, x, y){
		if(thresholdImage(x, y) == 255){
			outputImage(x, y, 0) = 255;
			outputImage(x, y, 1) = 255;
			outputImage(x, y, 2) = 255;
		}
	}
	cout<<"change success"<<endl;

	int width = thresholdImage._width, height = thresholdImage._height;

	findPoint = CImg<float>(width, height, 1, 1, 0);
	sortLineWeight = lineWeight;
	sort(sortLineWeight.begin(), sortLineWeight.end(), greater<int>()); // 将累加矩阵从大到小进行排序

	vector<pair<int, int>> result; // 存放累加值最大的边缘直线对应斜率和截距
	for (int i = 0; i < lineNumber; i++) {
		int weight = sortLineWeight[i], index;
		// if(weight == 443 || weight == 351){
		// 	continue;
		// }
		vector<int>::iterator iter = find(lineWeight.begin(), lineWeight.end(), weight);
		index = iter - lineWeight.begin();
		result.push_back(lines[index]);
	}
	for (int i = 0; i < result.size(); i++) {
		int theta = result[i].first, p = result[i].second;
		vector<int> xn;
		vector<int> yn;
		unsigned char blue[] = {0, 0, 255};
		/*根据theta和p求出斜率和截距*/
		cimg_forXY(thresholdImage, x, y) {
			int x0 = x - width / 2, y0 = height / 2 - y;
			if (p == (int)(x0 * setCos[theta] + y0 * setSin[theta])) {
				findPoint(x, y) += 255.0 / 2;
				xn.push_back(x);
				yn.push_back(y);
			}
		}
		outputImage.draw_line(xn[0], yn[0], xn[xn.size()-1], yn[yn.size()-1], blue);
		cout<<p<<"=x0*"<<setCos[theta]<<"+y0*"<<setSin[theta]<<endl;
	}
	outputImage.display("I2");
	cimg_forXY(findPoint, x, y){
		if(outputImage(x, y, 0) == 0 && outputImage(x, y, 1) == 0 && outputImage(x, y, 2) == 255 && thresholdImage(x, y) == 255){
			outputImage(x, y, 0) = 255;
			outputImage(x, y, 1) = 0;
			outputImage(x, y, 2) = 0;
		}
	}
	outputImage.display("I3");
}

/*描绘所检测出的角点*/
void EdgeDetect::drawPoint() {
	unsigned char red[] = { 255, 0, 0 };
	for (int y = 0; y < outputImage._height - 1; y++) {
		for (int x = 0; x < outputImage._width - 1; x++) {
			if (findPoint(x, y) >= 255.0) {
				outputImage.draw_circle(x, y, 5, red);
			}
		}
	}
	outputImage.display("I4");
}