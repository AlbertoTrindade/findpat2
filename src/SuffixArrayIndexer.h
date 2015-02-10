#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

#include <iostream>

#include <math.h>

using namespace std;

class SuffixArrayIndexer {

typedef std::tuple<int,int,int> triple;

private:
  static const int ALPHABET_LEN = 256;

  string text;
  int n;
  int m;
  int* suffixArray;
  int* LLcp;
  int* RLcp;

  int** buildP();
  void sortTriples(triple* triples, int numBuckets);
  static bool compareTriples(const triple& triple1, const triple& triple2);
  void computeLcps(int left, int right, int* suffixArray, int** p);
  int computeLcpP(int i, int j, int** p);

public:
  SuffixArrayIndexer(string& text);
  SuffixArrayIndexer(string& text, int* suffixArray, int* LLcp, int* RLcp);
  ~SuffixArrayIndexer();
  void buildSuffixArray();
  int findMatches(string& text);
  int getN();
  int getM();
  int* getSuffixArray();
  int* getLLcp();
  int* getRLcp();
};