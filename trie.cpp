#include "trie.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

trie::trie(int level, char newChar){
	myLevel = level;
	myChar = newChar;
	nextLetter = vector<trie*>('z' - (newChar == ' ' ? 'a' : newChar) + 1, nullptr);
	myList = vector<string>();
}// constructor

trie::~trie() {
	for (int i = 0; i < (int) nextLetter.size(); i++) {
		delete nextLetter[i];
	}
}// de-constructor

void trie::addNewWord(trie* root, string anagram, string word) {
	for (int i = 0; i < (int) anagram.size(); i++) {
		if (root->getNextLetterPtr(anagram[i]) == nullptr){
			root->setNewPtr(anagram[i]);
		}root = root->getNextLetterPtr(anagram[i]);
	}
	root->myList.push_back(word);
}// go through the trie data structure and add new word

trie* trie::getNextLetterPtr(char letter) {
	if (myLevel == 0)
		return nextLetter[letter - 'a'];
	else
		return nextLetter[letter - myChar];
}// return correct pointer to the next char

void trie::setNewPtr(char letter) {
	nextLetter[letter - (myChar == ' ' ? 'a' : myChar)] = new trie(myLevel + 1, letter);
}// build new trie

void trie::print() {
	// used for debugging purpose
	vector<trie*> toBeVisited{ this };
	while (toBeVisited.size()) {
		trie* cur = toBeVisited.back();
		toBeVisited.pop_back();
		cout << cur->myLevel << " " << cur->myChar << " " << cur->nextLetter.size() << endl;
		for (int i = 0; i < cur->myList.size(); i++) {
			cout << cur->myList[i] << endl;
		}
		for (int i = 0; i < cur->nextLetter.size(); i++) {
			if (cur->nextLetter[i] != nullptr) {
				toBeVisited.push_back(cur->nextLetter[i]);
			}
		}
	}
}

set<string> trie::findSolution(string letters, int min_letters) {
	// find all the solutions with a minimum len of min_letters
	set<string> re;
	sort(letters.begin(), letters.end());
	vector<trie*> location(pow(2, letters.size()), nullptr);
	location[0] = this;
	for (unsigned int i = 1; i < location.size(); i++) {
		//bit smearing, get all the bits after the left most 1 bits to be 1s
		unsigned int tmp = i;
		tmp |= tmp >> 16;
		tmp |= tmp >> 8;
		tmp |= tmp >> 4;
		tmp |= tmp >> 2;
		tmp |= tmp >> 1;
		tmp ^= tmp >> 1;
		//now only the leftmost 1 bit is on

		unsigned int prev = i - tmp;
		if (location[prev] == nullptr) {
			continue;
		}
		else {
			location[i] = location[prev]->getNextLetterPtr(letters[(int)log2(tmp)]);
			// use pointers generated previously to avoid traversing through the same trie
			if (location[i] == nullptr) 
				continue;
			if (location[i]->myLevel >= min_letters && location[i]->myList.size())
				re.insert(location[i]->myList.begin(), location[i]->myList.end());
		}
	}
	return re;
}
