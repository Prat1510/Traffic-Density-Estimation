#include <iostream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace boost::algorithm;
using namespace std;
float ErrorMeasure( string y)
{
	ifstream fin_x ("output_328_778.txt");
	ifstream fin_y ("output_" + y+".txt");

	vector<string> row_x{"","",""};
	vector<string> row_y{"","",""};
	string line_x, line_y, word, temp;
	float error_queue=0;
	// float error_dynamic=0;
	getline(fin_x, line_x);
	getline(fin_y, line_y);
	int count = 0;
	while (getline(fin_x, line_x)) {
		row_x.clear();
		row_y.clear();
		getline(fin_y, line_y);
		// used for breaking words
		boost::split(row_x, line_x, boost::is_any_of(","));  
		boost::split(row_y, line_y, boost::is_any_of(","));  
		// convert string to integer for comparision

		if (stof(row_y[1]) != 1){
			count++;
			error_queue = error_queue + abs(stof(row_x[1]) - stof(row_y[1]));
			// error_dynamic = error_dynamic + abs(stof(row_x[2]) - stof(row_y[2]));
		}
	}
	// arr[0] = error_queue/count;
	// arr[1] = error_dynamic/count;
	fin_x.close();
	fin_y.close();
	return error_queue/(float)count ; 
}
