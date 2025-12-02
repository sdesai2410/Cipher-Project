# Cipher-Project
This project implements both Caesar Cipher (encoding/decoding) and Substitution Cipher (encoding/decoding) tools in C++.
The goal is to build correct, efficient, and test-compliant cryptographic functions following the project guide.

🚀 Project Overview

This repository contains implementations for:

1. Caesar Cipher

Encode text using Caesar shift

Decode text by testing all rotations

Full command parsing using getline

Proper character handling with toupper + casting

2. Substitution Cipher

Encode by applying a 26-letter key

Decode using a hill-climbing algorithm with English scoring

Resetting & swapping logic to avoid incorrect convergence

Supports decrypting entire files encoded with one key

🛠️ Build & Run Instructions
Use make — Do NOT run code manually

The project requires using Make commands instead of running via IDE buttons or typing clang++ manually.
Tests and runners depend on the provided Makefile.

⚠️ Common Issues & Fixes

Below is a consolidated list of frequently asked questions from the FAQ:

🗡️ Caesar Cipher — Known Issues
❗ “utils.h not found”

Always run the project using make, not the Run button or manual compile.
This resolves include-path issues.

❗ Strings turn into numbers

Because toupper, tolower, etc. return int, inserting them directly into a stream prints ASCII numbers.

Fix:

ss << (char)toupper(c);

❗ Program hangs / infinite loop

Caused by mixing >> and getline.
Use only getline for all console input.

❗ Decoder gives repeated or unordered outputs

The rot function modifies the vector passed by reference.

Calling rot(vec,1) then rot(vec,2) actually rotates by 3.
Use a fresh copy for each test.

❗ Output looks correct, but tests fail

Common mistakes:

Not using putTogetherWithSpaces

Not printing each decrypted line separately

Extra console output beyond expected values

🔐 Substitution Cipher — Known Issues
❗ applySubstitutionCipher fails tests but works manually

Tests expect:

Conversion of lowercase → uppercase

Non-alphabet characters preserved

Correct handling of "Mr. Jock, TV quiz PhD, bags few lynx."

❗ Hill-climbing stops too early

The rule “1000 trials in a row without improvement” is often misread.
It does not mean “1000 total trials.”

❗ Decryption is too fast and inaccurate

Initial “best score” must NOT be zero, because all Englishness scores are negative.

❗ Gets stuck at a local max

You must:

Run hill-climbing 25 separate times

Reset swaps correctly

Compare each trial to the current iteration’s best score, NOT previous runs

❗ Code is extremely slow or hangs

Likely calling:

applySubstitutionCipher

getScore

clean
too often in loops.

Minimize repeated computation.

📄 File Decryption Issues
❗ Output file looks right but tests fail

Most common causes:

Missing newline at end of a line

Extra newline at end of file

Using while(!infile.eof()) — never do this

Use:

while (getline(infile, line)) {
    ...
}

❗ File decode wrong even though console version works

You must decrypt the entire file using a single key, not per line.
Each line decrypted with its own key produces gibberish.

🧩 C++ Function Declarations

If you get:

use of undeclared identifier


You must declare helper functions at the top of ciphers.cpp:

void rot(vector<string>&, int);


Real C++ projects use header files, but this assignment requires everything in a single .cpp.

📌 Tips for Passing All Tests

Follow input/output formats exactly

Avoid extra prints

Use helper functions you wrote

Reset state fully between decoder iterations

Verify character handling carefully

Recreate plaintext.txt using your passing decryption
