#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

#include <math.h>

using namespace std;

class SuffixArrayIndexer {

typedef std::tuple<int,int,int> triple;

private:
  static const int ALPHABET_LEN = 256;

  string text;
  int n;
  int m;

  int** buildP();
  void sortTriples(triple* triples, int numBuckets);
  static bool compareTriples(const triple& triple1, const triple& triple2);

public:
  SuffixArrayIndexer(string& text);
  ~SuffixArrayIndexer();
  int* buildSuffixArray();
  int findMatches(string& text);
  int getN();
  int getM();
};