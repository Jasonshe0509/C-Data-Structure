//#include "Array.hpp"
//
//int main() {
//    auto start = high_resolution_clock::now();
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
//    feedbackList.analyzeFeedback(positiveWordsList, negativeWordsList, "binary", "quick");
//    auto stop = high_resolution_clock::now();
//    auto duration = duration_cast<seconds>(stop - start);
//    cout << "The time execution for the whole program: " << duration.count() << " seconds" << endl;
//
//    return 0;
//}