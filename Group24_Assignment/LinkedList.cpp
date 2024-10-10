#include "LinkedList.hpp"

// Function to cout messages in txt file and console terminal
static void logToFileAndConsole(ofstream& outFile, const string& message) {
	cout << message;
	outFile << message;
}

/// list methods implementation
list::list() {
	head = nullptr;
	tail = nullptr;
}

singleListNode* list::createNewNode(string word, int count) {
	singleListNode* newNode = new singleListNode();
	newNode->word = word;
	newNode->count = count;
	newNode->next = nullptr;

	return newNode;
}

void list::insert(string word, int count) { //insert from front list
	singleListNode* newNode = createNewNode(word, count);

	if (head != nullptr) {
		tail->next = newNode;
		tail = newNode;
	}
	else {
		head = tail = newNode;
	}
	size++;
}

void list::display() {
	singleListNode* temp = head;
	if (temp != nullptr) {
		cout << "Review data loaded" << endl;
	}
}

//Function to convert analysis summary into CSV file 
void list::printCSV() {
	if (head == nullptr) {
		cout << "The list is empty, nothing to print." << endl;
		return;
	}

	time_t t = time(nullptr);
	tm tm;
	localtime_s(&tm, &t);

	ostringstream oss;
	oss << put_time(&tm, "%Y-%m-%d_%H-%M");
	string timestamp = oss.str();
	string filename = "Sentiment_Analysis_Reviews_" + timestamp + ".csv";

	ofstream outFile(filename);

	if (!outFile) {
		cout << "Error creating file: " << filename << endl;
		return;
	}

	// Write headers of CSV file
	outFile << "Review,Analysis Rating" << endl;

	singleListNode* current = head;
	while (current != nullptr) {
		// Enclose review in quotes to prevent splitting on commas
		outFile << "\"" << current->word << "\"," << current->count << endl;
		current = current->next; // Move to the next node
	}

	outFile.close();
	cout << "Reviews have been written to: " << filename << endl;
}

// Destructor
list::~list() {
	singleListNode* current = head;
	singleListNode* nextNode;
	while (current != nullptr) {
		nextNode = current->next;
		delete current;
		current = nextNode;
	}
	head = nullptr;
	size = 0;
}

/// Additional: Hash Table implementation 
HashTable::HashTable(int capacity) {
	this->capacity = capacity;
	this->size = 0;
	table = new HashNode * [capacity];

	for (int i = 0; i < capacity; i++) {
		table[i] = nullptr;
	}
}

int HashTable::hashFunction(string key) {
	unsigned long hash = 5381; // Starting value for the hash
	for (unsigned char ch : key) { // Use unsigned char to avoid sign issues
		hash = ((hash << 5) + hash) + ch; // hash * 33 + c
	}
	int index = hash % capacity;
	if (index < 0 || index >= capacity) {
		cerr << "Invalid hash index: " << index << " for key: " << key << std::endl;
		return 0;
	}
	return index;
}

void HashTable::insert(string key, doubleListNode* value) {
	int index = hashFunction(key);

	// Check if the key already exists (update the value if it does)
	HashNode* temp = table[index];
	while (temp != nullptr) {
		if (temp->key == key) {
			temp->value = value;
			return;
		}
		temp = temp->next;
	}

	// If the key does not exist, create a new node and insert it at the beginning
	HashNode* newNode = new HashNode(key, value);
	newNode->next = table[index];
	table[index] = newNode;
	size++;
}

doubleListNode* HashTable::search(singleListNode* wordNode) {
	int index = hashFunction(wordNode->word);

	HashNode* temp = table[index];
	while (temp != nullptr) {
		if (temp->key == wordNode->word) {
            temp->value->count += wordNode->count;
			return temp->value;
		} 
		temp = temp->next;
	}
	return nullptr;
}

HashTable::~HashTable() {
	for (int i = 0; i < capacity; i++) {
		HashNode* temp = table[i];
		while (temp != nullptr) {
			HashNode* toDelete = temp;
			temp = temp->next;
			delete toDelete;
		}
	}
	delete[] table;
}

/// doublyWordList methods implementation
doublyWordList::doublyWordList() {
	head = nullptr;
	tail = nullptr;
}

doublyWordList::doublyWordList(int capacity) {
	head = nullptr;
	tail = nullptr;
	hashTable = new HashTable(capacity);
}

