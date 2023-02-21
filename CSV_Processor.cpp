/*
 * This program takes an input CSV file and outputs a modified version of the file
 * with lowercase column headers, removed spaces from headers, and replaced "true"
 * and "false" values with "1" and "0" respectively. Additionally, it converts
 * column values to lowercase and replaces spaces with underscores.
 *
 * Author: Kylar Sorensen
 * Date: 02/20/2023
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "config.h"

using namespace std;

// Function to remove whitespace from a string
// Input: a string with whitespace
// Output: the input string with whitespace removed
string removeWhitespace(string str)
{
    str.erase(remove_if(str.begin(), str.end(), [](char c)
                        { return isspace(c); }),
              str.end());

    size_t pos;
    while ((pos = str.find("_(")) != string::npos)
    {
        str.replace(pos, 1, "");
    }

    return str;
}

int main()
{
    // Open the input CSV file, using the file path defined in the config file
    ifstream inputFile(INPUT_FILE_PATH);
    if (!inputFile.is_open())
    {
        cout << "Error: could not open input file." << endl;
        return 1;
    }
    else
    {
        cout << "Input file opened successfully." << endl;
    }

    // Open the output CSV file, using the file path defined in the config file
    ofstream outputFile(OUTPUT_FILE_PATH);
    if (!outputFile.is_open())
    {
        cout << "Error: could not open output file." << endl;
        return 1;
    }
    else
    {
        cout << "Output file opened successfully." << endl;
    }

    // Read the header line
    string header;
    if (getline(inputFile, header))
    {
        cout << "Header line read successfully." << endl;
    }
    else
    {
        cout << "Error: could not read header line." << endl;
        return 1;
    }

    // Change header to lowercase and replace spaces with underscores
    transform(header.begin(), header.end(), header.begin(), [](unsigned char c)
              { return tolower(c); });
    replace(header.begin(), header.end(), ' ', '_');

    // Split the header into column names
    vector<string> columnNames;
    string column;
    size_t pos = 0, lastPos = 0;
    while ((pos = header.find(";", lastPos)) != string::npos)
    {
        column = header.substr(lastPos, pos - lastPos);
        columnNames.push_back(removeWhitespace(column));
        lastPos = pos + 1;
    }
    column = header.substr(lastPos);
    columnNames.push_back(removeWhitespace(column));

    // Write the cleaned header to the output CSV file
    for (size_t i = 0; i < columnNames.size(); i++)
    {
        outputFile << columnNames[i];
        if (i < columnNames.size() - 1)
        {
            outputFile << ",";
        }
    }
    outputFile << endl;

    // Process the remaining lines in the input CSV file
    string line;
    while (getline(inputFile, line))
    {
        // Split the line into columns
        vector<string> columns;
        size_t pos = 0, lastPos = 0;
        while ((pos = line.find(";", lastPos)) != string::npos)
        {
            string column = line.substr(lastPos, pos - lastPos);
            columns.push_back(column);
            lastPos = pos + 1;
        }
        column = line.substr(lastPos);
        columns.push_back(column);

        // Modified lines as necessary and write them to the output CSV file
        for (size_t i = 0; i < columns.size(); i++)
        {
            if (i == 9)
            {
                // Don't change the city column
                outputFile << columns[i];
            }
            else if (columns[i].empty() || columns[i] == "No weather")
            {
                outputFile << "NULL";
            }
            else if (columns[i] == "true")
            {
                outputFile << "1";
            }
            else if (columns[i] == "false")
            {
                outputFile << "0";
            }
            else
            {
                // Convert the column to lowercase and replace spaces with underscores.
                // Then, write the modified column to the output CSV file.
                string column = columns[i];
                transform(column.begin(), column.end(), column.begin(), [](unsigned char c)
                          { return tolower(c); });
                replace(column.begin(), column.end(), ' ', '_');
                outputFile << column;
            }
            if (i < columns.size() - 1)
            {
                outputFile << ",";
            }
        }
        outputFile << endl;
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();

    return 0;
}