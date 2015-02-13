#pragma once

#include <string>
#include <iterator>
#include <tuple>
#include <unordered_map>

#include <iostream>
#include <sstream>

using namespace std;

struct TreeNode {
  int id;
  unordered_map<char, TreeNode*> children;

  TreeNode(int index):id(index) {}
};

class LZ78Compressor {

typedef tuple<int, char, int> decodeResult;
  
private:
  static const char LAST_MISMATCH = 254;
  static const char SEPARATOR = 245;

  static string cwEncode(int id, char letter);
  static decodeResult cwDecode(string code, int j);
  static void deleteTreeNode(TreeNode* node);

public:
  static string encode(string& text);
  static string decode(string& code);
};