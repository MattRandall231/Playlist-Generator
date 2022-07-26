#include <iostream>
#include <unordered_map>
#include <vector>
#include "Song.h"
#include "Sorts.h"
#include <chrono>
#include <fstream>
using namespace std::chrono;
using namespace  std;

void checkInput(string& input) {
    while (input != "0" && input != "1") {
        cout << R"(Invalid Entry. Enter "0" for no or "1" for yes. )";
        cin >> input;
    }
}

void checkRank(string& input) {
    while (input != "1" && input != "2" && input != "3" && input != "4" && input != "5" && input != "6") {
        cout << "Please enter a valid ranking. ";
        cin >> input;
    }
}

vector<string> split(const string &s) {
    vector<string> result;
    int beg = 0;
    int count = 0;
    bool freeze = false;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ',') {
            if (!freeze) {
                result.push_back(s.substr(beg, count));
                beg = i + 1;
                count = 0;
            }
            else {
                count++;
            }
        }
        else if (s[i] == '"') {
            if (!freeze) {
                freeze = true;
                beg = i + 1;
            }
            else {
                freeze = false;
            }
        }
        else {
            count++;
        }
    }
    return result;
}


int main() {
    // Variables used to give scores and create playlist
    bool cont = true;
    string input;
    int length;
    int year;
    bool danceability;
    bool energy;
    bool loudness;
    bool acousticness;
    bool tempo;
    bool exp;
    vector<Song*> songs;
    // Used to store the ranking of each quality
    unordered_map<string, float> rank;

    //todo start of csv parsing
    unsigned int sizeOfCsv = 0;
    vector<string> csvLines;
    vector<string> parsedValues;
    ifstream file1("tracks_features.csv");
    if (!file1.is_open())
    {
        cout << "CSV FILE NOT OPENED" << endl;
    }
    string line1;
    std::getline(file1, line1);
    for (; std::getline(file1, line1); ++sizeOfCsv)
    {
        csvLines.push_back(line1);
    }

    for(auto & csvLine : csvLines)
    {
        Song* songNode = new Song;
        parsedValues = split( csvLine);
        songNode->title = parsedValues[1];
        songNode->artist = parsedValues[4].substr(2,parsedValues[4].size() - 4);
        if( parsedValues[8] == "TRUE")
            songNode->exp = true; // whether a song is explicit or not
        else
            songNode->exp = false;
        songNode->danceability = stof(parsedValues[9]);
        songNode->energy = stof(parsedValues[10]);
        songNode->loudness = stof(parsedValues[12]) ;
        songNode->acousticness = stof(parsedValues[15]);
        songNode->year = stoi(parsedValues[22]);
        songNode->tempo = stof(parsedValues[19]);
        songNode->duration = stoi(parsedValues[20]);
        while (songNode->artist.find("', '") != string::npos)
            songNode->artist.replace(songNode->artist.find("', '"), 4, " and ");
        songs.push_back(songNode);
    }
    //todo end of csv parsing

    cout << endl;
    cout << "Hello! Welcome to the Playlist Generator." << endl;

    // While loop to create different playlists in one run
    while (cont) {
        cout << "How many songs would you like in your playlist? ";
        // Checks if playlist number is a valid input
        bool valid = false;
        while (!valid) {
            cin >> input;
            try {
                length = abs(stoi(input));
                valid = true;
            } catch (invalid_argument &) {
                cout << "Please enter a valid number. ";
            }
        }

        cout << endl;
        cout << "Great! Your playlist will include " << length << " songs. " << endl;
        cout << "Now we will ask you some questions about your playlist: " << endl;

        // Ask questions about song qualities and preferences, also checks input
        cout << endl;
        cout << "Is there any specific year that you would like your songs to be from? " << endl;
        cout << "Enter \"0\" if not. ";
        cin >> input;
        year = stoi(input);

        cout << endl;
        cout << "Would you like your songs to have higher danceability?" << endl;
        cout << R"(Enter "0" for no or "1" for yes. )";
        cin >> input;
        checkInput(input);
        danceability = stoi(input);

        cout << endl;
        cout << "Would you like your songs to have more energy?" << endl;
        cout << R"(Enter "0" for no or "1" for yes. )";
        cin >> input;
        checkInput(input);
        energy = stoi(input);

        cout << endl;
        cout << "Would you like your songs to be loud?" << endl;
        cout << R"(Enter "0" for no or "1" for yes. )";
        cin >> input;
        checkInput(input);
        loudness = stoi(input);

        cout << endl;
        cout << "Would you like your songs to be more acoustic?" << endl;
        cout << R"(Enter "0" for no or "1" for yes. )";
        cin >> input;
        checkInput(input);
        acousticness = stoi(input);

        cout << endl;
        cout << "Would you like your songs to have a higher tempo?" << endl;
        cout << R"(Enter "0" for no or "1" for yes. )";
        cin >> input;
        checkInput(input);
        tempo = stoi(input);

        cout << endl;
        cout << "We will create your playlist using your year, danceability," << endl;
        cout << "energy, loudness, acoustic, and tempo preferences." << endl;
        cout << "You will now rank these qualities from 1 - 6, with" << endl;
        cout << "6 being least important to 1 being most important." << endl;

        // Asks user to rank each quality, checks input
        // Stores each ranking in a map to use for scoring
        cout << endl;
        cout << "Please enter your rank for each quality: " << endl;
        cout << "Year: ";
        cin >> input;
        checkRank(input);
        rank["year"] = (float)7 - (float)stoi(input);
        cout << "Danceability: ";
        cin >> input;
        checkRank(input);
        rank["danceability"] = (float)7 - (float)stoi(input);
        cout << "Energy: ";
        cin >> input;
        checkRank(input);
        rank["energy"] = (float)7 - (float)stoi(input);
        cout << "Loudness: ";
        cin >> input;
        checkRank(input);
        rank["loudness"] = (float)7 - (float)stoi(input);
        cout << "Acousticness: ";
        cin >> input;
        checkRank(input);
        rank["acousticness"] = (float)7 - (float)stoi(input);
        cout << "Tempo: ";
        cin >> input;
        checkRank(input);
        rank["tempo"] = (float)7 - (float)stoi(input);

        // Adds up total points from ranking
        float totalPoints = 0;
        for (auto & iter : rank) {
            totalPoints += iter.second;
        }

        // Each song will get a certain amount of points based on its qualities and user preferences
        for (auto & song : songs) {
            song->score = 0;
            if (year == 0) {
                song->score += rank["year"];
            } else {
                auto yearPoints = (float)abs(2022 - song->year);
                yearPoints = yearPoints / (float)10;
                if (yearPoints > 1)
                    yearPoints = 1;
                yearPoints = 1 - yearPoints;
                song->score += rank["year"] * yearPoints;
            }

            if (danceability)
                song->score += rank["danceability"] * song->danceability;
            else
                song->score += rank["danceability"] * (1 - song->danceability);


            if (energy)
                song->score += rank["energy"] * song->energy;
            else
                song->score += rank["energy"] * (1 - song->energy);

            float loudPoints;
            if (song->loudness >= 0)
                loudPoints = song->loudness + 60;
            else
                loudPoints = 60 - abs(song->loudness);

            loudPoints = loudPoints / (float)67.23;
            if (loudness)
                song->score += rank["loudness"] * loudPoints;
            else
                song->score += rank["loudness"] * (1 - loudPoints);

            if (acousticness)
                song->score += rank["acousticness"] * song->acousticness;
            else
                song->score += rank["acousticness"] * (1 - song->acousticness);

            if (tempo)
                song->score += rank["tempo"] * (song->tempo / 249);
            else
                song->score += rank["tempo"] * (1 - song->tempo / 249);

            // final percentage is caluclated for each song
            song->percentage = song->score / totalPoints;
        }

        cout << endl;
        cout << "Please wait while we sort all songs..." << endl;
        vector<Song *> copy = songs;
        // Time it takes to sort vector with Quicksort
        auto start = high_resolution_clock::now();
        Sorts::quickSort(songs);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        cout << "Quicksort time: " << duration.count() << " nanoseconds" << endl;
        // Time it takes to sort vector with Timsort
        start = high_resolution_clock::now();
        Sorts::timSort(copy);
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        cout << "Timsort time:   " << duration.count() << " nanoseconds" << endl;

        // Asks about inclusion of explicit songs
        cout << endl;
        cout << "Would you like your playlist to include explicit songs?" << endl;
        cout << R"(Enter "0" for no and "1" for yes. )";
        cin >> input;
        exp = stoi(input);

        vector<Song*> playlist;
        // Store all valid songs in 'playlist' vector
        int index = 0;
        for (int j = 0; j < length; j++) {
            if (exp) {
                playlist.push_back(songs[j]);
            } else {
                for (int k = index; k < songs.size(); k++) {
                    if (!songs[k]->exp) {
                        playlist.push_back(songs[k]);
                        index++;
                        break;
                    }
                }
            }
        }

        // Print out all songs in playlist
        cout << endl;
        cout << "Here is your personalized playlist: " << endl;
        for (int p = 0; p < playlist.size(); p++) {
            cout << p + 1 << ". " << playlist[p]->title << " by " << playlist[p]->artist << endl;
        }

        // Asks user for next step: check song data, create new playlist, or exit
        cout << endl;
        bool dataCheck = true;
        while (dataCheck) {
            cout << R"(Enter "0" to exit or "1" to create another playlist.)" << endl;
            cout << "Enter \"data\" to see any song's data. ";
            cin >> input;
            // Checking input
            while (input != "0" && input != "1" && input != "data") {
                cout << R"(Enter "0" to exit or "1" to create another playlist.)" << endl;
                cout << "Enter \"data\" to see any song's data. ";
                cin >> input;
            }
            if (input == "data") {
                // Checks input of song and outputs data for given input
                cout << "Enter the number of the song that you would like to know more about: ";
                int songNum;
                valid = false;
                while (!valid) {
                    cin >> input;
                    try {
                        songNum = abs(stoi(input));
                        valid = true;
                        input = "data";
                    } catch (invalid_argument &) {
                        cout << "Please enter a valid number. ";
                    }
                }
                if (songNum <= playlist.size()) {
                    cout << endl;
                    cout << "Title: " << playlist[songNum - 1]->title << endl;
                    cout << "Artist: " << playlist[songNum - 1]->artist << endl;
                    cout << "Year: " << playlist[songNum - 1]->year << endl;
                    cout << "Explicit: ";
                    if (playlist[songNum - 1]->exp)
                        cout << "Yes" << endl;
                    else
                        cout << "No" << endl;
                    cout << "Danceability: " << playlist[songNum - 1]->danceability << endl;
                    cout << "Energy: " << playlist[songNum - 1]->energy << endl;
                    cout << "Loudness: " << playlist[songNum - 1]->loudness << " dB" << endl;
                    cout << "Acousticness: " << playlist[songNum - 1]->acousticness << endl;
                    cout << "Tempo: " << playlist[songNum - 1]->tempo << " BPM" << endl;
                    cout << "Duration: " << playlist[songNum - 1]->duration << " ms" << endl;
                    cout << "Final Score: " << playlist[songNum - 1]->percentage << endl;
                    cout << endl;
                }

            }

            if (input == "1" || input == "0") {
                dataCheck = false;
            }
            if (input == "0") {
                cont = false;
            }
        }
    }
    return 0;
}