//destructor
doublyWordList::~doublyWordList() {
	doubleListNode* current = head;
	while (current) {
		doubleListNode* nextNode = current->next; 
		delete current;
		current = nextNode;
	}
	// After deletion, set both head and tail to nullptr
	head = nullptr;
	tail = nullptr;
}

doubleListNode* doublyWordList::createDoublyListNode(string word, int count) {
	doubleListNode* newDoublyNode = new doubleListNode;
	newDoublyNode->word = word;
	newDoublyNode->count = count;
	newDoublyNode->prev = nullptr;
	newDoublyNode->next = nullptr;
	return newDoublyNode;
}

void doublyWordList::insertDoublyNodeAtEnd(string word, int count, string type) {
	if (type == "linear" || type == "binary") {
		doubleListNode* newDoublyNode = createDoublyListNode(word, count);
		if (head != nullptr) {
			tail->next = newDoublyNode;
			newDoublyNode->prev = tail;
			tail = newDoublyNode;
		}
		else {
			head = tail = newDoublyNode;
		}
	} 
	if (type == "hash") {
		doubleListNode* newDoublyNode = createDoublyListNode(word, count);
		if (!head) {
			head = newDoublyNode;
		}
		else {
			doubleListNode* temp = head;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = newDoublyNode;
			newDoublyNode->prev = temp;
		}
		// Insert into the hash table for O(1) search
		hashTable->insert(word, newDoublyNode);
	}
}

void doublyWordList::display(string display) {
	doubleListNode* temp = head;
	if (temp != nullptr) {
		cout << display << " words loaded" << endl;
	}
}

// Function to save preprocessed words into temporary review linked list
void doublyWordList::saveWordsInReviewToTempList(singleListNode* review, list& tempList) {
	//Remove punctuation
	review->word.erase(remove_if(review->word.begin(), review->word.end(), [](unsigned char c) {
		return (c < 32 || c > 126 || (ispunct(c) && c != '.' && c != '/' && c != '+' && c != '*'));
		}), review->word.end());

	//Replace all periods ('.' and '/') with spaces
	replace(review->word.begin(), review->word.end(), '.', ' ');
	replace(review->word.begin(), review->word.end(), '/', ' ');

	//Convert to lowercase
	transform(review->word.begin(), review->word.end(), review->word.begin(), ::tolower);

	istringstream iss(review->word);
	string wordInReview;
	while (iss >> wordInReview) {
		if (!wordInReview.empty()) {
			//Check if the word already exists in the list
			singleListNode* current = tempList.head;
			bool found = false;
			while (current != nullptr) {
				if (current->word == wordInReview) {
					current->count++;  // Increment count if word is found
					found = true;
					break;
				}
				current = current->next;
			}
			//If the word is not found, insert it with a count of 1
			if (!found) {
				tempList.insert(wordInReview, 1);
			}
		}
	}
}

