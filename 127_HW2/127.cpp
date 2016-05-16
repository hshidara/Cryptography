
//  Best runtime: 1.63 seconds
//  Worst runtime: 1.84 seconds
//  1500 iterations

//  *** Compile command:   *** //
/*
    g++ -g -Wall -ansi -std=c++14 127.cpp -o 127.out
 */
//------------------------------------------------------------------------//
//  *** Using the stl map:   *** //
/*
   Set map pair:
     M[make_pair('a',' ')] = 34;
   Access map index:
     cout << M[make_pair('a',' ')] << endl;
 */
//------------------------------------------------------------------------//
//  *** Some useful links   *** //
/*
 https://piazza.com/class/immnhhtwdft34c?cid=30
 http://statweb.stanford.edu/~cgates/PERSI/papers/MCMCRev.pdf
 http://web.cs.ucdavis.edu/~rogaway/classes/127/spring16/p2.pdf
 http://web.cs.ucdavis.edu/~rogaway/classes/127/spring16/
 http://en.cppreference.com/w/cpp/language/ascii
 https://www.google.com/webhp?sourceid=chrome-instant&ion=1&espv=2&ie=UTF-8#q=check+if+key+pair+exists+in+map+c%2B%2B
 http://www.cplusplus.com/reference/map/map/map/
 http://www.cplusplus.com/reference/cmath/log/
 */

//  Importing libraries
#include "stdio.h"
#import <iostream>
#import <string>
#import <algorithm>
#import <map>
#import <fstream>
#import <cstdlib>
#import <iterator>
#include <math.h>
#include <random>
#include <cmath>
#include <time.h>

//  Some Macros
using namespace std;
#define HEADS 1

void read();
void setMapHits(string str);
bool pairExists(char x,char y);
auto setf();
void setMap();
long double pl(map<char,char>func);
void setup();
map<char,char> randomTransposition();
int bernouli (double p );
void printKey();
void path();
void decipher();
void intro();
void randomWalk();

//  Data members, map M is the Matrix M, f is the inverse of the function f, fPrime is f*, (as seen in the prompt).
map<pair<char,char>, long double> M;
map<char,char> f;
map<char,char> fPrime;

string cipherText;

long double length_C = 0.0;

//  Prints out introduction
void intro(){
    cout << endl;
    cout << "****************** Introduction ******************" << endl;
    cout << "Professor Phillip Rogaway, ECS 127, Spring 2016" << endl << endl;
    cout << "A program that decrypts ciphertext that is encrypted using a substitution cipher." << endl;
    cout << "Implements a ciphertext-only attack based on the Diaconis strategy, found here:  http://statweb.stanford.edu/~cgates/PERSI/papers/MCMCRev.pdf" << endl << endl;
    cout << "Hidekazu Shidara, 998883164" << endl;
}

//  Reads in the corpus and cipher text.
void read(){
    string corpus;
    string cipher;
    string str;

    cout << "Please Enter a Corpus text file: ";
    cin >> corpus;
    
    cout << "Please Enter a Cipher text file: ";
    cin >> cipher;
    
    //  Read in corpus line-by-line
    ifstream infile(corpus);
    
    while (getline(infile, str)){
        setMapHits(str);
    }
    
    //  Read in cipher into one string.
    ifstream inf(cipher);
    while (getline(inf, str)){
        cipherText.append(str);
    }
    setMap();
    cout << "Size of M: " << length_C << endl;
}

//  What's happening right now: "My name" => ['M','y'], ['y', ' '], ...
//  Adds up the hits in the corpus text.
void setMapHits(string str){
    int i = 0;
    for (i = 0; i < str.length(); i++) {
        //  increment
        if (!islower(str[i]) && !islower(str[i+1])) {
                M[make_pair(' ', ' ')]++;

        }
        else if(!islower(str[i]) && islower(str[i+1])){
                M[make_pair(' ', str[i+1])]++;

        }
        else if(islower(str[i]) && !islower(str[i+1])){
                M[make_pair(str[i], ' ')]++;
        }
        else{
                M[make_pair(str[i], str[i+1])]++;
        }
        length_C++;
    }
}

