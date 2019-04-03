#include "serialport.h"

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cassert>

using namespace std;

#define MAX_SCORES 100

bool recieveScore = false;
bool sendRanks = false;
bool sendAck = false;

struct Rankings{
  string names;
  int points;
};
Rankings playerRecord[MAX_SCORES];

int counter;

string inputName(){
  cout << "Enter your username(no spaces): ";
  string playerName;
  cin >> playerName;
  return playerName;
}

void writeText(string pName, string pScore){
  ofstream out;
  out.open("scores.txt", ios::app);
  out << pName;
  out << " ";
  out << pScore;
}

void readScores(string filename, unordered_map<int,Rankings>& rank){
  ifstream fin(filename);
  string line;
  counter = 1;
  //if (infile.is_open()){
    while (getline(fin, line)) {
      // since at most we will have 2 parameters
      string p[2];
      int at = 0;
      for (auto c : line) {
        // read in a space sperarated from text file
        if (c == ' ') {
          // start new string
          ++at;
        }
        else {
          // append character to the string we are building
          p[at] += c;
        }
        // don't want to index out of bounds!
        assert(at < 2);
      }
      // first value read from string is name
      playerRecord[counter].names = p[0];
      cout << "name: "<< p[0] << " ";
      // second value read from string is points
      playerRecord[counter].points = stoi(p[1]);
      cout << "points: " << p[1] << endl;
      counter++;
    }
    //infile.close();
  //}
  // else{
  //   cout << "Error opening file"<< endl;
  //   exit (1);
  // }
  cout << "co9unter " << counter << endl;
  //return counter;
}

int serverScore(string playerName, unordered_map<int,Rankings>& rank, int counter){
  SerialPort Serial("/dev/ttyACM0");
  string stats;
  string p[2];
  bool link = false;
  cout << " before reading score" << endl;
  while (link == false) {
    do {
      // timeout for readline increased to 2 seconds since it seemed more consistent
      stats = Serial.readline(10000);
      cout << "this is what is read: " << stats << endl;
    } while (stats.find("Score") == (string::npos)); // because pacman scores always have at least a 0
    cout << "read score: " << endl;
    int at = 0;
    for (auto c: stats){
      if (c == ' '){
        at++;
      }
      else {
        p[at] += c;
      }
      assert(at < 2);
    }
    playerRecord[counter].names = playerName;
    cout << "This was player name: " << playerName;
    playerRecord[counter].points = stoi(p[1]);
    cout << " This was points: " << p[1] << endl;
    cout << "counter: " << counter << endl;
    counter = counter + 1;
    cout << "counter: " << counter << endl;
    writeText(playerName, p[1]);
    link = true;
  }
  recieveScore = true;
  return counter;
}


// swap function similar to eclass quicksort.cpp that swaps two inputs
void swap(Rankings& place,Rankings& place2){
  Rankings temp = place;
  place = place2;
  place2 = temp;
}
/*
// sort the scores
bool sorting(playerRecord lhs, playerRecord rhs){
  return (lhs.points > rhs.points);
}
*/

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (Rankings dist[], int low, int high){
	int pivot = dist[high].points; // pivot
	int i = (low - 1); // Index of smaller element

	for (int j = low; j <= high- 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (dist[j].points >= pivot)
		{
			i++; // increment index of smaller element
			swap(dist[i], dist[j]);
		}
	}
	swap(dist[i + 1], dist[high]);
	return (i + 1);
}

void quickSort(Rankings dist[], int low, int high){
	if (low < high){
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(dist, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(dist, low, pi - 1);
		quickSort(dist, pi + 1, high);
	}
}


void serverCom (unordered_map<int,Rankings>& rank, int counter){
  SerialPort Serial("/dev/ttyACM0");
  string readIn;
  int sentCounter = 0;
  string stats;
  while (sendRanks == false){
    if ((recieveScore == true) && (sendAck == false)){
      cout << "send the N" << endl;
      string countVal = to_string(counter);
      assert(Serial.writeline("N "));
      assert(Serial.writeline(countVal));
      assert(Serial.writeline("\n"));
      sendAck = true;
    } else if ((recieveScore == true) && (sendAck == true)){
      do {
        // timeout for readline increased to 2 seconds since it seemed more consistent
        readIn = Serial.readline(10000);
      } while (readIn.find("A") == (string::npos));
      // is that the correct iterations??
      if (sentCounter < (counter - 1)){
        cout << "couitng the vals and names" << endl;
        string val = to_string(playerRecord[sentCounter].points);
        assert(Serial.writeline("W "));
        assert(Serial.writeline(playerRecord[sentCounter].names));
        cout << "this is name: " << playerRecord[sentCounter].names;
        assert(Serial.writeline(" "));
        assert(Serial.writeline(val));
          cout << "this is val: " << val << endl;
        assert(Serial.writeline("\n"));
        sentCounter++;
      }
      if (sentCounter == (counter-1)){
        assert(Serial.writeline("E"));
        assert(Serial.writeline("\n"));
        sendRanks = true;
        // do {
        //   // timeout for readline increased to 2 seconds since it seemed more consistent
        //   stats = Serial.readline(10000);
        //   cout << "this is what is read: " << stats << endl;
        // } while (stats.find("Score") == (string::npos)); // because pacman scores always have at least a 0
      }
    }
  }
}

int main(){
  SerialPort Serial("/dev/ttyACM0");
  unordered_map<int,Rankings> rank;
  string username = inputName();
  cout << "before read score" << endl;
  readScores("scores.txt",rank);
  cout << "before recieving names and score" << endl;
  counter = serverScore(username , rank, counter);
  cout << "after recieving stuff" << endl;
  //sort(playerRecord, playerRecord + counter, sorting);
  quickSort(playerRecord, 0, counter-1);
  cout << "sorted " << endl;
  cout << "counter: " << counter << endl;
  cout << "before serverCom" << endl;
  serverCom(rank, counter);
  if (counter < MAX_SCORES){
    for (int i = 0; i < (counter-1); i++) {
      cout << (i+1) << ")  " << playerRecord[i].names << " "<< playerRecord[i].points << endl ;
      //scoreboard(playerRecord[i]);
    }
  }
  else {
    for (int i = 0; i < (MAX_SCORES); i++) {
      cout << (i+1) << ")  " << playerRecord[i].names << " "<< playerRecord[i].points << endl ;
      //scoreboard(playerRecord[i]);
    }
  }
  while (true){
  }
  cout << "done" << endl;
  return 0;
}
