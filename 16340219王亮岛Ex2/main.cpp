#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include "cannycimg.h"
using namespace std;

int main(int argc, char *argv[])
{
	if (argc >= 2){
		string filePath = argv[1]; 
		cannyImg cny(filePath);
	}else{
		cout << "Please add argument when running" << endl;
	}
        
    return 0;
}