//  Takes the # of hits and divides by the total # of bigrams.
void setMap(){
    for(auto iterator = M.begin(); iterator != M.end(); iterator++) {
        iterator->second = (iterator->second)/length_C;
    }
    cout << "Matrix Set" << endl;
}

//  This should return a table that maps the alphabet to the encryption.
//  Inverse of the key.
//  Initially sets the key using a random permutation.
auto setf(){
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    int j = 0;
    
    for (int i = 0 + 97; i < 26 + 97; i++) {
        j = rand() % alphabet.length();
        f[i] = alphabet[j];
        alphabet.erase(alphabet.begin()+j);
    }
    f[' '] = ' ';
    
}


//  Function to call other functions, makes everything more streamlined.
void setup(){
    read();
    setf();
}

//  Tells if the key already exists, if it has already been initialized.
bool pairExists(char x, char y){
    if ( M.find(make_pair(x,y)) == M.end()) {
        // not found
        return false;
    } else {
        // found
        return true;
    }
}

//  A function that prints out f inverse.
void printKey(map<char, char>F){
    for(auto iterator = F.begin(); iterator != F.end(); iterator++) {
        cout << "\""<< iterator-> first << "\"" << " -> " << "\"" << iterator->second << "\"" << endl;
    }
}

//  Function to calculate the plausability.
long double pl(map<char,char>func){
    long double pi = 1;
    for (int i = 0; i < cipherText.length(); i++) {
        if (M[make_pair(func[cipherText[i]],func[cipherText[i+1]])] == 0) {
            pi = pi * (1.0/length_C);
        }
        else{
            pi = pi * (M[make_pair(func[cipherText[i]],func[cipherText[i+1]])]);
        }
    }
    return log(pi);
}

//  Switches 2 random values in f to get f*.
map<char,char> randomTransposition(){
    int i,j;
    fPrime = f;
    i = ((rand()%RAND_MAX)%27) + 97;
    j = i;
    while (j == i) {
        j = ((rand()%RAND_MAX)%27) + 97;
    }
    char temp = ' ';
    temp = fPrime[i];
    fPrime[i] = fPrime[j];
    fPrime[j] = temp;
    return fPrime;
}

//  Random walk implemented with a loop.
void randomWalk(){
    long double plausabilityF = 0.0;
    long double plausabilityFPrime = 0.0;
    
    for (int i = 0; i < 1500; i++) {
        
        fPrime = randomTransposition();
        plausabilityF = pl(f);
        plausabilityFPrime = pl(fPrime);
        if (plausabilityFPrime > plausabilityF) {
            //  f* now becomes f
            f = fPrime;
        }
        else{
            if(bernouli(exp(plausabilityFPrime - plausabilityF)) == HEADS){
                f = fPrime;
            }
        }
    }
    cout << "Plausibility: " << plausabilityF << endl;
}

//  Function given by Professor Rogaway
int bernouli (double p ) {
    double r = fmod(((double)rand()),((double)RAND_MAX));
    return ( r <= p ) ;
}

//  Applies the inverse of key f to the ciphertext.
void decipher(){
    for (int i = 0; i < cipherText.length(); i++) {
        if (!islower(cipherText[i])) {
            cout << cipherText[i];
        }
        else{
            cout << f[cipherText[i]];
        }
    }
    cout << endl;
}

//  Calls setup and then starts the walk.
void path(){
    cout << endl;
    cout << "****************** Statistics ******************" << endl;
    setup();
    randomWalk();
}

//  Main Function
int main(){
    //  Sets the clock to calculate runtime.
    clock_t start, end;
    //  Starts the clock.
    start = clock();
    
    intro();
    cout << endl;
    path();
    cout << endl;
    cout << "****************** Inverse of Key/f() ******************" << endl;
    printKey(f);
    cout << endl;
    cout << "****************** Cipher Text ******************" << endl;
    cout << cipherText << endl << endl;
    cout << "****************** Plain Text ******************" << endl;
    decipher();
    cout << endl;
    
    //  Turns clock off.
    end = clock();
    cout << "Runtime: "
    << (double)(end-start)/CLOCKS_PER_SEC
    << " seconds" << "\n\n";
    
    return 0;
}