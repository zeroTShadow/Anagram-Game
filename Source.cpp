# Anagram-Game
a c++ program in which user attempts to guess as many words as possible given a string of random characters

#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <string>
#include <set>
#include <vector>
#include "trie.h"

using namespace std;

void generateLetters(string &letters, int numOfLetters) {
	string vowels, consonants;
	srand(time(0));
	for (int i = 0; i < numOfLetters; i++) {
		char tmp = (char)(rand() % 26 + 'a');
		if (tmp == 'a' || tmp == 'e' || tmp == 'i' || tmp == 'o' || tmp == 'u' || tmp == 'y')
			vowels.push_back(tmp);
		else
			consonants.push_back(tmp);
	}
	if (vowels.size() && consonants.size() >= vowels.size())
		letters = vowels + consonants;
	else
		generateLetters(letters, numOfLetters);
}

int main() {
	ifstream dictionary;
	string newWord, sorted;
	trie root(0,' ');

	dictionary.open("C:\\Desktop\\Anagram\\vocabulary.txt");
	if (!dictionary) {
		cout << "ERROR in opening the dictionary file. System exiting!!!" << endl;
		exit(1);
	}//attempt to obtain the dictionary file

	while (getline(dictionary, newWord)) {
		sorted = newWord;
		sort(sorted.begin(), sorted.end());

		//only words composed of only letters will be included
		//names are not included within this library of vocabulary
		if (!islower(sorted[0]) || !islower(sorted.back()))
			continue;
		root.addNewWord(&root, sorted, newWord);
	}
	dictionary.close();
	//add the words one by one and close the file after it is completed

	cout << "Library was established. Game is ready to be played" << endl;
	cout << "Enter 1 for easy mode, \n2 for medium difficulty, \n3 for hard, \n4 to exit" << endl;
	int mode = 0;
	string input = "";
	//set a random seed every time this program is initiated
	while (getline(cin, input)) {
		// read the inputs to determine the game mode
		if (input.size() != 1 || !isdigit(input[0])) {
			cout << "incorrect input format" << endl;
			continue;
		}
		mode = stoi(input);
		if (mode == 4) break;
		else if (mode < 1 || mode>4) {
			cout << "incorrect input format" << endl;
			continue;
		}

		string allowedLetter = "";
		set<string> toBeGuessed;
		vector<string> guessed;
		int wrongInput = 0, max_wrong = 5, 
			numOfChar = mode == 1 ? 5 : mode == 2 ? 7 : 9;

		while (!toBeGuessed.size()) {
			generateLetters(allowedLetter, numOfChar);
			toBeGuessed = root.findSolution(allowedLetter, 2 + mode);
			// obtain all the possible combinations from the data structures
		}
		cout << "Letter for this round:" << endl;
		for (char i : allowedLetter) {
			cout << i << " ";
		}cout << endl;
		cout << "There is/are " << toBeGuessed.size() << " words of length " << 2 + mode <<
			" or longer left to be guessed\n\n\n" << endl;

		while(wrongInput<=max_wrong && toBeGuessed.size() && getline(cin, input)){
			if(toBeGuessed.find(input)!=toBeGuessed.end()){
				cout<<"Correct."<<endl;
				guessed.push_back(input);
				toBeGuessed.erase(input);
			}else{
				cout<<"Error. "<<input<<" is not in the dictionary at this moment."<<endl;
				wrongInput++;
			}
			cout << "Letter for this round:" << endl;
			for (char i : allowedLetter) {
				cout << i << " ";
			}cout << endl;
			cout << "You have guessed the following:" << endl;
			for (string vocab : guessed) {
				cout << vocab << endl;
			}
			cout << "There is/are " << toBeGuessed.size() << " words of length " << 2 + mode <<
				" or longer left to be guessed\n\n\n" << endl;
		}// reading user's guesses and keep track of the number of failed guesses

		if(toBeGuessed.size()){
			cout << "You have guessed incorrectly more than 5 times" << endl;
			cout << "Here are the ones that were not guessed by you" << endl;
			for (set<string>::iterator it = toBeGuessed.begin(); it != toBeGuessed.end(); it++) {
				cout<<*it<<endl;
			}
		}else{
			cout << "Congratulation. You have guessed all the possible combinations" << endl;
		}// game ended, print result

		cout << "Enter 1 for easy mode, \n2 for medium difficulty, \n3 for hard, \n4 to exit" << endl;
	}
	return 0;
}
