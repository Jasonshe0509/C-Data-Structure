#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cctype> 

using namespace std;
using namespace std::chrono;

struct WordItem {
	string words;
	int count;
};

struct FeedbackItem {
	string reviews;
	int ratings;
};

class ArrayList {
	WordItem* wordList;
	FeedbackItem* feedbackList;
	string arrayName;
	int arraySize;
public:
	ArrayList(string arrayName) {
		this->arrayName = arrayName;
		this->arraySize = 0;
		this->wordList = nullptr;
		this->feedbackList = nullptr;
	}

	int countFileLines(string filename);

	void setWords(int index, string words);

	void setFeedbackWords(int index, string reviews, int ratings);

	void readFileData(string filename, string fileType);

	void displayData(string dataType);

	void updateWordCount(string word, ArrayList& wordsList, ArrayList& wordsFound, int& index, int& count, string search);

	void countSentimentWords(string review, int& positiveCount, int& negativeCount, ArrayList& positiveWordsList, ArrayList& negativeWordsList, ArrayList& positiveWordsFound, ArrayList& negativeWordsFound, int& posWordsIndex, int& negWordsIndex, string search);

	void getTop10Words(string sort);

	void getMostFrequentAndLeastFrequentWords();

	void displayMaxAndMinUsedWordsCombined(ArrayList& positiveWordsList, ArrayList& negativeWordsList);

	void analyzeFeedback(ArrayList& positiveWordsList, ArrayList& negativeWordsList, string search,string sort);

	~ArrayList();
};