#include "Array.hpp"
#define MAX_FOUND_WORDS 1000

string toLowerCase(string str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

string cleanWord(string word) {
	// Use std::remove_if to remove non-ASCII characters and punctuation except for '.', '+', '*', "/"
	word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) {
		// Remove non-ASCII characters (outside 32 to 126) and unwanted punctuation
		return (c < 32 || c > 126) || (ispunct(c) && c != '.' && c != '/' && c != '+' && c != '*');
		}), word.end());

	// Replace all periods ('.') with spaces
	replace(word.begin(), word.end(), '.', ' ');
	replace(word.begin(), word.end(), '/', ' ');

	return word;
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

void logToFileAndConsole(ofstream& outFile, const string& message) {
	cout << message;
	outFile << message;
}

int ArrayList::countFileLines(string filename, string fileType) {
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
	if (fileType == "txt") {
		return lines;
	}
	else {
		return lines - 1;
	}
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
	arraySize = countFileLines(fileName, fileType);

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

			int rating = stoi(ratingStr);
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

int ArrayList::resizeArrayList() {
	int newArraySize = arraySize + 1;  // Increase size by 1
	FeedbackItem* newfeedbackList = new FeedbackItem[newArraySize];

	// Copy old data to the new array
	for (int i = 0; i < arraySize; i++) {
		newfeedbackList[i] = feedbackList[i];
	}

	newfeedbackList[newArraySize - 1].reviews = "";
	newfeedbackList[newArraySize - 1].ratings = 0;

	// Delete the old array and assign the new one
	delete[] feedbackList;
	feedbackList = newfeedbackList;
	arraySize = newArraySize;  // Update the size

	return arraySize;  // Return the new size
}

bool ArrayList::linearSearch(const string& word, int& wordIndex) {
	for (int i = 0; i < arraySize; i++) {
		if (wordList[i].words == word) {
			wordIndex = i;
			return true;
		}
	}
	wordIndex = -1;
	return false;
}

int ArrayList::binarySearch(int left, int right, const string& word) {
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

int ArrayList::exponentialSearch(const string& word) {
	if (arraySize == 0) {
		return -1;
	}

	if (wordList[0].words == word) {
		return 0;
	}

	int i = 1;
	while (i < arraySize && wordList[i].words <= word) {
		i *= 2;
	}

	return binarySearch(i / 2, min(i, arraySize - 1), word); 
}

void ArrayList::updateWordCount(string word, ArrayList& wordsList, ArrayList& wordsFound, int& index, int& count, string search) {
	int wordIndex = -1;
	if (search == "linear") {
		// Code for linear search (already handled)
		if (wordsList.linearSearch(word, wordIndex)) {
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
		wordIndex = wordsList.exponentialSearch(word);
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
		wordIndex = wordsList.binarySearch( 0, wordsList.arraySize - 1, word);

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

		stringstream wordStream(word);
		string splitWord;

		// Process each part of the word after punctuation is replaced
		while (wordStream >> splitWord) {
			updateWordCount(splitWord, positiveWordsList, positiveWordsFound, posWordsIndex, positiveCount, search);
			updateWordCount(splitWord, negativeWordsList, negativeWordsFound, negWordsIndex, negativeCount, search);
		}
	}
}

void ArrayList::merge(int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	// Create temporary arrays for left and right subarrays
	WordItem* L = new WordItem[n1];
	WordItem* R = new WordItem[n2];

	// Copy data to temporary arrays
	for (int i = 0; i < n1; i++)
		L[i] = wordList[left + i];
	for (int j = 0; j < n2; j++)
		R[j] = wordList[mid + 1 + j];

	// Merge the temporary arrays back into wordList
	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2) {
		if (L[i].count >= R[j].count) {
			wordList[k] = L[i];
			i++;
		}
		else {
			wordList[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of L[], if any
	while (i < n1) {
		wordList[k] = L[i];
		i++;
		k++;
	}

	// Copy the remaining elements of R[], if any
	while (j < n2) {
		wordList[k] = R[j];
		j++;
		k++;
	}

	// Clean up temporary arrays
	delete[] L;
	delete[] R;
}

void ArrayList::mergeSort(int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;

		// Sort first and second halves
		mergeSort(left, mid);
		mergeSort(mid + 1, right);

		// Merge the sorted halves
		merge(left, mid, right);
	}
}

void ArrayList::quickSort(int left, int right) {
	int i = left, j = right;
	WordItem tmp;  // Use WordItem for swapping
	int pivot = wordList[(left + right) / 2].count;  // Use the 'count' field for comparison

	while (i <= j) {
		// Modify comparison to sort in descending order
		while (wordList[i].count > pivot)  // Sort descending
			i++;
		while (wordList[j].count < pivot)  // Sort descending
			j--;
		if (i <= j) {
			// Swap the WordItem objects directly
			tmp = wordList[i];
			wordList[i] = wordList[j];
			wordList[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j)
		quickSort(left, j);
	if (i < right)
		quickSort(i, right);
}

void ArrayList::getSortedWords(string sort, ofstream& outFile) {
	// Sort the wordList array by word count in descending order
	if (sort == "merge") {
		mergeSort(0, arraySize - 1);
	}
	else if (sort == "quick") {
		quickSort(0, arraySize - 1);
	}

	for (int i = 0; i < arraySize; i++) {
		if (wordList[i].count > 0) {
			string output = "~ " + wordList[i].words + ": " + to_string(wordList[i].count) + " times\n";
			logToFileAndConsole(outFile, output);
		}
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

void ArrayList::displayMaxAndMinUsedWordsCombined(ArrayList& positiveWordsList, ArrayList& negativeWordsList, ofstream& outFile) {
	if (positiveWordsList.wordList == nullptr || negativeWordsList.wordList == nullptr ||
		positiveWordsList.arraySize == 0 || negativeWordsList.arraySize == 0) {
		logToFileAndConsole(outFile, "No words to process!\n");
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
	stringstream maxWordsStream;
	maxWordsStream << "Maximum used word(s) in the reviews(" << maxFrequency << " times) :" ;
	bool maxWordsFound = false;

	for (int i = 0; i < positiveWordsList.arraySize; i++) {
		if (positiveWordsList.wordList[i].count == maxFrequency) {
			if (maxWordsFound) maxWordsStream << ", ";
			maxWordsStream << positiveWordsList.wordList[i].words;
			maxWordsFound = true;
		}
	}

	for (int i = 0; i < negativeWordsList.arraySize; i++) {
		if (negativeWordsList.wordList[i].count == maxFrequency) {
			if (maxWordsFound) maxWordsStream << ", ";
			maxWordsStream << negativeWordsList.wordList[i].words;
			maxWordsFound = true;
		}
	}

	if (!maxWordsFound) {
		maxWordsStream << "None";
	}
	maxWordsStream << endl;

	// Log maximum words to console and file
	logToFileAndConsole(outFile, maxWordsStream.str());

	// Step 3: Display words with the min frequency (ignoring 0 count) from both lists
	stringstream minWordsStream;
	minWordsStream << "Minimum used word(s) in the reviews(" << minFrequency << " times) :" << endl;
	bool minWordsFound = false;

	for (int i = 0; i < positiveWordsList.arraySize; i++) {
		if (positiveWordsList.wordList[i].count == minFrequency && positiveWordsList.wordList[i].count > 0) {
			if (minWordsFound) minWordsStream << ", ";
			minWordsStream << positiveWordsList.wordList[i].words;
			minWordsFound = true;
		}
	}

	for (int i = 0; i < negativeWordsList.arraySize; i++) {
		if (negativeWordsList.wordList[i].count == minFrequency && negativeWordsList.wordList[i].count > 0) {
			if (minWordsFound) minWordsStream << ", ";
			minWordsStream << negativeWordsList.wordList[i].words;
			minWordsFound = true;
		}
	}

	if (!minWordsFound) {
		minWordsStream << "None";
	}
	minWordsStream << endl;

	// Log minimum words to console and file
	logToFileAndConsole(outFile, minWordsStream.str());
}

void ArrayList::analyzeFeedback(ArrayList& positiveWordsList, ArrayList& negativeWordsList, string search, string sort) {
	if (arraySize == 0) {
		cout << "No reviews to analyze!" << endl;
		return;
	}
	time_t t = time(nullptr);
	tm tm;
	localtime_s(&tm, &t);

	ostringstream oss;
	oss << put_time(&tm, "%Y-%m-%d_%H-%M");
	string timestamp = oss.str();
	string filename = "Review_Analysis_Report " + timestamp + ".txt";

	ofstream outFile(filename);

	int totalReviews = 0, totalPositiveWords = 0, totalNegativeWords = 0, totalCorrectReview = 0;

	ArrayList positiveWordsFound("Positive Words Found");
	positiveWordsFound.wordList = new WordItem[MAX_FOUND_WORDS];

	ArrayList negativeWordsFound("Negative Words Found");
	negativeWordsFound.wordList = new WordItem[MAX_FOUND_WORDS];

	for (int i = 0; i < arraySize; i++) {
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
	logToFileAndConsole(outFile, "Total Reviews: " + to_string(totalReviews) + "\n");
	logToFileAndConsole(outFile, "Total Counts of positive words: " + to_string(totalPositiveWords) + "\n");
	logToFileAndConsole(outFile, "Total Counts of negative words: " + to_string(totalNegativeWords) + "\n");
	logToFileAndConsole(outFile, "\n");

	double totalCorrectPercentage = (double(totalCorrectReview) / totalReviews) * 100;
	logToFileAndConsole(outFile, "The accuracy of user rating based on the given feedback will be: " + to_string(totalCorrectPercentage) + "%\n");
	logToFileAndConsole(outFile, "\n");
	logToFileAndConsole(outFile, "Frequency of each word used in overall reviews, listed in ascending order based on frequency: \n");
	logToFileAndConsole(outFile, "Positive Words: \n");
	positiveWordsList.getSortedWords(sort,outFile);
	logToFileAndConsole(outFile, "\n");
	logToFileAndConsole(outFile, "Negative Words: \n");
	negativeWordsList.getSortedWords(sort,outFile);
	logToFileAndConsole(outFile, "\n");

	positiveWordsList.getMostFrequentAndLeastFrequentWords();
	negativeWordsList.getMostFrequentAndLeastFrequentWords();
	displayMaxAndMinUsedWordsCombined(positiveWordsList, negativeWordsList, outFile);

	logToFileAndConsole(outFile, "\n");

	outFile.close();
}

void ArrayList::printCSV() {
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

	//write headers of csv file
	outFile << "Review,Analysis Rating" << endl;

	for (int i = 0; i < arraySize - 1; i++) {
		outFile << feedbackList[i].reviews << ","
			<< feedbackList[i].ratings << endl;
	}

	outFile.close();
	cout << "Reviews have been written to: " << filename << endl;
}

ArrayList::~ArrayList() {
	cout << "Array of " << arrayName << " is now totally removed from the memory!" << endl;
}