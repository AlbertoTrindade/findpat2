#include "IndexProcessor.h"

void IndexProcessor::processParameters (string& textFileName) {

  ifstream textFile(textFileName);

  if (textFile.fail()) {
    cout << textFileName << ": No such file or directory" << endl;
    return;
  }

  // Reading text from buffer
  stringstream textFileBuffer;
  textFileBuffer << textFile.rdbuf();

  // Text to be indexed
  string text = textFileBuffer.str();

  //Suffix array indexer
  SuffixArrayIndexer* indexer = new SuffixArrayIndexer(text);
  indexer->buildSuffixArray();

  int n = indexer->getN();
  int* suffixArray = indexer->getSuffixArray();
  int* LLcp = indexer->getLLcp();
  int* RLcp = indexer->getRLcp();
  vector<int> breakLinePositions = indexer->getBreakLinePositions();

  // Create index file
  
  stringstream uncompressedTextStream;

  // Write n (text length) in first line
  uncompressedTextStream << n << endl;

  // Write suffix array in second line
  for (int i = 0; i < n; i++) {
    uncompressedTextStream << suffixArray[i] << " ";
  }
  uncompressedTextStream << endl;

  // Write LLcp in third line
  for (int i = 0; i < n; i++) {
    uncompressedTextStream << LLcp[i] << " ";
  }
  uncompressedTextStream << endl;

  // Write RLcp in fourth line
  for (int i = 0; i < n; i++) {
    uncompressedTextStream << RLcp[i] << " ";
  }
  uncompressedTextStream << endl;

  // Write break line positions in fifth line
  for (int& breakLinePosition : breakLinePositions) {
    uncompressedTextStream << breakLinePosition << " ";
  }
  uncompressedTextStream << endl;

  // Write text in the remaining lines
  uncompressedTextStream << text;

  string uncompressedText = uncompressedTextStream.str();
  string compressedText = LZ78Compressor::encode(uncompressedText);

  string indexFileName = getIndexFileName(textFileName);
  ofstream indexFile(indexFileName);

  // Write compressed text in file
  indexFile << compressedText;
  

  // Sucess message
  cout << "Index file '" << indexFileName << "' has been successfully created" << endl;

  // Releasing resources
  textFile.close();
  indexFile.close();
  delete indexer;
}

string IndexProcessor::getIndexFileName(string& textFileName) {
  string indexFileName = "";
  int textFileNameSize = textFileName.size();
  char currentChar;

  for (int i = 0; i < textFileNameSize; i++) {
    currentChar = textFileName.at(i);

    if (currentChar == '.') {
      break;
    }
    else {
      indexFileName += currentChar;
    }
  }

  indexFileName += ".idx";

  return indexFileName;
}