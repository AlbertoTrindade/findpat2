#include "LZ78Compressor.h"

string LZ78Compressor::encode(string& text) {
  int n = text.size();

  TreeNode* root = new TreeNode(0);
  int nextId = 1;

  stringstream codeStream;
  TreeNode* currentNode = root;

  for (int i = 0; i < n; i++) {
    char currentLetter = text.at(i);

    if (currentNode->children.count(currentLetter) > 0) { // wti belongs to dictionary
      currentNode = currentNode->children[currentLetter];
    }
    else {
      codeStream << cwEncode(currentNode->id, currentLetter); 

      TreeNode* newNode = new TreeNode(nextId);
      currentNode->children[currentLetter] = newNode;

      nextId++;

      currentNode = root;
    }
  }

  if (currentNode != root) { // w belongs to dictionary
    codeStream << cwEncode(currentNode->id, LAST_MISMATCH); 
  }

  deleteTreeNode(root);

  return codeStream.str();
}

string LZ78Compressor::cwEncode(int id, char letter) {
  stringstream encodedCwStream;
  encodedCwStream << id <<  "," <<  letter << SEPARATOR;

  return encodedCwStream.str();
}

void LZ78Compressor::deleteTreeNode(TreeNode* node) {
  if (node != NULL) {
    for (unordered_map<char, TreeNode*>::iterator it = node->children.begin(); it != node->children.end(); it++) {
      deleteTreeNode(node->children[it->first]);
    }

    delete node;
  }
}

string LZ78Compressor::decode(string& code) {
  int m = code.size();

  unordered_map<int, string> dictionary;

  dictionary[0] = "";
  int nextId = 1;

  stringstream textStream;

  int j = 0;

  while (j < m) {
    decodeResult result = cwDecode(code, j);

    int id = get<0>(result);
    char mismatch = get<1>(result);
    int length = get<2>(result);

    j = j + length;
    textStream << dictionary[id] << mismatch;
    dictionary[nextId] = dictionary[id] + mismatch;

    nextId++;
  }

  dictionary.clear();

  return textStream.str();
}

LZ78Compressor::decodeResult LZ78Compressor::cwDecode(string code, int j) {
  int id;
  char mismatch;
  int length;

  string idString;
  bool hasFinished;

  int i = j;

  while (!hasFinished) {
    if (code.at(i) != ',') {
      idString += code.at(i);

      i++;
    }
    else {
      id = stoi(idString);
      mismatch = code.at(i+1);
      length = i - j + 3;

      hasFinished = true;
    }
  }

  return decodeResult(id, mismatch, length);
}