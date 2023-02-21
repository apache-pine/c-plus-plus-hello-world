#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config.h"

using namespace std;

int main()
{
    // Open the input CSV file
    ifstream inputFile(INPUT_TEST_FILE_PATH);
    if (!inputFile.is_open())
    {
        cout << "Error: could not open input file." << endl;
        return 1;
    }

    // Open the output CSV file
    ofstream outputFile(OUTPUT_TEST_FILE_PATH);
    if (!outputFile.is_open())
    {
        cout << "Error: could not open output file." << endl;
        return 1;
    }

    // Read the input CSV file line by line
    string line;
    while (getline(inputFile, line))
    {

        // Split the line into columns
        vector<string> columns;
        size_t pos = 0;
        while ((pos = line.find(",")) != string::npos)
        {
            string column = line.substr(0, pos);
            columns.push_back(column);
            line.erase(0, pos + 1);
        }
        columns.push_back(line);

        // Convert the value in the second column
        if (columns.size() >= 2)
        {
            if (columns[1] == "Yes")
            {
                columns[1] = "1";
            }
            else if (columns[1] == "No")
            {
                columns[1] = "0";
            }
        }

        // Write the modified line to the output CSV file
        for (size_t i = 0; i < columns.size(); i++)
        {
            outputFile << columns[i];
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

    cout << "Done." << endl;

    return 0;
}