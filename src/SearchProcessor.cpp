#include "SearchProcessor.h"

void SearchProcessor::processParameters (string& patternFileName,
                                         bool count,
                                         string& pattern,
                                         string& indexFileName) {

  // Getting the list of patterns either from patternfile or pattern parameters
  vector<string> patterns;

  if (!pattern.empty()) {
    patterns.push_back(pattern);
  }
  else {
    ifstream patternFile(patternFileName);
    string patternInFile;

    while (getline(patternFile, patternInFile)) {
      patterns.push_back(patternInFile);
    }
  }

  // Reading index file
  ifstream indexFile(indexFileName);

  if (indexFile.fail()) {
    cout << indexFileName << ": No such file or directory" << endl;
    return;
  }

  // Reading index text from buffer
  stringstream indexFileBuffer;
  indexFileBuffer << indexFile.rdbuf();
  string indexText = indexFileBuffer.str();

  // TODO: Decompress indexText

  // Read first (n), second (suffixArray) and remaining lines (text) of decompressed index text
  istringstream indexTextStream(indexText);
  string indexTextLine;

  int n; // size of text and suffix array
  int* suffixArray;
  string text;

  bool firstLine = true;

  while (getline(indexTextStream, indexTextLine)) {
    if (firstLine) { 
      // n: first line
      n = stoi(indexTextLine);
      suffixArray = new int[n];

      firstLine = false;
    }
    else {
      // suffix array: second line
      istringstream suffixArrayStream(indexTextLine);
      string suffixArrayElementString;

      int i = 0;

      while (getline(suffixArrayStream, suffixArrayElementString, ' ')) {
        int suffixArrayElement = stoi(suffixArrayElementString);
        suffixArray[i] = suffixArrayElement;

        i++;
      }

      // text: remaining lines
      text = indexTextStream.str().substr(indexTextStream.tellg());
    
      break;
    }
  }

  // TODO: use suffix array, text and patterns to find occurrences

  // Releasing resources
  delete [] suffixArray;
}