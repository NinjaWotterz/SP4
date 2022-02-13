/**
 CCSVReader
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include <string>
#include <vector>
#include <utility> // std::pair

using namespace std;

class CCSVReader
{
public:
	// Constructor
	CCSVReader();
	// Destructor
	virtual ~CCSVReader();

	// Read from a CSV file which contains column names
	vector<pair<string, vector<int>>> read_csv_with_columnname(string filename);

	// Read from a CSV file
	vector<vector<int>> read_csv(string filename, const int NUM_TILES_XAXIS, const int NUM_TILES_YAXIS);
};