#include "Array.hpp"
#define MAX_FOUND_WORDS 100

string toLowerCase(string str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

string cleanWord(string word) {
	// Remove punctuation and spaces from the word
	word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) {
		return ispunct(c) || isspace(c);
		}), word.end());

	return word;
}

bool linearSearch(string word, WordItem* wordList, int listSize, int& wordIndex) {
	for (int i = 0; i < listSize; i++) {
		if (wordList[i].words == word) {
			wordIndex = i;
			return true;
		}
	}
	wordIndex = -1;
	return false;
}

int binarySearch(WordItem* wordList, int left, int right, const string& word) {
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (wordList[mid].words == word) {
			return mid;
		}
		else if (wordList[mid].words < word) {
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}
	return -1;
}

int exponentialSearch(WordItem* wordList, int listSize, const string& word) {
	if (listSize == 0) {
		return -1;
	}

	if (wordList[0].words == word) {
		return 0;
	}

	int i = 1;
	while (i < listSize && wordList[i].words <= word) {
		i *= 2;
	}

	return binarySearch(wordList, i / 2, min(i, listSize - 1), word);
}

int interpolationSearch(WordItem* wordList, int listSize, const string& word) {
	int low = 0, high = listSize - 1;

	while (low <= high && word >= wordList[low].words && word <= wordList[high].words) {
		if (low == high) {
			if (wordList[low].words == word) return low;
			return -1;
		}

		// Calculate position based on the interpolation formula
		int pos = low + ((high - low) / (wordList[high].words.compare(wordList[low].words) != 0 ?
			(wordList[high].words.compare(wordList[low].words)) : 1)) *
			(word.compare(wordList[low].words));

		// Check if the word is found at pos
		if (wordList[pos].words == word) {
			return pos;
		}

		// If the word is larger, it is in the upper part
		if (wordList[pos].words < word) {
			low = pos + 1;
		}
		// If the word is smaller, it is in the lower part
		else {
			high = pos - 1;
		}
	}

	return -1; // Word not found
}


double calculateSentimentScore(int positiveCount, int negativeCount) {
	int rawSentimentScore = positiveCount - negativeCount;
	int N = positiveCount + negativeCount;

	if (N == 0) return 3.0;

	int minRawScore = -N;
	int maxRawScore = +N;

	double normalizedScore = (rawSentimentScore - minRawScore) / double(maxRawScore - minRawScore);
	double sentimentScore = 1 + (4 * normalizedScore);

	return sentimentScore;
}

void merge(WordItem arr[], int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	// Temporary arrays
	WordItem* L = new WordItem[n1];
	WordItem* R = new WordItem[n2];

	// Copy data to temporary arrays L[] and R[]
	for (int i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (int j = 0; j < n2; j++)
		R[j] = arr[mid + 1 + j];

	int i = 0, j = 0, k = left;

	// Merge the temporary arrays back into arr[left..right]
	while (i < n1 && j < n2) {
		if (L[i].count >= R[j].count) { // Sort in descending order of count
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of L[], if any
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of R[], if any
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}

	delete[] L;
	delete[] R;
}

void mergeSort(WordItem arr[], int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		// Sort first and second halves
		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);

		// Merge the sorted halves
		merge(arr, left, mid, right);
	}
}

