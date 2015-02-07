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
  int* suffixArray = indexer->buildSuffixArray();
  int n = indexer->getN();

  // Create index file
  // TODO: Compress index text (write content into a string stream, instead of directly into the file, to be used in compression)
  string indexFileName = getIndexFileName(textFileName);
  ofstream indexFile(indexFileName);

  // Write n (text length) in first line
  indexFile << n << endl;

  // Write suffix array in second line
  for (int i = 0; i < n; i++) {
    indexFile << suffixArray[i] << " ";
  }
  indexFile << endl;

  // Write text in the remaining lines
  indexFile << text;

  // Sucess message
  cout << "Index file '" << indexFileName << "' has been successfully created" << endl;

  // Releasing resources
  textFile.close();
  indexFile.close();
  delete [] suffixArray;
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