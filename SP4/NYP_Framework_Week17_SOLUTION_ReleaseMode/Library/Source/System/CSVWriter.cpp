/**
 CCSVWriter
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "CSVWriter.h"

#include <fstream>
#include <stdexcept> // runtime_error
#include <sstream> // stringstream
#include "filesystem.h"

/**
 @brief Constructor
 */
CCSVWriter::CCSVWriter()
{
}

/**
 @brief Destructor
 */
CCSVWriter::~CCSVWriter()
{
}

/**
@brief Write to a CSV file which contains column names
@param filename A string containing the name of the input file
@param vector<pair<string, vector<int>>> A vector containing pairs of string and vector which represents the tile map
@return A vector<vector<int>> variable
*/
bool CCSVWriter::write_csv_with_columnname(string filename, vector<pair<string, vector<int>>> vData)
{
	// The result of this CSV reading attempt
	bool bResult = false;

	// Create an input filestream
	ofstream myFile(filename, ios::out | ios::trunc);

	// Make sure the file is open
	if (!myFile.is_open()) throw runtime_error("Could not create file");

	// Write all data including the column names
	if (myFile.good())
	{
		vector<pair<string, vector<int>>>::iterator iterRow;
		for (iterRow = vData.begin(); iterRow != vData.end(); iterRow++)
		{
			vector<int>::iterator iterColumn;
			for (iterColumn = iterRow->second.begin(); iterColumn != iterRow->second.end(); iterColumn++)
			{
				myFile << *iterColumn;
				if (iterColumn != iterRow->second.end())
					myFile << ",";
			}
			myFile << endl;
		}
		// Set the result to true
		bResult = true;
	}

	// Close file
	myFile.close();

	return bResult;
}

/**
 @brief Write to a CSV file
 @param filename A string containing the name of the input file
 @param NUM_TILES_XAXIS A const int containing the number of tiles in the x-axis
 @param NUM_TILES_YAXIS A const int containing the number of tiles in the y-axis
 @param vector<pair<string, vector<int>>> A vector containing pairs of string and vector which represents the tile map
 @return A vector<vector<int>> variable
 */
bool CCSVWriter::write_csv(	string filename, 
							const int NUM_TILES_XAXIS, const int NUM_TILES_YAXIS, 
							vector<vector<int>> vData)
{
	// The result of this CSV reading attempt
	bool bResult = false;

	// Create an input filestream
	ofstream myFile(filename, ios::out | ios::trunc);

	// Make sure the file is open
	try {
		if (!myFile.is_open())
			throw runtime_error("Could not open file");
	}
	catch (runtime_error e)
	{
		//cout << "Runtime error: " << e.what();
		return false;
	}

	// Write all data including the column names
	if (myFile.good())
	{
		// Check if the number of rows in vData matches the declared value in NUM_TILES_YAXIS
		if (vData.size() == NUM_TILES_YAXIS)
		{
			vector<vector<int>>::iterator iterRow = vData.begin();

			// Check if the number of columns in vData matches the declared value in NUM_TILES_XAXIS
			if ((*iterRow).size() == NUM_TILES_XAXIS)
			{
				for (iterRow = vData.begin(); iterRow != vData.end(); iterRow++)
				{
					vector<int>::iterator iterColumn;
					for (iterColumn = (*iterRow).begin(); iterColumn != (*iterRow).end(); iterColumn++)
					{
						myFile << *iterColumn;
						if (iterColumn != (*iterRow).end())
							myFile << ",";
					}
					myFile << endl;
				}
				// Set the result to true
				bResult = true;
			}
		}
	}

	// Close file
	myFile.close();

	return bResult;
}
