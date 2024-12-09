#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <set>
#include <queue>
#include <algorithm>

struct funcObj
{
	bool operator()(const std::string& a, const std::string& b) { return a.length() < b.length(); }
};

using namespace std;
using WORD = string;
using WORD_LIST = set<WORD>;
using CHAR_FREQ_LIST = map<char, int>;
using FREQ_WORD_LIST = multimap<int, WORD>;
using COUNTER = tuple<int, int, int>;
using LONGEST = priority_queue<WORD, vector<WORD>, funcObj>;

struct DICTION {
	COUNTER stats; //tuple<int, int, int> stats;
	WORD_LIST words;
	CHAR_FREQ_LIST charFreq;
	FREQ_WORD_LIST freqWord;
	LONGEST longWord;
};

//defining functions above main for readability:
WORD getNextWord(DICTION& d);
void createDict(DICTION& d);
void printBorder(string s);
void printLetterFreq(DICTION& d);
void printDictionary(DICTION& d);
void printHistogram(DICTION& d);
void printChars(int i, string c);

int main() {

	//goal here is to print how many of each letter there are with map
	string testString = "how many letters";

	DICTION diction;
	//all letters a-z
	diction.charFreq = { {'a',0}, {'b',0}, {'c',0}, {'d',0},
		{'e',0}, {'f',0}, {'g',0}, {'h',0}, {'i',0}, {'j',0}, {'k',0},
		{'l',0}, {'m',0}, {'n',0}, {'o',0}, {'p',0}, {'q',0}, {'r',0},
		{'s',0}, {'t',0}, {'u',0}, {'v',0}, {'w',0}, {'x',0}, {'y',0},
		{'z',0}, {'A',0}, {'B',0}, {'C',0}, {'D',0}, {'E',0}, {'F',0},
		{'G',0}, {'H',0}, {'I',0}, {'J',0}, {'K',0}, {'L',0}, {'M',0},
		{'N',0}, {'O',0}, {'P',0}, {'Q',0}, {'R',0}, {'S',0}, {'T',0},
		{'U',0}, {'V',0}, {'W',0}, {'X',0}, {'Y',0}, {'Z',0} };
	//the list of words found in the text file + how many of each
	//diction.freqWord = { {1,"alphabet"}, {7,"micro"}, {3,"work"} };

	//diction.longWord.push("alphabet");
	//diction.longWord.push("work");
	//diction.longWord.push("micro");
	//get<1>(diction.stats) = 3;

	createDict(diction);

	get<2>(diction.stats) = 1; //set the default # of lines

	cout << "Words in dictionary - ";

	for (auto iter = diction.freqWord.begin(); iter != diction.freqWord.end(); iter++) {
		cout << iter->second;
		if (iter == diction.freqWord.end())
			break;
		cout << ",";
	}

	//completes the dictionary, accounting for character/line/word frequency, longest word, etc.
	cout << endl << endl << "Number of Characters  : " << get<0>(diction.stats) << endl;
	cout << "Number of Words       : " << get<1>(diction.stats) << endl;
	cout << "Number of Lines       : " << get<2>(diction.stats) << endl << endl;
	printLetterFreq(diction);
	printDictionary(diction);
	printHistogram(diction);
}

WORD getNextWord(DICTION& d)
{
	WORD w;
	char ch;
	bool inWord = false;

	cin.get(ch);
	while (!cin.eof())
	{
		get<0>(d.stats)++; //inc char by 1
		if (ch == '\n')
		{
			get<2>(d.stats)++; // inc newline by 1
		}
		if (isalpha(ch)) // is ch in [A-Z, a-z]
		{

			w.push_back(ch); //start forming the word
			inWord = true;
			d.charFreq[(ch)]++; //inc the the value at the key of the char
		}
		else if (inWord)
		{
			get<1>(d.stats)++; // inc words by one  
			return w;
		}
		cin.get(ch);
	}
	get<1>(d.stats)++;
	return w;
}

void createDict(DICTION& d) {
	WORD crWord; //holds the current word
	bool isItNew; //keeps track of if a word is new or not

	while (!cin.eof()) {
		crWord = getNextWord(d);

		if (crWord == "")
			continue;

		d.words.insert(crWord);
		isItNew = true;

		//'first' is the key of a map, 'second' is the value.
		//freqWord has words as the values. So, we point to 'second.'
		for (auto iter = d.freqWord.begin(); iter != d.freqWord.end(); iter++) {
			if (iter->second == crWord) {
				int temp = iter->first;
				d.freqWord.erase(iter);
				d.freqWord.insert({ temp + 1, crWord });
				isItNew = false;
				break;
			}
		}
		if (isItNew)
			d.freqWord.insert({ 1, crWord });
		//if there isn't a longest word, or if the current word is longer, push the current word to the top
		if ((d.longWord.empty()) || (d.longWord.top().length() < crWord.length())) {
			d.longWord.push(crWord);
		}
		else if (d.longWord.top().length() == crWord.length())
			d.longWord.push(crWord);
	}
}

