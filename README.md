# Cipher Project — Caesar & Substitution Ciphers  
### C++ Implementation | Fully Test-Compatible

This project implements **Caesar** and **Substitution** cipher tools in C++, including both **encoding** and **decoding** functionality.  
The README includes explanations, troubleshooting, and best practices based on the official FAQ.

---

##  Features

### 🔸 Caesar Cipher
- Encode using rotation shift
- Decode by generating all 26 rotations
- Safe character handling (uppercase conversion, punctuation preserved)

### 🔸 Substitution Cipher
- Encode using a 26-letter key
- Decode using hill-climbing + English scoring
- Automatic full-file decryption using a *single* key

---

##  Getting Started

### 1️⃣ Build & Run  
You **must use `make`**.  
Do **NOT** use clang commands manually or your IDE run button.

Example:
```
make
make run
make test
```

---

# Common Problems & Fixes

## Caesar Cipher Issues

### Characters turning into numbers
`toupper()` returns an int.  
Use:
```
(char)toupper(c)
```

---

### Decoder freezes  
Caused by mixing getline and >>.  
Use ONLY getline.

---

### Repeated Caesar decryptions  
rot() modifies the vector in-place.  
Use a fresh copy for each rotation.

---

## Substitution Cipher Issues

### applySubstitutionCipher fails tests  
Convert lowercase → uppercase.  
Preserve punctuation.

---

### Hill-climbing stops early  
Stop only after 1000 *non-improving* swaps IN A ROW.

---

### Poor decryption  
English scores are negative; do NOT use bestScore = 0.

---

### Local maximum issue  
Perform 25 hill-climbs.  
Undo swaps that worsen score.

---

## File Decryption Issues

### Bad file output  
Do NOT use:
```
while(!eof())
```

Use:
```
while(getline(infile, line))
```

---

## C++ Function Declarations
If you get “undeclared identifier,” add declarations at the top:
```
void rot(vector<string>&, int);
```

---

## Tips to Pass Every Test
- No extra prints  
- Use helper functions  
- Reset states properly  
- Decrypt full file using one key  
- Use getline for input

---

