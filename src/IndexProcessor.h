#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include "SuffixArrayIndexer.h"
#include "LZ78Compressor.h"

using namespace std;

class IndexProcessor {

private:
  static string getIndexFileName(string& textFileName);

public:
  static void processParameters (string& textFileName);

};