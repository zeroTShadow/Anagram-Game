#pragma once
#include <vector>
#include <set>
#include <string>

using namespace std;
class trie
{
public:
	int myLevel;
	char myChar;
	vector<trie*> nextLetter;
	vector<string> myList;

	trie::trie(int level, char newChar);
	trie::~trie();
	void trie::addNewWord(trie* root, string anagram, string word);
	trie* trie::getNextLetterPtr(char letter);
	void trie::setNewPtr(char letter);
	void trie::print();
	set<string> trie::findSolution(string letters, int min_letters);
};

