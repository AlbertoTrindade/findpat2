#pragma once

#include <string>
#include <iterator>
#include <tuple>
#include <vector>
#include <unordered_map>

#include <sstream>
#include <fstream>

using namespace std;

struct DictionaryTreeNode {
  int id;
  unordered_map<char, DictionaryTreeNode*> children;

  DictionaryTreeNode(int index):id(index) {}
};

struct CodeWord {
  unsigned short id;
  char mismatch;
};

class LZ78Compressor {
  
private:
  static const char LAST_MISMATCH = 254;
  static const unsigned short DICTIONARY_LIMIT = 65535; // max value for unsigned shor

  static void cwEncode(unsigned short id, char mismatch, vector<CodeWord>& codeWords);
  static void deleteDictionaryTreeNode(DictionaryTreeNode* node);

public:
  static void encode(string& text, ofstream& indexFile);
  static string decode(ifstream& indexFile, int indexFileSize);
};