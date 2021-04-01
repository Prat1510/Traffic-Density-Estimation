#include <iostream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace boost::algorithm;
using namespace std;
float UtilityMeasure(int y)
{
	ifstream fin_x ("dense.txt");
	ifstream fin_y ("sparse.txt");

	vector<string> row_x{"",""};
	vector<string> row_y{"",""};

	string line_x, line_y, word, temp;
	float error_queue = 0;
	float base_queue = 0;

	int count = 0;

	while (getline(fin_x, line_x)) {
		row_x.clear();
		row_y.clear();
		getline(fin_y, line_y);

		boost::split(row_x, line_x, boost::is_any_of(","));  
		boost::split(row_y, line_y, boost::is_any_of(","));  

		if (stof(row_y[1]) != 1){
			count++;
			error_queue = error_queue + abs(stof(row_x[1]) - stof(row_y[1]));
			base_queue = base_queue + stof(row_x[1]);
		}
	}
	fin_x.close();
	fin_y.close();
	return 1.0 - error_queue/base_queue ; 
}

int main(int argc, char const *argv[])
{
	float result;
	ofstream file ("utility.txt");
	result = UtilityMeasure(1);
	file << result<<"\n";
	file.close();
	return 0;
}

