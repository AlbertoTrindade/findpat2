#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>

#include "SuffixArrayIndexer.h"
#include "LZ78Compressor.h"

using namespace std;

class SearchProcessor {

private:
  static void setArrayFromIndexLine (string& indexLine, int* array);
  static void setVectorFromIndexLine (string& indexLine, vector<int>& vector);
  static void printMatchedLines(vector<int>& occurrences, vector<int>& breakLinePositions, string& text);

public:
  static void processParameters (string& patternFileName,
                                 bool count,
                                 string& pattern,
                                 string& indexFileName);

};