void printBorder(string s) {
	cout << "/--------------" << "\\" << endl;
	cout << "| " << s;
	for (int i = s.length(); i <= 12; i++) //places the right wall at the proper place based on string length
		cout << " ";
	cout << "|" << endl;
	cout << "\\" << "--------------/" << endl;
	cout << endl; //just to add an extra space beneath the border
}

void printLetterFreq(DICTION& d) {
	int starNum; //how many * to print
	bool appendNum; //tells program to append true freq of chars if >10
	printBorder("Letter Freq");

	for (auto w : d.charFreq) {
		appendNum = false; //do not append (n) to the * count
		cout << w.first << " |";
		starNum = w.second;

		if (starNum > 10) {
			starNum = 10;
			appendNum = true; //>10 *, so append (n) to the * count
		}
		for (int i = 0; i < starNum; i++) {
			cout << "*";
		}
		if (appendNum) {
			cout << " (" << w.second << ")";
		}
		cout << endl;
	}
}

void printDictionary(DICTION& d) {
	cout << endl; //add an extra space to separate from the prev function's output
	printBorder("Dictionary");
	cout << left << setw(10) << "Word" << "Frequency" << endl;
	printChars(19, "-");
	cout << endl;

	for (auto w : d.freqWord) {
		cout << left << setw(18) << w.second << w.first << endl;
	}
}


void printHistogram(DICTION& d) {
	int wordNum = get<1>(d.stats);
	int longLen = d.longWord.top().length(); //length of the longest word
	int hLineCount = 14;
	int vLineCount = (d.freqWord.size() - 1);
	int crLength;
	int mostStars = 0;

	cout << endl; //add an extra space to separate from the prev function's output
	printBorder("Histogram");

	vector<pair<int, WORD>> wordList(d.freqWord.begin(), d.freqWord.end());
	sort(wordList.begin(), wordList.end(), [](const auto& a, const auto& b)
		{
			return a.second.length() > b.second.length(); // Sort by word length
		});

	for (auto w : wordList) {
		if (w.first > mostStars)
			mostStars = w.first;
	}

	for (int i = mostStars; i > 0; i--) {
		printChars((longLen + hLineCount), " "); //moves cursor to where the stars will be printed

		for (auto& j : wordList) {
			if (j.first == mostStars) {
				cout << "* ";
				j.first--;
			}
			else {
				cout << "  ";
			}
		}
		mostStars--;
		cout << endl;
	}

	printChars(longLen, "-"); //prints lines for the length of the longest word
	printChars(hLineCount, "-"); //print lines essentially acting as a buffer between the beginning and histogram
	printChars((d.freqWord.size() * 2 - 1), "-");
	cout << endl;
	printChars(14 + longLen, " ");

	for (int i = 0; i < d.freqWord.size(); i++)
		cout << "^ ";
	cout << endl;

	for (int j = 0; j < d.freqWord.size(); j++) {
		crLength = wordList[j].second.length();
		cout << wordList[j].second;

		for (int i = crLength; i < longLen; i++)
			cout << " "; //prints the appropriate amount of spaces between the word and lines

		printChars(hLineCount, "-");
		cout << "/";
		printChars(vLineCount, " |");

		hLineCount = hLineCount + 2;
		vLineCount--;
		cout << endl;
	}

}

void printChars(int j, string c) { //print a specified character a specified number of times
	for (int i = 0; i < j; i++) {
		cout << c;
	}
}

/*
void checkChars(string s, map<char, int>& d) {

	for (auto w : s) {
		cout << w << " ";
		if (w == ' '){
			continue;
		}
		cout << w << " ";
		d[w]++; //increments the key's value by one. if no key exists for the character, creates one.
	}
}
//read a text file

//arrow -> operator in c++ accesses members through a pointer.
//It dereferences the pointer and accesses in one step.

//tuple: std::get<0>(d.stats)++ increases the first value of the tuple.
//chars, words, lines

//when part of the struct, you are using d.charFreq[ch]++ instead of d[ch]++
//becuase d is a struct, which contains everything else!!
//charFreq is your map contained within the struct.
*/