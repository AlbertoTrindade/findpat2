#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

#include <math.h>

using namespace std;

class SuffixArrayIndexer {

typedef tuple<int, int, int> triple;

private:
  static const int ALPHABET_LEN = 256;

  string text;
  int n;
  int m;
  int* suffixArray;
  int* LLcp;
  int* RLcp;
  vector<int> breakLinePositions;

  int** buildP();
  void sortTriples(triple* triples, int numBuckets);
  static bool compareTriples(const triple& triple1, const triple& triple2);
  void computeLcps(int left, int right, int* suffixArray, int** p);
  int computeLcpP(int i, int j, int** p);
  int searchLp(string& pattern);
  int computeLcp(string& text1, string& text2);

public:
  SuffixArrayIndexer(string& text);
  SuffixArrayIndexer(string& text, int* suffixArray, int* LLcp, int* RLcp);
  ~SuffixArrayIndexer();
  void buildSuffixArray();
  void findMatches(string& pattern, vector<int>& occurrences);
  int getN();
  int getM();
  int* getSuffixArray();
  int* getLLcp();
  int* getRLcp();
  vector<int> getBreakLinePositions();
};