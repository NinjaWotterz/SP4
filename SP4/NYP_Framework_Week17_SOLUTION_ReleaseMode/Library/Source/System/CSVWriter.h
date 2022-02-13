/**
 CCSVWriter
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

#include <string>
#include <vector>
#include <utility> // std::pair

using namespace std;

class CCSVWriter
{
public:
	// Constructor
	CCSVWriter();
	// Destructor
	virtual ~CCSVWriter();

	// Write to a CSV file which contains column names
	bool write_csv_with_columnname(string filename, vector<pair<string, vector<int>>> vData);
	// Write to a CSV file
	bool write_csv(	string filename, 
					const int NUM_TILES_XAXIS, const int NUM_TILES_YAXIS, 
					vector<vector<int>> vData);
};