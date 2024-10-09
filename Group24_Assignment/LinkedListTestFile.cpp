#include "LinkedList.hpp"

int main() { 
    list reviewList;
    int capacity = 300;  // Define a suitable capacity for the wordList (hash table)
    doublyWordList positiveWords(capacity), negativeWords(capacity);
    doublyWordList analyzer(capacity);

    string line, review, ratingStr;
    int rating;

    string word;

    /*ifstream file("feedback.csv");*/
    ifstream file("tripadvisor_hotel_reviews.csv");
    ifstream pfile("positive-words.txt");
    ifstream nfile("negative-words.txt");

    int index = 0;

    // Reading the feedback.csv file
    while (getline(file, review)) {
        if (review.find("Review") != string::npos) {
            continue; // Skip the header
        }
        else if (review == "") {
            break;
        }
        char ratingChar = review.back(); // Get the last character
        int rating = ratingChar - '0'; // Convert char to int

        // The review is everything before the last character
        review = review.substr(0, review.length() - 1); // Remove the last character
        reviewList.insert(review, rating);

        index++;
    }

    // Load positive words into the linked list
    if (pfile.is_open()) {
        while (getline(pfile, word)) {
#if 0
            positiveWords.insertDoublyNodeAtEnd(word, 0); //Insert Method for Linear and Binary Search
#endif
#if 1
            positiveWords.insertDoublyNodeAtEnd(word, 0, "hash"); // Additional: Hash insert involved
#endif
        }
        pfile.close();
    }
    else {
        cout << "Error opening pfile" << endl;
    }

    // Load negative words into the linked list
    if (nfile.is_open()) {
        while (getline(nfile, word)) {
#if 0
            negativeWords.insertDoublyNodeAtEnd(word, 0); //Insert Method for Linear and Binary Search
#endif
#if 1
            negativeWords.insertDoublyNodeAtEnd(word, 0, "hash"); // Additional: Hash insert involved
#endif
        }
        nfile.close();
    }
    else {
        cout << "Error opening nfile" << endl;
    }

    // Display the status of loaded linked lists
    reviewList.display();
    positiveWords.display("Positive");
    negativeWords.display("Negative");

    int choice, repeat;
        cout << "Do you want to add reviews? (1=yes/0=no)? ";
        cin >> choice;
    
        if (choice == 1) {
            do {
                string reviews;
                int ratings;
    
                // Get the review text
                cout << "Key in the review: ";
                cin.ignore();  // Ignore any newline left in the buffer
                getline(cin, reviews);
    
                // Get the rating with validation
                cout << "Key in the rating (1-5): ";
                while (!(cin >> ratings) || ratings < 1 || ratings > 5) {
                    cin.clear();  // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
                    cout << "Invalid rating. Please enter a number between 1 and 5: ";
                }
    
                int size = reviewList.size + 1;
                reviewList.insert(reviews, ratings);
                // Ask if the user wants to add another review
                cout << "Do you want to add another review? (1=yes/0=no)? ";
                cin >> repeat;
    
            } while (repeat == 1);
        }
     
    auto start = high_resolution_clock::now();
#if 0
    //Binary Search and Insertion Sort
    analyzer.analyzeReviewDoubly(reviewList, positiveWords, negativeWords, "binary", "insertion");
#endif
#if 0
    //Binary Search and Merge Sort
    analyzer.analyzeReviewDoubly(reviewList, positiveWords, negativeWords, "binary", "merge");
#endif
#if 0
    //Linear Search and Insertion Sort
    analyzer.analyzeReviewDoubly(reviewList, positiveWords, negativeWords, "linear", "insertion");
#endif
#if 0
    //Linear Search and Merge Sort
    analyzer.analyzeReviewDoubly(reviewList, positiveWords, negativeWords, "linear", "merge");
#endif
#if 1
    //Additional Feature: Hash Search and Insertion Sort (Self-Created Hash Table)
    analyzer.analyzeReviewDoubly(reviewList, positiveWords, negativeWords, "hash", "insertion");
#endif
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    cout << "The time execution for the entire program: " << duration.count() << " seconds" << endl;

    int generate;
    do {
        cout << endl;
        cout << "Do you like to store the analysis rating for the review in a new csv file (1=yes/0=no)? ";
        cin >> generate;
    } while (generate != 1 && generate != 0);

    if (generate == 1) {
        reviewList.printCSV();
    }

    return 0;
}
