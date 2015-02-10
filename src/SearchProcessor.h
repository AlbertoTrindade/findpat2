#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include "SuffixArrayIndexer.h"

using namespace std;

class SearchProcessor {

private:
  static void setArrayFromIndexLine (string indexLine, int* array);

public:
  static void processParameters (string& patternFileName,
                                 bool count,
                                 string& pattern,
                                 string& indexFileName);

};