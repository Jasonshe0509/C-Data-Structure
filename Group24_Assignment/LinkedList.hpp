#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> //transform, remove_if
#include <cctype> //tolower, ispunct, isspace
#include <chrono> //timing
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct singleListNode {
	string word;
	int count;

	singleListNode* next = nullptr;
};

struct doubleListNode {
	string word;
	int count;
	doubleListNode* prev = nullptr;
	doubleListNode* next = nullptr;
};

struct HashNode {
	string key;        // the word (key)
	doubleListNode* value;   // pointer to singleListNode in the singlyWordList
	HashNode* next;    // pointer to the next node

	HashNode(string k, doubleListNode* v) : key(k), value(v), next(nullptr) {}
};

struct HashTable {
	HashNode** table;   // array of pointers to HashNodes (buckets)
	int capacity;       // number of buckets
	int size;           // number of elements

	HashTable(int capacity);
	int hashFunction(string key);
	void insert(string key, doubleListNode* value);
	doubleListNode* search(singleListNode* wordNode);
	~HashTable();
};

struct list { // store review and rating nodes in singly linked list
	singleListNode* head;
	singleListNode* tail;
	int size = 0;

	list();
	singleListNode* createNewNode(string word, int count);
	void insert(string word, int count);
	void display();
	void printCSV();
	~list();
};

struct doublyWordList { // store positive & negative word nodes in doubly linked list
	doubleListNode* head;
	doubleListNode* tail;
	HashTable* hashTable;

	doublyWordList();
	doublyWordList(int capacity);
	doubleListNode* createDoublyListNode(string word, int count);
	void insertDoublyNodeAtEnd(string word, int count = 0, string type = "linear");
	void display(string display);
	//binary search
	doubleListNode* findDoublyMidPt(doubleListNode* head, doubleListNode* tail);
	doubleListNode* findReverseDoublyMidPt(doubleListNode* head, doubleListNode* tail);
	void twoWaybinarySearch(singleListNode* wordInReview, doublyWordList* positiveList, doublyWordList* negtiveList, doublyWordList& tempPositiveWords, doublyWordList& tempNegativeWords);
	void saveWordsInReviewToTempList(singleListNode* review, list& tempList);
	// direct search
	bool hashSearch(singleListNode* wordNode);
	// linear search
	bool linearsearch(string word, singleListNode* wordNode);
	// insertion sort algorithm
	void insertionsort();
	//merge sort algorithm
	doubleListNode* splitDoublyList(doubleListNode* midpoint);
	doubleListNode* doublymerge(doubleListNode* leftList, doubleListNode* rightList);
	doubleListNode* doublymergeSort(doubleListNode* head);
	//analysis methods
	void analyzeReviewDoubly(list& feedbackList, doublyWordList& positiveList, doublyWordList& negativeList, string searchType, string sortType);
	void displayMaxMinWords(doubleListNode* posNode, doubleListNode* negNode, ofstream& outFile);
	void displayMatches(doubleListNode* head, ofstream& outFile);
	~doublyWordList();
};