void quickSort(WordItem arr[], int left, int right) {
	int i = left, j = right;
	WordItem tmp; // Use WordItem for swapping
	int pivot = arr[(left + right) / 2].count; // Use the 'count' field for comparison

	while (i <= j) {
		// Modify comparison to sort in descending order
		while (arr[i].count > pivot) // Sort descending
			i++;
		while (arr[j].count < pivot) // Sort descending
			j--;
		if (i <= j) {
			// Swap the WordItem objects directly
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

int ArrayList::countFileLines(string filename) {
	ifstream file(filename);
	if (!file) {
		cout << "Error opening " << filename << endl;
		return 0;
	}

	int lines = 0;
	string temp;
	while (getline(file, temp)) {
		lines++;
	}

	file.close();
	return lines;
}

void ArrayList::setWords(int index, string words) {
	wordList[index].words = words;
	wordList[index].count = 0;
}

void ArrayList::setFeedbackWords(int index, string reviews, int ratings) {
	feedbackList[index].reviews = reviews;
	feedbackList[index].ratings = ratings;
}

void ArrayList::readFileData(string fileName, string fileType) {
	arraySize = countFileLines(fileName);

	if (arraySize == 0) {
		cout << "File is empty or could not be read." << endl;
		return;
	}

	if (fileType == "txt") {
		wordList = new WordItem[arraySize];
	}
	else {
		feedbackList = new FeedbackItem[arraySize];
	}

	ifstream file(fileName);
	if (!file) {
		cout << "Error opening " << fileName << endl;
		return;
	}

	int i = 0;
	string line;

	if (fileType == "txt") {
		while (getline(file, line) && i < arraySize) {
			setWords(i, line);
			i++;
		}
	}
	else if (fileType == "csv") {
		while (getline(file, line) && i < arraySize) {
			size_t lastComma = line.find_last_of(',');
			string review, ratingStr;

			if (lastComma != string::npos) {
				review = line.substr(0, lastComma);
				ratingStr = line.substr(lastComma + 1);
			}

			if (review.find("Review") != string::npos) {
				continue;
			}
			else if (review == "") {
				break;
			}

			int rating = atoi(ratingStr.c_str());
			setFeedbackWords(i, review, rating);

			i++;
		}
	}

	file.close();
}

void ArrayList::displayData(string dataType) {
	if (arraySize == 0) {
		cout << "No data to print!" << endl;
		return;
	}
	if (dataType == "positive") {
		for (int i = 0; i < arraySize; i++) {
			cout << "Positive Word [" << i << "]: " << wordList[i].words << " with count of " << wordList[i].count << endl;
		}
	}
	else if (dataType == "negative") {
		for (int i = 0; i < arraySize; i++) {
			cout << "Negative Word [" << i << "]: " << wordList[i].words << " with count of " << wordList[i].count << endl;
		}
	}
	else if (dataType == "feedback") {
		for (int i = 0; i < arraySize; i++) {
			cout << "Review: " << feedbackList[i].reviews << "\n";
			cout << "Rating: " << feedbackList[i].ratings << "\n";
			cout << string(54, '-') << endl;
		}
	}

}

void ArrayList::updateWordCount(string word, ArrayList& wordsList, ArrayList& wordsFound, int& index, int& count, string search) {
	int wordIndex = -1;
	if (search == "linear") {
		// Code for linear search (already handled)
		if (linearSearch(word, wordsList.wordList, wordsList.arraySize, wordIndex)) {
			count++;
			wordsList.wordList[wordIndex].count++;
			// Check if the word is found in wordsFound and update accordingly
			bool wordExists = false;
			for (int i = 0; i < index; i++) {
				if (wordsFound.wordList[i].words == word) {
					wordsFound.wordList[i].count++;
					wordExists = true;
					break;
				}
			}
			if (!wordExists && index < MAX_FOUND_WORDS) {
				wordsFound.wordList[index].words = word;
				wordsFound.wordList[index].count = 1;
				index++;
			}
		}
	}
	else if (search == "exponential") {
		// Code for exponential search (already handled)
		wordIndex = exponentialSearch(wordsList.wordList, wordsList.arraySize, word);
		if (wordIndex != -1) {
			count++;
			wordsList.wordList[wordIndex].count++;
			// Same logic for checking wordsFound
			bool wordExists = false;
			for (int i = 0; i < index; i++) {
				if (wordsFound.wordList[i].words == word) {
					wordsFound.wordList[i].count++;
					wordExists = true;
					break;
				}
			}
			if (!wordExists && index < MAX_FOUND_WORDS) {
				wordsFound.wordList[index].words = word;
				wordsFound.wordList[index].count = 1;
				index++;
			}
		}
	}
	else if (search == "binary") {
		// Call binary search to find the word index
		wordIndex = binarySearch(wordsList.wordList, 0, wordsList.arraySize - 1, word);

		if (wordIndex != -1) {  // If the word is found
			count++;
			wordsList.wordList[wordIndex].count++; 

			// Same logic for updating wordsFound
			bool wordExists = false;
			for (int i = 0; i < index; i++) {
				if (wordsFound.wordList[i].words == word) {
					wordsFound.wordList[i].count++;
					wordExists = true;
					break;
				}
			}
			if (!wordExists && index < MAX_FOUND_WORDS) {
				wordsFound.wordList[index].words = word;
				wordsFound.wordList[index].count = 1;
				index++;
			}
		}
	}
	else if (search == "interpolation") {
		// Code for interpolation search
		wordIndex = interpolationSearch(wordsList.wordList, wordsList.arraySize, word);
		if (wordIndex != -1) {
			count++;
			wordsList.wordList[wordIndex].count++;

			// Same logic for updating wordsFound
			bool wordExists = false;
			for (int i = 0; i < index; i++) {
				if (wordsFound.wordList[i].words == word) {
					wordsFound.wordList[i].count++;
					wordExists = true;
					break;
				}
			}
			if (!wordExists && index < MAX_FOUND_WORDS) {
				wordsFound.wordList[index].words = word;
				wordsFound.wordList[index].count = 1;
				index++;
			}
		}
	}
}

void ArrayList::countSentimentWords(string review, int& positiveCount, int& negativeCount, ArrayList& positiveWordsList, ArrayList& negativeWordsList, ArrayList& positiveWordsFound, ArrayList& negativeWordsFound, int& posWordsIndex, int& negWordsIndex, string search) {
	positiveCount = 0;
	negativeCount = 0;
	posWordsIndex = 0;
	negWordsIndex = 0;

	stringstream ss(review);
	string word;

	while (ss >> word) {
		word = cleanWord(word);
		word = toLowerCase(word);

		updateWordCount(word, positiveWordsList, positiveWordsFound, posWordsIndex, positiveCount, search);
		updateWordCount(word, negativeWordsList, negativeWordsFound, negWordsIndex, negativeCount, search);

	}
}

void ArrayList::getTop10Words(string sort) {
	// Sort the wordList array by word count in descending order
	if (sort == "merge") {
		mergeSort(this->wordList, 0, this->arraySize - 1);
	}
	else if (sort == "quick") {
		quickSort(this->wordList, 0, this->arraySize - 1);
	}

	// Print the top 10 words
	int topN = std::min(this->arraySize, 10); // If there are less than 10 words, adjust the limit
	for (int i = 0; i < topN; i++) {
		cout << wordList[i].words << " : " << wordList[i].count << endl;
	}
}

void ArrayList::getMostFrequentAndLeastFrequentWords() {
	if (wordList == nullptr || arraySize == 0) {
		cout << "No words to process!" << endl;
		return;
	}

	int maxFrequency = 0;
	int minFrequency = INT_MAX;

	for (int i = 0; i < arraySize; i++) {
		if (wordList[i].count > 0) {
			if (wordList[i].count > maxFrequency) {
				maxFrequency = wordList[i].count;
			}
			if (wordList[i].count < minFrequency) {
				minFrequency = wordList[i].count;
			}
		}
	}

	// Step 2: Filter words that have either max or min frequency
	int newSize = 0;
	for (int i = 0; i < arraySize; i++) {
		if (wordList[i].count == maxFrequency || wordList[i].count == minFrequency) {
			// Keep words with max or min frequency
			wordList[newSize] = wordList[i];
			newSize++;
		}
	}
	arraySize = newSize;
}

void ArrayList::displayMaxAndMinUsedWordsCombined(ArrayList& positiveWordsList, ArrayList& negativeWordsList) {
	if (positiveWordsList.wordList == nullptr || negativeWordsList.wordList == nullptr ||
		positiveWordsList.arraySize == 0 || negativeWordsList.arraySize == 0) {
		cout << "No words to process!" << endl;
		return;
	}

	int maxFrequency = 0;
	int minFrequency = INT_MAX;

	// Step 1: Find the max and min frequency from both positive and negative lists
	for (int i = 0; i < positiveWordsList.arraySize; i++) {
		if (positiveWordsList.wordList[i].count > 0) {
			if (positiveWordsList.wordList[i].count > maxFrequency) {
				maxFrequency = positiveWordsList.wordList[i].count;
			}
			if (positiveWordsList.wordList[i].count < minFrequency) {
				minFrequency = positiveWordsList.wordList[i].count;
			}
		}
	}

	for (int i = 0; i < negativeWordsList.arraySize; i++) {
		if (negativeWordsList.wordList[i].count > 0) {
			if (negativeWordsList.wordList[i].count > maxFrequency) {
				maxFrequency = negativeWordsList.wordList[i].count;
			}
			if (negativeWordsList.wordList[i].count < minFrequency) {
				minFrequency = negativeWordsList.wordList[i].count;
			}
		}
	}

	// Step 2: Display words with the max frequency from both lists
	cout << "Maximum used word(s) in the reviews: ";
	bool maxWordsFound = false;

	for (int i = 0; i < positiveWordsList.arraySize; i++) {
		if (positiveWordsList.wordList[i].count == maxFrequency) {
			if (maxWordsFound) cout << ", ";
			cout << positiveWordsList.wordList[i].words;
			maxWordsFound = true;
		}
	}

	for (int i = 0; i < negativeWordsList.arraySize; i++) {
		if (negativeWordsList.wordList[i].count == maxFrequency) {
			if (maxWordsFound) cout << ", ";
			cout << negativeWordsList.wordList[i].words;
			maxWordsFound = true;
		}
	}

	if (!maxWordsFound) {
		cout << "None";
	}
	cout << endl;

	// Step 3: Display words with the min frequency (ignoring 0 count) from both lists
	cout << "Minimum used word(s) in the reviews: ";
	bool minWordsFound = false;

	for (int i = 0; i < positiveWordsList.arraySize; i++) {
		if (positiveWordsList.wordList[i].count == minFrequency && positiveWordsList.wordList[i].count > 0) {
			if (minWordsFound) cout << ", ";
			cout << positiveWordsList.wordList[i].words;
			minWordsFound = true;
		}
	}

	for (int i = 0; i < negativeWordsList.arraySize; i++) {
		if (negativeWordsList.wordList[i].count == minFrequency && negativeWordsList.wordList[i].count > 0) {
			if (minWordsFound) cout << ", ";
			cout << negativeWordsList.wordList[i].words;
			minWordsFound = true;
		}
	}

	if (!minWordsFound) {
		cout << "None";
	}
	cout << endl;
}

void ArrayList::analyzeFeedback(ArrayList& positiveWordsList, ArrayList& negativeWordsList, string search, string sort) {
	if (arraySize == 0) {
		cout << "No reviews to analyze!" << endl;
		return;
	}
	int totalReviews = 0, totalPositiveWords = 0, totalNegativeWords = 0, totalCorrectReview = 0;

	ArrayList positiveWordsFound("Positive Words Found");
	positiveWordsFound.wordList = new WordItem[MAX_FOUND_WORDS];

	ArrayList negativeWordsFound("Negative Words Found");
	negativeWordsFound.wordList = new WordItem[MAX_FOUND_WORDS];

	for (int i = 0; i < arraySize - 1; i++) {
		int positiveCount = 0, negativeCount = 0;
		int posWordsIndex = 0, negWordsIndex = 0;

		countSentimentWords(feedbackList[i].reviews, positiveCount, negativeCount, positiveWordsList, negativeWordsList, positiveWordsFound, negativeWordsFound, posWordsIndex, negWordsIndex, search);

		double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);

		int finalRating = round(sentimentScore);
		string ratingType;
		if (finalRating <= 5 && finalRating > 3) {
			ratingType = "Positive";
		}
		else if (finalRating == 3) {
			ratingType = "Neutral";
		}
		else if (finalRating >= 1 && finalRating < 3) {
			ratingType = "Negative";
		}

		//Display Review
		cout << "Review #" << i + 1 << endl;
		cout << "Review: " << feedbackList[i].reviews << endl << endl;

		//Display positive word found
		cout << "Positive Words = " << positiveCount << ": " << endl;
		for (int j = 0; j < posWordsIndex; j++) {
			cout << "~ " << positiveWordsFound.wordList[j].words << " with frequency of " << positiveWordsFound.wordList[j].count << endl;
		}
		cout << endl;

		// Display negative words found
		cout << "Negative Words = " << negativeCount << ": " << endl;
		for (int j = 0; j < negWordsIndex; j++) {
			cout << "~ " << negativeWordsFound.wordList[j].words << " with frequency of " << negativeWordsFound.wordList[j].count << endl;
		}
		cout << endl;

		cout << fixed << setprecision(2);
		cout << "Sentiment Score (1-5) = " << sentimentScore << ", then the rating should be equal to " << finalRating << " (" << ratingType << ")" << endl;

		cout << "Rating given by the user: " << feedbackList[i].ratings << endl;
		cout << "Analysis output: " << endl;
		if (feedbackList[i].ratings != finalRating) {
			feedbackList[i].ratings = finalRating;
			cout << "User's subjective evaluation does not match the sentiment score provided by the analysis." << endl;
			cout << "There is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment." << endl;
		}
		else {
			totalCorrectReview++;
			cout << "User's subjective evaluation matches the sentiment score provided by the analysis. " << endl;
			cout << "There is a consistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment.  " << endl;
		}
		cout << string(54, '-') << endl;
		cout << endl;

		totalReviews++;
		totalPositiveWords += positiveCount;
		totalNegativeWords += negativeCount;
	}
	cout << "Total Reviews: " << totalReviews << endl;
	cout << "Total Counts of positive words: " << totalPositiveWords << endl;
	cout << "Total Counts of negative words: " << totalNegativeWords << endl;
	cout << endl;
	double totalCorrectPercentage = (double(totalCorrectReview) / totalReviews) * 100;
	cout << "The accurancy of user rating based on the given feedback will be: " << totalCorrectPercentage << "%" << endl;
	cout << endl;
	cout << "Frequency of each word used in overall reviews, listed in ascending order based on frequency: " << endl;
	cout << "Positive Words: " << endl;
	positiveWordsList.getTop10Words(sort);
	cout << endl;
	cout << "Negative Words: " << endl;
	negativeWordsList.getTop10Words(sort);
	cout << endl;

	positiveWordsList.getMostFrequentAndLeastFrequentWords();
	negativeWordsList.getMostFrequentAndLeastFrequentWords();
	displayMaxAndMinUsedWordsCombined(positiveWordsList, negativeWordsList);

	cout << endl;
}

ArrayList::~ArrayList() {
	cout << "Array of " << arrayName << " is now totally removed from the memory!" << endl;
}