//binary search algorithm implementation (doubly linked list)
doubleListNode* doublyWordList::findDoublyMidPt(doubleListNode* head, doubleListNode* tail) {
	if (head == nullptr) {
		return nullptr;
	}

	doubleListNode * slow = head;
	doubleListNode* fast = head;

	while (fast != tail && fast->next != tail && fast->next != nullptr && fast->next->next != nullptr) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

doubleListNode* doublyWordList::findReverseDoublyMidPt(doubleListNode* head, doubleListNode* tail) {
	if (head == nullptr) {
		return nullptr;
	}

	doubleListNode* slow = head;
	doubleListNode* fast = head;

	while (fast != tail && fast->prev != tail && fast->prev != nullptr && fast->prev->prev != nullptr) {
		slow = slow->prev;
		fast = fast->prev->prev;
	}

	return slow;
}

void doublyWordList::twoWaybinarySearch(singleListNode* wordInReview, doublyWordList* positiveList, doublyWordList* negativeList, doublyWordList& tempPositiveWords, doublyWordList& tempNegativeWords) {
	if (!positiveList || !negativeList) {
		return ;
	}

	//Positive search setup
	doubleListNode* positivestart = positiveList->head;
	doubleListNode* positivestartEnd = nullptr;
	doubleListNode* positivetail = positiveList->tail;
	doubleListNode* positivetailEnd = nullptr;

	//Negative search setup
	doubleListNode* negativestart = negativeList->head;
	doubleListNode* negativestartEnd = nullptr;
	doubleListNode* negativetail = negativeList->tail;
	doubleListNode* negativetailEnd = nullptr;

	while (positivetail != positivetailEnd && positivestart != nullptr && positivetail != nullptr) {

		doubleListNode* positivestart_middleNode = findDoublyMidPt(positivestart, positivestartEnd);
		doubleListNode* positivetail_middleNode = findReverseDoublyMidPt(positivetail, positivetailEnd);

		//Check if the middle node or start/tail matches the word
		if (!positivestart_middleNode && !positivetail_middleNode) {
			break;
		}

		//Positive word match cases
		if (positivestart->word == wordInReview->word) {
			positivestart->count += wordInReview->count;
			tempPositiveWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}
		else if (positivestart_middleNode->word == wordInReview->word) {
			positivestart_middleNode->count += wordInReview->count;
			tempPositiveWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}
		if (positivetail_middleNode->word == wordInReview->word) {
			positivetail_middleNode->count += wordInReview->count;
			tempPositiveWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}
		else if (positivetail->word == wordInReview->word) {
			positivetail->count += wordInReview->count;
			tempPositiveWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}

		//continue search in positive 
		if (positivestart_middleNode->word < wordInReview->word) {
			positivestart = positivestart_middleNode->next;
		}
		else {
			positivestartEnd = positivestart_middleNode;
		}
		if (positivetail_middleNode->word > wordInReview->word) {
			positivetail = positivetail_middleNode->prev;
		}
		else {
			positivetailEnd = positivetail_middleNode;
		}
	}
	while (negativestart != negativestartEnd && negativetail != negativetailEnd && negativestart != nullptr && negativetail != nullptr) {
		doubleListNode* negativestart_middleNode = findDoublyMidPt(negativestart, negativestartEnd);
		doubleListNode* negativetail_middleNode = findReverseDoublyMidPt(negativetail, negativetailEnd);

		//Check if the middle node or start/tail matches the word
		if (!negativestart_middleNode || !negativetail_middleNode) {
			break;
		}

		//Negative word match cases
		if (negativestart->word == wordInReview->word) {
			negativestart->count += wordInReview->count;
			tempNegativeWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}
		else if (negativestart_middleNode->word == wordInReview->word) {
			negativestart_middleNode->count += wordInReview->count;
			tempNegativeWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;

		}
		if (negativetail_middleNode->word == wordInReview->word) {
			negativetail_middleNode->count += wordInReview->count;
			tempNegativeWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;

		}
		else if (negativetail->word == wordInReview->word) {
			negativetail->count += wordInReview->count;
			tempNegativeWords.insertDoublyNodeAtEnd(wordInReview->word, wordInReview->count, "binary");
			break;
		}

		//continue search in negative
		if (negativestart_middleNode->word < wordInReview->word) {
			negativestart = negativestart_middleNode->next;
		}
		else {
			negativestartEnd = negativestart_middleNode;
		}
		if (negativetail_middleNode->word > wordInReview->word) {
			negativetail = negativetail_middleNode->prev;
		}
		else {
			negativetailEnd = negativetail_middleNode;
		}
	}
	return;
}

//direct search - link to hash search
bool doublyWordList::hashSearch(singleListNode* wordNode) {
	return hashTable->search(wordNode);
}

//linear search algorithm implementation
bool doublyWordList::linearsearch(string word, singleListNode* wordNode) {
	doubleListNode* temp = head;
	while (temp) {
		if (temp->word == word) {
			temp->count += wordNode->count;
			return true;
		}
		temp = temp->next;
	}
	return false;
}

//insertion sort algorithm implementation
void doublyWordList::insertionsort() {
	if (head == nullptr || head->next == nullptr) {
		return;
	}

	doubleListNode* sorted = nullptr;  // Sorted portion of the list

	doubleListNode* current = head;
	while (current != nullptr) {
		doubleListNode* next = current->next;

		// If sorted list is empty or the current node should be placed before the first node in sorted
		if (sorted == nullptr ||
			(current->count < sorted->count) ||
			(current->count == sorted->count && current->word < sorted->word)) {
			current->next = sorted;
			sorted = current;
		}
		else {
			// Insert current in the correct position within the sorted list
			doubleListNode* temp = sorted;
			while (temp->next != nullptr &&
				(temp->next->count < current->count ||
					(temp->next->count == current->count && temp->next->word < current->word))) {
				temp = temp->next;
			}

			current->next = temp->next;
			temp->next = current;
		}

		// Move to the next node in the unsorted part of the list
		current = next;
	}

	head = sorted;
}


//merge sort algorithm implementation
doubleListNode* doublyWordList::splitDoublyList(doubleListNode* midpoint) {
	if (!midpoint || !midpoint->next) {
		return nullptr;
	}
	doubleListNode* right_headNode = midpoint->next;
	midpoint->next = nullptr;
	if (right_headNode) {
		right_headNode->prev = nullptr;
	}
	return right_headNode;
}

doubleListNode* doublyWordList::doublymerge(doubleListNode* leftList, doubleListNode* rightList) {
	if (!leftList) return rightList;
	if (!rightList) return leftList;

	// Create a dummy node to serve as the starting point for the merged list
	doubleListNode dummy;
	doubleListNode* tail = &dummy;

	// Iteratively merge two lists
	while (leftList && rightList) {
		if (leftList->count < rightList->count) {
			tail->next = leftList;
			leftList->prev = tail;
			leftList = leftList->next;
		}
		else if (leftList->count == rightList->count) {
			if (leftList->word < rightList->word) {
				tail->next = leftList;
				leftList->prev = tail;
				leftList = leftList->next;
			}
			else {
				tail->next = rightList;
				rightList->prev = tail;
				rightList = rightList->next;
			}
		}
		else {
			tail->next = rightList;
			rightList->prev = tail;
			rightList = rightList->next;
		}
		tail = tail->next;
	}

	// Attach the remaining part of the lists
	if (leftList) {
		tail->next = leftList;
		leftList->prev = tail;
	}
	else {
		tail->next = rightList;
		rightList->prev = tail;
	}

	// Return the merged list, excluding the dummy node
	return dummy.next;
}


doubleListNode* doublyWordList::doublymergeSort(doubleListNode* head) {
	if (!head || !head->next) {
		return head;
	}
	doubleListNode* end = nullptr;
	doubleListNode* midPoint = findDoublyMidPt(head, end);
	doubleListNode* rightListHead = splitDoublyList(midPoint);//firstly find midpoint then split the list

	doubleListNode* leftsorted = doublymergeSort(head);
	doubleListNode* rightsorted = doublymergeSort(rightListHead);

	return doublymerge(leftsorted, rightsorted);
}

// feedback analysis
// Function to process and count positive and negative words in a review
int countSentimentScore(int positiveCount, int negativeCount) {
	// Calculate sentiment score based on word counts
	int rawSentimentScore = positiveCount - negativeCount;
	int N = positiveCount + negativeCount;
	if (N == 0) return 3.0;
	int minRawScore = -N;
	int maxRawScore = +N;

	double normalizedScore = (rawSentimentScore - minRawScore) / double(maxRawScore - minRawScore);
	double sentimentScore = 1 + (4 * normalizedScore);

	cout << fixed << setprecision(2);
	cout << "Sentiment score (1 - 5) is " << sentimentScore << endl;

	int finalSentimentScore = round(sentimentScore);

	return finalSentimentScore;
}

int compareRatingScore(int totalCorrectReview, int sentimentScore, int rating) {
	if (sentimentScore > 3 && sentimentScore <= 5) {
		cout << "Rating should be " << sentimentScore << " (Positive)" << endl << endl;
	}
	else if (sentimentScore == 3) {
		cout << "Rating should be " << sentimentScore << " (Neutral)" << endl << endl;
	}
	else if (sentimentScore >= 1 && sentimentScore < 3){
		cout << "Rating should be " << sentimentScore << " (Negative)" << endl << endl;
	}

	// Function to compare review score and sentiment score
	cout << endl << string(20, '-') << "Comparison" << string(20, '-') << endl;
	cout << "Sentiment Score (1 - 5) = " << sentimentScore << endl;
	cout << "Rating given by user = " << rating << endl << endl;

	cout << "Analysis output:" << endl;
	if (sentimentScore == rating) {
		totalCorrectReview++;
		cout << "User's subjective evaluation matches the sentiment score provided by the analysis." << endl
			<< "There is a consistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl << endl;
	}
	else {
		cout << "User's subjective evaluation does not match the sentiment score provided by the analysis." << endl
			<< "There is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl << endl;
	}
	return totalCorrectReview;
}

void doublyWordList::analyzeReviewDoubly(list& feedbackList, doublyWordList& positiveList, doublyWordList& negativeList, string searchType, string sortType) {
	time_t t = time(nullptr);
	tm tm;
	localtime_s(&tm, &t);

	ostringstream oss;
	oss << put_time(&tm, "%Y-%m-%d_%H-%M");
	string timestamp = oss.str();
	string filename = "Review_Analysis_Report " + timestamp + ".txt";

	ofstream outFile(filename);

	if (!feedbackList.head || !positiveList.head || !negativeList.head) {
		cout << "One of the lists is empty." << endl;
		return;
	}

	int reviewPrintCounter = 1, reviewCounter = 0;
	int positiveCounter = 0, negativeCounter = 0;
	int totalCorrectReview = 0;
	singleListNode* reviewTemp = feedbackList.head;

	doublyWordList tempPositiveWords(300);
	doublyWordList tempNegativeWords(300);

	while (reviewTemp) {
		cout << "Review " << reviewPrintCounter << " :" << reviewTemp->word << endl << endl;
		list wordList;
		saveWordsInReviewToTempList(reviewTemp, wordList);
		
		int positiveCount = 0;
		int negativeCount = 0;
		singleListNode* currentWordNode = wordList.head;
		
		while (currentWordNode != nullptr) {
			string wordInReview = currentWordNode->word;
			if (positiveList.head != nullptr || negativeList.head != nullptr) {
				if (searchType == "binary") {
					twoWaybinarySearch(currentWordNode, &positiveList, &negativeList, tempPositiveWords, tempNegativeWords);
				}
				else if (searchType == "hash") {
					// Direct search for word in positive and negative lists using hash table
					if (positiveList.hashSearch(currentWordNode)) {
						tempPositiveWords.insertDoublyNodeAtEnd(wordInReview, currentWordNode->count, "hash");
					}
					if (negativeList.hashSearch(currentWordNode)) {
						tempNegativeWords.insertDoublyNodeAtEnd(wordInReview, currentWordNode->count, "hash");
					}
				}
				else if (searchType == "linear") {
					// Linear search for word in positive and negative lists
					if (positiveList.linearsearch(wordInReview, currentWordNode)) {
						tempPositiveWords.insertDoublyNodeAtEnd(wordInReview, currentWordNode->count, "linear");
					}
					if (negativeList.linearsearch(wordInReview, currentWordNode)) {
						tempNegativeWords.insertDoublyNodeAtEnd(wordInReview, currentWordNode->count, "linear");
					}
				}
			}
			currentWordNode = currentWordNode->next;
		}
		cout << "Positive List: " << endl;
		while (tempPositiveWords.head != nullptr) {
			//count for every single review
			positiveCount = positiveCount + tempPositiveWords.head->count;
			//count for overall 
			positiveCounter = positiveCounter + tempPositiveWords.head->count;
			cout << "~ " << tempPositiveWords.head->word << " with frequency of " << tempPositiveWords.head->count << endl;

			//delete temp positive list
			doubleListNode* tempPositiveWord = nullptr;
			tempPositiveWord = tempPositiveWords.head;
			tempPositiveWords.head = tempPositiveWords.head->next;
			delete tempPositiveWord;
		}
		cout << "Total: " << positiveCount << endl << endl;

		cout << "Negative List: " << endl;
		while (tempNegativeWords.head != nullptr) {
			//count for every single review
			negativeCount = negativeCount + tempNegativeWords.head->count;
			//count for overall 
			negativeCounter = negativeCounter + tempNegativeWords.head->count;
			cout << "~ " << tempNegativeWords.head->word << " with frequency of " << tempNegativeWords.head->count << endl;

			//delete temp negative list
			doubleListNode* tempNegativeWord = nullptr;
			tempNegativeWord = tempNegativeWords.head;
			tempNegativeWords.head = tempNegativeWords.head->next;
			delete tempNegativeWord;
		}
		cout << "Total: " << negativeCount << endl << endl;

		int sentimentScore = countSentimentScore(positiveCount, negativeCount);
		totalCorrectReview = compareRatingScore(totalCorrectReview, sentimentScore, reviewTemp->count);
		reviewCounter++;
		reviewPrintCounter++;
		reviewTemp = reviewTemp->next;
	}

	logToFileAndConsole(outFile, "Total Reviews: " + to_string(reviewCounter) + "\n");
	logToFileAndConsole(outFile, "Total Counts of positive words: " + to_string(positiveCounter) + "\n");
	logToFileAndConsole(outFile, "Total Counts of negative words: " + to_string(negativeCounter) + "\n");
	logToFileAndConsole(outFile, "\n");

	double totalCorrectPercentage = (double(totalCorrectReview) / reviewCounter) * 100;

	if (sortType == "insertion") {
		logToFileAndConsole(outFile, "The accuracy of user rating based on the given feedback will be: " + to_string(totalCorrectPercentage) + "%\n");
		logToFileAndConsole(outFile, "\n");
		logToFileAndConsole(outFile, "Frequency of each word used in overall reviews, listed in descending order based on frequency: \n");
		logToFileAndConsole(outFile, "Positive Words: \n");
		positiveList.insertionsort();
		doubleListNode* positiveWords = positiveList.head;
		positiveList.displayMatches(positiveWords, outFile);
		logToFileAndConsole(outFile, "\n");
		logToFileAndConsole(outFile, "Negative Words: \n");
		negativeList.insertionsort();
		doubleListNode* negativeWords = negativeList.head;
		positiveList.displayMatches(negativeWords, outFile);
		logToFileAndConsole(outFile, "\n");

		// Display the maximum and minimum used words
		displayMaxMinWords(positiveWords, negativeWords, outFile);
		logToFileAndConsole(outFile, "\n");
	}
	else if (sortType == "merge") {
		logToFileAndConsole(outFile, "The accuracy of user rating based on the given feedback will be: " + to_string(totalCorrectPercentage) + "%\n");
		logToFileAndConsole(outFile, "\n");
		logToFileAndConsole(outFile, "Frequency of each word used in overall reviews, listed in descending order based on frequency: \n");
		logToFileAndConsole(outFile, "Positive Words: \n");
		doubleListNode* positiveWords = positiveList.head;
		positiveWords = positiveList.doublymergeSort(positiveWords);
		positiveList.displayMatches(positiveWords, outFile);
		logToFileAndConsole(outFile, "\n");

		logToFileAndConsole(outFile, "Negative Words: \n");
		doubleListNode* negativeWords = negativeList.head;
		negativeWords = negativeList.doublymergeSort(negativeWords);
		negativeList.displayMatches(negativeWords, outFile);
		logToFileAndConsole(outFile, "\n");

		// Display the maximum and minimum used words
		displayMaxMinWords(positiveWords, negativeWords, outFile);
		logToFileAndConsole(outFile, "\n");
	}
	
	outFile.close();
}

//Function to display max and min words based on their frequency (positive & negative)
void doublyWordList::displayMaxMinWords(doubleListNode* posNode, doubleListNode* negNode, ofstream& outFile) {
	if (posNode == nullptr && negNode == nullptr) {
		logToFileAndConsole(outFile, "No words to process!\n");
		return;
	}

	doubleListNode* maxPositive = nullptr;
	doubleListNode* minPositive = nullptr;
	doubleListNode* maxNegative = nullptr;
	doubleListNode* minNegative = nullptr;

	// Create doubly lists to store words with the same minimum frequency
	doublyWordList minPositiveWords(300);
	doublyWordList minNegativeWords(300);

	// Find max and min positive words
	doubleListNode* tempPos = posNode;

	if (tempPos) {
		// Initialize maxPositive to the first node
		maxPositive = tempPos;

		// Find the first word with count > 0 for minPositive initialization
		while (tempPos && tempPos->count == 0) {
			tempPos = tempPos->next;
		}

		// If no word has count > 0, there's no valid minPositive
		if (tempPos) {
			minPositive = tempPos; 
		}
		else {
			logToFileAndConsole(outFile, "No positive words with count > 0\n");
			return;
		}

		// Reset tempPos to the head to traverse the entire list for max and min
		tempPos = posNode;

		while (tempPos) {
			if (tempPos->count > maxPositive->count) {
				maxPositive = tempPos;
			}
			if (tempPos->count > 0 && tempPos->count < minPositive->count) {
				minPositive = tempPos;
			}
			tempPos = tempPos->next;
		}

		// Gather all words with the same minimum frequency into the linked list
		tempPos = posNode;
		while (tempPos) {
			if (tempPos->count == minPositive->count && tempPos->count > 0) {
				minPositiveWords.insertDoublyNodeAtEnd(tempPos->word, tempPos->count);
			}
			tempPos = tempPos->next;
		}
	}

	// Find max and min negative words
	doubleListNode* tempNeg = negNode;
	if (tempNeg) {
		// Initialize maxPositive to the first node
		maxNegative = tempNeg;

		// Find the first word with count > 0 for minPositive initialization
		while (tempNeg && tempNeg->count == 0) {
			tempNeg = tempNeg->next;
		}

		// If no word has count > 0, there's no valid minPositive
		if (tempNeg) {
			minNegative = tempNeg;
		}
		else {
			logToFileAndConsole(outFile, "No positive words with count > 0\n");
			return;
		}

		// Reset tempPos to the head to traverse the entire list for max and min
		tempNeg = negNode;

		// Traverse the negative list to find max and min negative words
		while (tempNeg) {
			if (tempNeg->count > maxNegative->count) {
				maxNegative = tempNeg;
			}
			if (tempNeg->count > 0 && tempNeg->count < minNegative->count) {
				minNegative = tempNeg;
			}
			tempNeg = tempNeg->next;
		}

		// Gather all words with the same minimum frequency into the linked list
		tempNeg = negNode;
		while (tempNeg) {
			if (tempNeg->count == minNegative->count && tempNeg->count > 0) {
				minNegativeWords.insertDoublyNodeAtEnd(tempNeg->word, tempNeg->count);
			}
			tempNeg = tempNeg->next;
		}
	}

	// Prepare the output streams
	stringstream maxWordsStream;
	maxWordsStream << endl << "Maximum and Minimum Used Words:" << endl;
	maxWordsStream << string(50, '-') << endl;

	// Display max positive word
	if (maxPositive) {
		maxWordsStream << "Maximum used positive word: " << maxPositive->word << " (" << maxPositive->count << " times)" << endl;
	}
	else {
		maxWordsStream << "No maximum positive words found." << endl;
	}

	// Display min positive words
	if (minPositive) {
		maxWordsStream << "Minimum used positive words (" << minPositive->count << " times): ";
		doubleListNode* minWordsPos = minPositiveWords.head;
		while (minWordsPos) {
			maxWordsStream << minWordsPos->word;
			minWordsPos = minWordsPos->next;
			if (minWordsPos) maxWordsStream << ", ";
		}
		maxWordsStream << endl;
	}
	else {
		maxWordsStream << "No minimum positive words found." << endl;
	}

	maxWordsStream << string(50, '-') << endl;

	// Display max negative word
	if (maxNegative) {
		maxWordsStream << "Maximum used negative word: " << maxNegative->word << " (" << maxNegative->count << " times)" << endl;
	}
	else {
		maxWordsStream << "No maximum negative words found." << endl;
	}

	// Display min negative words
	if (minNegative) {
		maxWordsStream << "Minimum used negative words (" << minNegative->count << " times): ";
		doubleListNode* minWordsNeg = minNegativeWords.head;
		while (minWordsNeg) {
			maxWordsStream << minWordsNeg->word;
			minWordsNeg = minWordsNeg->next;
			if (minWordsNeg) maxWordsStream << ", ";
		}
		maxWordsStream << endl;
	}
	else {
		maxWordsStream << "No minimum negative words found." << endl;
	}

	maxWordsStream << string(50, '=') << endl;

	// Log the maximum and minimum words to the console and file
	logToFileAndConsole(outFile, maxWordsStream.str());
}

// Function to display list of words with respective frequency (doubly linked list)
void doublyWordList::displayMatches(doubleListNode* wordNode, ofstream& outFile) {

	// Starting from the head of the list (ascending)
	while (wordNode) {
		if (wordNode->count > 0) {
			string output = "~ " + wordNode->word + ": " + to_string(wordNode->count) + " times\n";
			logToFileAndConsole(outFile, output);
		}
		wordNode = wordNode->next;
	}

	// Optionally, if you want to display from both ends: (descending)
	//cout << "\nDisplaying in reverse order (from tail):" << endl;
	//temp = tail;  // Start from the tail
	//while (temp && temp->count != 0) {
	//	cout << temp->word << " : " << temp->count << endl;
	//	temp = temp->prev;  // Move backward in the DLL
	//}
}
