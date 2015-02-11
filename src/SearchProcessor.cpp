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
  int* LLcp;
  int* RLcp;
  vector<int> breakLinePositions;
  string text;

  bool firstLine = true;
  bool secondLine = true;
  bool thirdLine = true;
  bool fourthLine = true;

  while (getline(indexTextStream, indexTextLine)) {
    if (firstLine) { 
      // n: first line
      n = stoi(indexTextLine);
      suffixArray = new int[n];
      LLcp = new int[n];
      RLcp = new int[n];

      firstLine = false;
    }
    else if (secondLine){
      // suffix array: second line
      setArrayFromIndexLine(indexTextLine, suffixArray);
      secondLine = false;
    }
    else if (thirdLine) {
      // LLcp array: third line
      setArrayFromIndexLine(indexTextLine, LLcp);
      thirdLine = false;
    }
    else if (fourthLine){
      // RLcp array: fourth line
      setArrayFromIndexLine(indexTextLine, RLcp);
      fourthLine = false;
    }
    else {
      // breakLinePositions: fifth line
      setVectorFromIndexLine(indexTextLine, breakLinePositions);

      // text: remaining lines
      text = indexTextStream.str().substr(indexTextStream.tellg());
    
      break;
    }
  }

  SuffixArrayIndexer* indexer = new SuffixArrayIndexer(text, suffixArray, LLcp, RLcp);

  // TODO: use indexer to find occurrences and print lines or count from result

  // Releasing resources
  delete indexer;
}

void SearchProcessor::setArrayFromIndexLine (string indexLine, int* array) {
  istringstream stringStream(indexLine);
  string arrayElementString;

  int i = 0;

  while (getline(stringStream, arrayElementString, ' ')) {
   int arrayElement = stoi(arrayElementString);
   array[i] = arrayElement;

   i++;
  }
}

void SearchProcessor::setVectorFromIndexLine (string indexLine, vector<int>& vector) {
  istringstream stringStream(indexLine);
  string vectorElementString;

  while (getline(stringStream, vectorElementString, ' ')) {
   int vectorElement = stoi(vectorElementString);
   vector.push_back(vectorElement);
  }
}