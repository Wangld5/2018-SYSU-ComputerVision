#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "cannycimg.h"
using namespace std;

int main(int argc, char *argv[])
{
	string fileName;
	fileName = argv[1];
	int sigma, low, high;
	cin>>sigma>>low>>high;
	cannyImg cny(fileName, sigma, low, high);
        
    return 0;
}