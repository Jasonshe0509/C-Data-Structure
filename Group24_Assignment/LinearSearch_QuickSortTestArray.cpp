//#include "Array.hpp"
//
//int main() {
//    ArrayList positiveWordsList("Positive Words Array");
//    ArrayList negativeWordsList("Negative Words Array");
//    ArrayList feedbackList("Feedback & Rating Array");
//
//    cout << "Reading positive words.txt" << endl;
//    positiveWordsList.readFileData("positive-words.txt", "txt");
//    cout << "Reading negative words.txt" << endl;
//    negativeWordsList.readFileData("negative-words.txt", "txt");
//    cout << "Reading tripadvisor hotel reviews. csv" << endl;
//    feedbackList.readFileData("tripadvisor_hotel_reviews.csv", "csv");
//
//    int choice, repeat;
//    cout << "Do you want to add reviews? (1=yes/0=no)? ";
//    cin >> choice;
//
//    if (choice == 1) {
//        do {
//            string reviews;
//            int ratings;
//
//            // Get the review text
//            cout << "Key in the review: ";
//            cin.ignore();  // Ignore any newline left in the buffer
//            getline(cin, reviews);
//
//            // Get the rating with validation
//            cout << "Key in the rating (1-5): ";
//            while (!(cin >> ratings) || ratings < 1 || ratings > 5) {
//                cin.clear();  // Clear the error flag
//                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
//                cout << "Invalid rating. Please enter a number between 1 and 5: ";
//            }
//
//            // Assuming increaseArraySize returns the new size, you should subtract 1
//            int size = feedbackList.resizeArrayList();
//            feedbackList.setFeedbackWords(size - 1, reviews, ratings);
//            // Ask if the user wants to add another review
//            cout << "Do you want to add another review? (1=yes/0=no)? ";
//            cin >> repeat;
//
//        } while (repeat == 1);
//    }
//    auto start = high_resolution_clock::now();
//    feedbackList.analyzeFeedback(positiveWordsList, negativeWordsList, "linear", "quick");
//    auto stop = high_resolution_clock::now();
//    auto duration = duration_cast<seconds>(stop - start);
//    cout << "The time execution for the whole program: " << duration.count() << " seconds" << endl;
//
//    int generate;
//    do {
//        cout << endl;
//        cout << "Do you like to store the analysis rating for the review in a new csv file (1=yes/0=no)? ";
//        cin >> generate;
//    } while (generate != 1 && generate != 0);
//    if (generate == 1) {
//        feedbackList.printCSV();
//    }
//    return 0;
//}