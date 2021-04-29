// Author: Silver Dhuka
// Date: 2021-04-29
// Brief: This is a dictionary class. It implements singleton pattern to load words into memory and find words which matches closest to the supplied input. 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

const char filename[] = "words.txt";

// Dictionary class
class Dictionary {
    static Dictionary *instance;
    vector<std::string> _words;

    // Constructor
    Dictionary() {
        string line, word;
        ifstream MyFile;

        // Open the words.txt file and loads into vector named _words
        MyFile.open(filename, ios::in);
        while (getline(MyFile, line)) {
            istringstream iss(line);
            if (!(iss >> word))
                break;
            _words.push_back(word);
        }
    }

    // Destructor
    ~Dictionary() {}

    // This method makes sure that there is only one instance of Dictionary class
    public:
        static Dictionary *getInstance () {
            if (!instance) {
                instance = new Dictionary();
            }
            return instance;
        }

    // The Levenshtein distance between two strings means the minimum number of edits needed to transform
    // one string into the other, with the edit operations i.e; insertion, deletion, or substitution of a single character
    // Reference: https://www.tutorialspoint.com/cplusplus-program-to-implement-levenshtein-distance-computing-algorithm
    vector<pair<string, int>> get_distances (const string &input, int start, int end) {
        int i, j, l1, l2, t, track;
        int dist[50][50];
        vector<pair<string, int>> distances;

        l2 = input.length();

        for (auto it = _words.begin()+start; it < _words.begin()+end; it += 1) {
            l1 = it->length();
 
            for(i=0; i<=l1; i++) {
                dist[0][i] = i;
            }
   
            for(j=0;j<=l2;j++) {
                dist[j][0] = j;
            }
 
            for (j=1;j<=l1;j++) {
                for(i=1;i<=l2;i++) {
                    if(input.at(i-1) == it->at(j-1)) {
                        track= 0;
                    } else {
                        track = 1;
                    }
                    t = min((dist[i-1][j]+1),(dist[i][j-1]+1));
                    dist[i][j] = min(t,(dist[i-1][j-1]+track));
                }
            }

            distances.push_back(make_pair(*it, dist[l2][l1]));
        }

        return distances;
    }

    // // This method calls the get_distance method and return the vector of pair objects <str, int>
    // vector<pair<string, int>> get_matching_words (string input, int start, int end) {
    //     vector<pair<string, int>> temp;
    //     transform(input.begin(), input.end(), input.begin(), ::tolower);
    //     temp = get_distances(input, start, end);
    //     return temp;        
    // }

    // Return the total size of vector named _words
    int get_words_size () {
        return _words.size();
    }
};