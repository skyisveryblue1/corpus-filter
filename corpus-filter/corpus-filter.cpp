// corpus-filter.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <stdlib.h>

using namespace std;

// Function to tokenize a string into words
vector<string> tokenize(const string& input) {
    istringstream ss(input);
    istream_iterator<string> begin(ss), end;
    return vector<string>(begin, end);
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << " <input_pl_file> <input_en_file> <threshold>" << endl;
		return 1;
	}

    string inputPlFileName = argv[1];
    string inputEnFileName = argv[2];
    int threshold = atoi(argv[3]);

    // Read the input files
    ifstream inputPlFile(inputPlFileName);
    ifstream inputEnFile(inputEnFileName);

    if (!inputPlFile.is_open() || !inputEnFile.is_open()) {
        cerr << "Error opening input files." << endl;
        return 1;
    }

    unordered_map<string, int> wordFrequency;

    // Create a dictionary of word frequencies from the Polish file
    string linePl;
    int lineCount = 0;
    int dotCount = 0;
    cout << "Counting frequencies of words ";
    while (getline(inputPlFile, linePl)) {
        lineCount++;
        if (lineCount == 1) {
            continue;
        }
        if (lineCount % 1000 == 1) {
            if (dotCount == 10) {
                cout << "\b\b\b\b\b\b\b\b\b";
                cout << "         ";
                cout << "\b\b\b\b\b\b\b\b\b";
                dotCount = 0;
            }
            else {
                cout << ".";
            }
            dotCount++;
        }
        vector<string> words = tokenize(linePl);
        for (const string& word : words) {
            wordFrequency[word]++;
        }
    }

    cout << endl << "\t Total word count:" << wordFrequency.size() << endl;

    // Sort the words by frequency
    cout << "Sorting words according their frequencies..." << endl;
    vector<pair<string, int>> sortedWords(
        wordFrequency.begin(),
        wordFrequency.end()
    );
    sort(sortedWords.begin(), sortedWords.end(),
        [] (const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        }
    );

    // Numbering words in order of decreasing frequency
    int totalWords = 0;
    int wordNo = 0;
    unordered_map<string, int> wordNumber;
    for (const auto& pair : sortedWords) {
        totalWords += pair.second;
        wordNumber[pair.first] = wordNo++;
    }
    int thresholdNo = (threshold * wordNo) / 100;

    // Create output files for filtered content
    string outputPlFileName = inputPlFileName + ".filtered";
    string outputEnFileName = inputEnFileName + ".filtered";

    ofstream outputPlFile(outputPlFileName);
    ofstream outputEnFile(outputEnFileName);

    // Read and filter the input files
    inputPlFile.clear();
    inputPlFile.seekg(0);

    cout << "Filtering sentences according their words' frequencies";
    string lineEn;
    lineCount = 0;
    dotCount = 0;
    while (getline(inputPlFile, linePl) && getline(inputEnFile, lineEn)) {
        lineCount++;
        if (lineCount == 1) {
            outputPlFile << linePl << "\n";
            outputEnFile << lineEn << "\n";
            continue;
        }

        if (lineCount % 1000 == 1) {
            if (dotCount == 10) {
                cout << "\b\b\b\b\b\b\b\b\b";
                cout << "         ";
                cout << "\b\b\b\b\b\b\b\b\b";
                dotCount = 0;
            }
            else {
                cout << ".";
            }
            dotCount++;
        }

        vector<string> words = tokenize(linePl);
        bool skipLine = false;
        for (const string& word : words) {
            int curNo = wordNumber[word];
            if (curNo > thresholdNo) {
                skipLine = true;
                break;
            } 
        }

        if (!skipLine) {
            outputPlFile << linePl << "\n";
            outputEnFile << lineEn << "\n";
        }
    }

    // Close the files
    inputPlFile.close();
    inputEnFile.close();
    outputPlFile.close();
    outputEnFile.close();

    cout << endl << "Filtered files saved as '" << outputPlFileName << "' and '" << outputEnFileName << "'" << endl;

	return 0;
}


