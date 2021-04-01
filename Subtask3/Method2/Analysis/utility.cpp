#include <iostream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace boost::algorithm;
using namespace std;
float UtilityMeasure(string y)
{
	ifstream fin_x ("output_328_778.csv");
	ifstream fin_y ("Outfiles/output_" + y + ".txt");

	vector<string> row_x{"",""};
	vector<string> row_y{"",""};

	string line_x, line_y, word, temp;
	float error_queue = 0;
	float base_queue = 0;
	getline(fin_x, line_x);
	getline(fin_y, line_y);

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
	return (1.0-error_queue/base_queue) ; 
}

int main(int argc, char const *argv[])
{
	int n = stoi(argv[1]);
	float result;
	string line;
	vector<string> row_x{"",""};
	ofstream file ("Utility_vs_Runtime.txt");
	ifstream myfile ("runtimes.txt");
	string resolution, runtime;

	for (int i = 1; i <= n; i++)
	{
		getline(myfile, line);
		boost::split(row_x, line, boost::is_any_of(",")); 
		resolution = row_x[0];
		runtime = row_x[1];
		result = UtilityMeasure(resolution);
		file << resolution <<","<< result << "," <<runtime<<"\n";
	}
	file.close();
	myfile.close();
	return 0;
}


