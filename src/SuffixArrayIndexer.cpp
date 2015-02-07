#include "SuffixArrayIndexer.h"

SuffixArrayIndexer::SuffixArrayIndexer(string& text) {
  this->text = text;
  this->n = text.size();
  this->m = ceil(log2(n));
}

SuffixArrayIndexer::~SuffixArrayIndexer() {
  text.clear();
}

int* SuffixArrayIndexer::buildSuffixArray() {
  int** p = buildP();
  int* suffixArray = new int[n];

  for (int i = 0; i < n; i++) {
    suffixArray[p[m][i]-1] = i;
  }

  // delete p
  for (int i = 0; i < m; i++) {
    delete [] p[i];
  }

  return suffixArray;
}

int** SuffixArrayIndexer::buildP() {
  int** p = new int*[m+1];
  
  // Initializing p
  for (int i = 0; i <= m; i++) {
    p[i] = new int[n];
  }

  // Initialize p[0] with letter values
  for (int i = 0; i < n; i++) {
    p[0][i] = text[i];
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

int SuffixArrayIndexer::getN() {
  return n;
}

int SuffixArrayIndexer::getM() {
  return m;
}