// ciphers.cpp
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// RNG initialization
std::mt19937 Random::rng;

// Allowed constant
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// --- Forward declarations ---
void printMenu();
void caesarEncryptCommand();
void caesarDecryptCommand(const vector<string> &dict);
void applyRandSubstCipherCommand();
void computeEnglishnessCommand(const QuadgramScorer &scorer);
void decryptSubstCipherCommand(const QuadgramScorer &scorer);
void decryptSubstCipherFileCommand(const QuadgramScorer &scorer);

// ====================== MAIN ======================
int main() {
  Random::seed(time(NULL));

  // --- Stage 1: Load dictionary ---
  vector<string> dict;
  {
    ifstream fin("dictionary.txt");
    string word;
    while (fin >> word) {
      for (char &c : word) c = toupper(c);
      dict.push_back(word);
    }
  }

  // --- Stage 2: Load quadgram frequencies ---
  vector<string> quadgrams;
  vector<int> counts;
  {
    ifstream fin("english_quadgrams.txt");
    string line;
    while (getline(fin, line)) {
      if (line.empty()) continue;
      size_t comma = line.find(',');
      quadgrams.push_back(line.substr(0, comma));
      counts.push_back(stoi(line.substr(comma + 1)));
    }
  }
  QuadgramScorer scorer(quadgrams, counts);

  // --- Stage 3: Interactive loop ---
  string command;
  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";
    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    } else if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dict);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    }

    cout << endl;
  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

// ====================== MENU ======================
void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// ====================== Caesar Encoder ======================
char rot(char c, int amount) {
  int idx = ALPHABET.find(c);
  if (idx == string::npos) return c;
  int shifted = (idx + amount) % 26;
  if (shifted < 0) shifted += 26;
  return ALPHABET[shifted];
}

string rot(const string &line, int amount) {
  string result;
  for (char c : line) {
    if (isalpha(c)) {
      result.push_back(rot(toupper(c), amount));
    } else if (isspace(c)) {
      result.push_back(c);
    }
  }
  return result;
}

void caesarEncryptCommand() {
  string text, key_str;
  cout << "Enter the text to encrypt:";
  getline(cin, text);
  cout << "Enter the number of characters to rotate by:";
  getline(cin, key_str);
  int key = stoi(key_str);
  cout << rot(text, key) << endl;
}

// ====================== Caesar Decoder ======================
void rot(vector<string> &strings, int amount) {
  for (string &s : strings) s = rot(s, amount);
}

string clean(const string &s) {
  string out;
  for (char c : s)
    if (isalpha(c)) out.push_back(toupper(c));
  return out;
}

vector<string> splitBySpaces(const string &s) {
  stringstream ss(s);
  string word;
  vector<string> words;
  while (ss >> word) words.push_back(word);
  return words;
}

string joinWithSpaces(const vector<string> &words) {
  string out;
  for (size_t i = 0; i < words.size(); i++) {
    if (i) out += " ";
    out += words[i];
  }
  return out;
}

int numWordsIn(const vector<string> &words, const vector<string> &dict) {
  int count = 0;
  for (const string &w : words)
    for (const string &d : dict)
      if (w == d) {
        count++;
        break;
      }
  return count;
}

void caesarDecryptCommand(const vector<string> &dict) {
  string text;
  cout << "Enter the text to decrypt:";
  getline(cin, text);

  vector<string> words = splitBySpaces(text);
  for (string &w : words) w = clean(w);

  bool found = false;
  for (int shift = 0; shift < 26; shift++) {
    vector<string> trial = words;
    rot(trial, shift);
    int matches = numWordsIn(trial, dict);
    if (!trial.empty() && matches * 2 > (int)trial.size()) {
      cout << joinWithSpaces(trial) << endl;
      found = true;
    }
  }
  if (!found) cout << "No good decryptions found" << endl;
}

// ====================== Substitution Encoder ======================
string applySubstCipher(const vector<char> &cipher, const string &s) {
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      result.push_back(cipher[toupper(c) - 'A']);
    } else {
      result.push_back(c);
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  string text;
  cout << "Enter text: ";
  getline(cin, text);
  vector<char> cipher = genRandomSubstCipher();
  cout << applySubstCipher(cipher, text) << endl;
}

// ====================== Substitution Decoder ======================
double scoreString(const QuadgramScorer &scorer, const string &s) {
  string cleaned = clean(s);
  double total = 0;
  for (size_t i = 0; i + 3 < cleaned.size(); i++) {
    total += scorer.getScore(cleaned.substr(i, 4));
  }
  return total;
}

void computeEnglishnessCommand(const QuadgramScorer &scorer) {
  string text;
  cout << "Enter a string for scoring:";
  getline(cin, text);
  cout << scoreString(scorer, text) << endl;
}

vector<char> hillClimb(const QuadgramScorer &scorer, const string &ciphertext) {
  vector<char> best = genRandomSubstCipher();
  string bestPlain = applySubstCipher(best, ciphertext);
  double bestScore = scoreString(scorer, bestPlain);

  int failures = 0;
  while (failures < 1000) {
    vector<char> cand = best;
    int i = Random::randInt(25);
    int j = Random::randInt(25);
    while (j == i) j = Random::randInt(25);
    swap(cand[i], cand[j]);

    string candPlain = applySubstCipher(cand, ciphertext);
    double candScore = scoreString(scorer, candPlain);

    if (candScore > bestScore) {
      best = cand;
      bestScore = candScore;
      failures = 0;
    } else {
      failures++;
    }
  }
  return best;
}

vector<char> decryptSubstCipher(const QuadgramScorer &scorer,
                                const string &ciphertext) {
  vector<char> best;
  double bestScore = -1e9;
  for (int run = 0; run < 25; run++) {
    vector<char> cand = hillClimb(scorer, ciphertext);
    string candPlain = applySubstCipher(cand, ciphertext);
    double candScore = scoreString(scorer, candPlain);
    if (candScore > bestScore) {
      best = cand;
      bestScore = candScore;
    }
  }
  return best;
}

void decryptSubstCipherCommand(const QuadgramScorer &scorer) {
  string text;
  cout << "Enter ciphertext: ";
  getline(cin, text);
  vector<char> key = decryptSubstCipher(scorer, text);
  cout << applySubstCipher(key, text) << endl;
}

void decryptSubstCipherFileCommand(const QuadgramScorer &scorer) {
  string inFile, outFile;
  cout << "Enter input filename: ";
  getline(cin, inFile);

  cout << "Enter output filename: ";
  getline(cin, outFile);

  ifstream fin(inFile);
  if (!fin) {
    cerr << "Error: could not open input file " << inFile << endl;
    return;
  }

  stringstream buffer;
  buffer << fin.rdbuf();
  string ciphertext = buffer.str();

  vector<char> key = decryptSubstCipher(scorer, ciphertext);
  string plain = applySubstCipher(key, ciphertext);

  ofstream fout(outFile);
  if (!fout) {
    cerr << "Error: could not write to " << outFile << endl;
    return;
  }
  fout << plain;
  fout.close();

  cout << "Decryption complete. Output written to " << outFile << endl;
}
