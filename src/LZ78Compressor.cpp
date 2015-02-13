#include "LZ78Compressor.h"

void LZ78Compressor::encode(string& text, ofstream& indexFile) {
  int n = text.size();

  // in compression, the dictionary is represented as a tree
  DictionaryTreeNode* dictionaryRoot = new DictionaryTreeNode(0);
  unsigned short nextId = 1;

  DictionaryTreeNode* currentNode = dictionaryRoot;

  vector<CodeWord> codeWords;

  for (int i = 0; i < n; i++) {
    char currentLetter = text.at(i);

    if (currentNode->children.count(currentLetter) > 0) { // w.ti belongs to dictionary
      currentNode = currentNode->children[currentLetter];
    }
    else {
      // add (id, mismatch) to codewords
      cwEncode(currentNode->id, currentLetter, codeWords); 

      if (nextId < DICTIONARY_LIMIT) { // checking dictionary limit
        DictionaryTreeNode* newNode = new DictionaryTreeNode(nextId);
        currentNode->children[currentLetter] = newNode;

        nextId++;
      }

      currentNode = dictionaryRoot;
    }
  }

  if (currentNode != dictionaryRoot) { // w belongs to dictionary
    cwEncode(currentNode->id, LAST_MISMATCH, codeWords); 
  }

  // saving vector of codewords into binary file
  indexFile.write(reinterpret_cast<const char*>(&codeWords[0]), codeWords.size() * sizeof(CodeWord));

  // Deallocating memory
  deleteDictionaryTreeNode(dictionaryRoot);
  codeWords.clear();
}

void LZ78Compressor::cwEncode(unsigned short id, char mismatch, vector<CodeWord>& codeWords) {
  CodeWord codeWord;
  codeWord.id = id;
  codeWord.mismatch = mismatch;

  codeWords.push_back(codeWord);
}

void LZ78Compressor::deleteDictionaryTreeNode(DictionaryTreeNode* node) {
  if (node != NULL) {
    for (unordered_map<char, DictionaryTreeNode*>::iterator it = node->children.begin(); it != node->children.end(); it++) {
      deleteDictionaryTreeNode(node->children[it->first]);
    }

    delete node;
  }
}

string LZ78Compressor::decode(ifstream& indexFile, int indexFileSize) {
  //in decompression, the dictionary is represented as a hashmap, instead of tree
  unordered_map<int, string> dictionary;

  dictionary[0] = "";
  int nextId = 1;

  stringstream textStream;

  // Reading codewords from binary file
  int numberCodeWords = indexFileSize/sizeof(CodeWord);
  CodeWord* codeWords = new CodeWord[numberCodeWords];
  indexFile.read(reinterpret_cast<char*>(&codeWords[0]), numberCodeWords * sizeof(CodeWord));

  // For each codeword, get the dictionaty id and mismatch to append the corresponding string to text
  for (int i = 0; i < numberCodeWords - 1; i++) {
    CodeWord codeWord = codeWords[i];

    textStream << dictionary[codeWord.id] << codeWord.mismatch;

    if (nextId < DICTIONARY_LIMIT) { // checking dictionary limit
      dictionary[nextId] = dictionary[codeWord.id] + codeWord.mismatch;
      nextId++;
    }
  }

  // last one (which might have the special character as the mismatch one, what will not be appended)
  textStream << dictionary[codeWords[numberCodeWords - 1].id];
  if (codeWords[numberCodeWords - 1].mismatch != LAST_MISMATCH) {
    textStream << codeWords[numberCodeWords - 1].mismatch;
  }

  // Deallocating memory
  dictionary.clear();
  delete [] codeWords;

  return textStream.str();
}