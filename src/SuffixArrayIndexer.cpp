#include "SuffixArrayIndexer.h"

SuffixArrayIndexer::SuffixArrayIndexer(string& text) {
  this->text = text;
  this->n = text.size();
  this->m = ceil(log2(n));
  this->suffixArray = new int[n];
  this->LLcp = new int[n]();
  this->RLcp = new int[n]();
}

SuffixArrayIndexer::SuffixArrayIndexer(string& text, int* suffixArray, int* LLcp, int* RLcp) {
  this->text = text;
  this->n = text.size();
  this->m = ceil(log2(n));
  this->suffixArray = suffixArray;
  this->LLcp = LLcp;
  this->RLcp = RLcp;
}

SuffixArrayIndexer::~SuffixArrayIndexer() {
  text.clear();
  delete [] suffixArray;
  delete [] LLcp;
  delete [] RLcp;
  breakLinePositions.clear();
}

void SuffixArrayIndexer::buildSuffixArray() {
  int** p = buildP();

  for (int i = 0; i < n; i++) {
    suffixArray[p[m][i]-1] = i;
  }

  // compute LLcp and RLcp
  computeLcps(0, n-1, suffixArray, p);

  // adding position after last one as a break line
  breakLinePositions.push_back(n);

  // delete p
  for (int i = 0; i < m; i++) {
    delete [] p[i];
  }
}

int** SuffixArrayIndexer::buildP() {
  int** p = new int*[m+1];
  
  // Initializing p
  for (int i = 0; i <= m; i++) {
    p[i] = new int[n];
  }

  // Initialize p[0] with letter (ascii) values
  for (int i = 0; i < n; i++) {
    p[0][i] = text.at(i);
    
    //save break line position into a vector
    if (text.at(i) == '\n') {
      breakLinePositions.push_back(i);
    }
  }

  int lastHighestRank;

  for (int k = 1; k <= m; k++) {
    int j = pow(2, k-1);
    triple* triples = new triple[n];

    for (int i = 0; i < n; i++) {
      if (i + j + 1 > n) {
        triples[i] = triple(p[k-1][i], 0, i);
      }
      else {
        triples[i] = triple(p[k-1][i], p[k-1][i + j], i);
      }
    }

    if (k == 1) {
      sortTriples(triples, 255); // 255 is the highest ascii value, which is the highest possible rank value at first iteration
    }
    else {
      sortTriples(triples, lastHighestRank);
    }

    int rank = 1;
    p[k][get<2>(triples[0])] = rank;

    for (int i = 1; i < n; i++) {
      if ((get<0>(triples[i]) != get<0>(triples[i-1])) || (get<1>(triples[i]) != get<1>(triples[i-1]))) {
        rank++;
      }

      p[k][get<2>(triples[i])] = rank;
    }

    lastHighestRank = rank;

    delete [] triples;
  }

  return p;
}

void SuffixArrayIndexer::sortTriples(triple* triples, int numBuckets) {
  vector<triple>* buckets = new vector<triple>[numBuckets+1];

  for (int i = 0; i < n; i++) {
    int firstValue = get<0>(triples[i]);
    buckets[firstValue].push_back(triples[i]);
  }

  // sorting the buckets
  for (int i = 0; i <= numBuckets; i++) {
    if (buckets[i].size() > 0) {
      sort(buckets[i].begin(), buckets[i].end(), compareTriples);
    }
  }

  // copying the sorted triples back to array
  int index = 0;

  for (int i = 0; i <= numBuckets; i++) {
    int currentBucketSize = buckets[i].size();

    for (int j = 0; j < currentBucketSize; j++) {
      triples[index] = buckets[i].at(j);
      index++;
    }
  }

  delete [] buckets;
}

bool SuffixArrayIndexer::compareTriples(const triple& triple1, const triple& triple2) {
  return (get<1>(triple1) < get<1>(triple2));
}

void SuffixArrayIndexer::computeLcps(int left, int right, int* suffixArray, int** p) {
  if (right - left <= 1) {
    return;
  }
  else {
    int middle = (left + right)/2;

    LLcp[middle] = computeLcpP(suffixArray[left], suffixArray[middle], p);
    RLcp[middle] = computeLcpP(suffixArray[middle], suffixArray[right], p);

    computeLcps(left, middle, suffixArray, p);
    computeLcps(middle, right, suffixArray, p);
  }
}

int SuffixArrayIndexer::computeLcpP(int i, int j, int** p) {
  if (i == j) {
    return n - 1 + i;
  }
  else {
    int k = m;
    int lcp = 0;

    while (k >= 0 && i <= n && j <=n) {
      if (p[k][i] == p[k][j]) {
        lcp = lcp + pow(2, k);
        i = i + pow(2, k);
        j = j + pow(2, k);
      }

      k--;
    }

    return lcp;
  }
}

int SuffixArrayIndexer::getN() {
  return n;
}

int SuffixArrayIndexer::getM() {
  return m;
}

int*  SuffixArrayIndexer::getSuffixArray() {
  return suffixArray;
}

int*  SuffixArrayIndexer::getLLcp() {
  return LLcp;
}

int*  SuffixArrayIndexer::getRLcp() {
  return RLcp;
}

vector<int> SuffixArrayIndexer::getBreakLinePositions() {
  return breakLinePositions;
}