// Author: Silver Dhuka
// Date: 2021-04-29
// Brief: The program takes a random string as input and outputs the 10 best matches from the dictionary, in order of relevance.
// 
//       For example, the input "potata" would output a list similar to this:
//       potato
//       potatoes
//       potash
//       ...
//       (10 closest matches in order of relevance)
//       ...

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <future>
#include <cmath>
#include "dictionary.h"

using namespace std;

//Initialize pointer to zero so that it can be initialized in first call to getInstance
Dictionary *Dictionary::instance = 0;

// Display top 10 best matches from the dictionary in order of relevance. 
void display_matching_words(string input, const vector<pair<int, int>> &words_size_range) {
    Dictionary *d = d->getInstance ();

    vector<future<vector<pair<string, int>>>> data;
    vector<pair<string, int>> distances;

    // Call a method named get_matching_words asychronously and load the future objects into vector named data
    for (auto it = words_size_range.begin(); it < words_size_range.end(); it += 1) {
        data.push_back(async(Dictionary::get_distances, d, input, it->first, it->second));
    }

    // Merge all vectors from each future object into one vector named distance
    for (auto &dt : data) {
        auto v = dt.get();
        distances.insert(distances.end(), v.begin(), v.end());
    }

    // Do the partial sort the vector named distances to get the first 10 smallest distances which is between two strings means the minimum number of edits
    // needed to transform one string into the other, with the edit operations i.e; insertion, deletion, or substitution of a single character
    int length = min(static_cast<int>(distances.size()), 10);
    partial_sort(distances.begin(), distances.begin() + length, distances.end(), [](pair<string, int> a, pair<string, int> b) { return a.second < b.second; });

    // Display top 10 best matches
    for (auto it = distances.begin(); it < distances.begin() + length; it += 1) {
        cout << it->first << endl;
    }
    cout << endl;

    // Clear all vectors
    data.clear();
    distances.clear();
}

int main(){
    Dictionary *d = d->getInstance ();
    string choice, input;
    bool quitting = false;

    if (d->get_words_size() > 0) {
        int thread_size = 1;
        int words_size = d->get_words_size();
        int remaining_words_size = d->get_words_size();
        int divisor =  words_size >= 8 ? 8 : words_size;

        vector<pair<int, int>> words_size_range;

        // Calculate how many threads we need here and also determine how many words to be passed to each thread for processing
        thread_size = floor(d->get_words_size() / divisor);
        remaining_words_size = words_size - (thread_size * divisor);
        divisor = remaining_words_size > 0 ? divisor - 1 : divisor;
        for (int i = 0; i < divisor; i += 1) {
            int start = i*thread_size;
            int end = (i+1)*thread_size;
            words_size_range.push_back(make_pair(start, end));
        }

        if (remaining_words_size > 0) {
            int start = divisor * thread_size;
            int end = (divisor + 1) * thread_size + remaining_words_size;
            words_size_range.push_back(make_pair(start, end));
        }

        do {
            cout << "Choices:" << endl
                << "1. Any Key For Searching 10 Closest Matches" << endl
                << "2. Q or q Key For Quitting Program" << endl;
            cout << endl;
            cout << "Enter Your Choice: ";
            cin >> choice;
            cout << endl;

            if (choice == "Q" || choice == "q")
                quitting = true;
            else {
                cout << "Enter The Word Which You Want To Search: ";
                cin >> input;
                cout << endl;

                // Display matching words closest to the input
                display_matching_words(input, words_size_range);
            }
        } while (!quitting);
    } else {
        cout << "The dictionary is empty." << endl;
    }

    return 0;
}