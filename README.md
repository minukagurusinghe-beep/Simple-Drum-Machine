# SIT102: Simple Drum Machine

A simple C++ terminal-based drum machine application that allows users to program, play, and manage rhythm patterns. This project demonstrates object-oriented programming concepts and file I/O operations.

## 🚀 Features
- **Interactive Sequencer:** Easily compose, preview, and edit basic drum loops and patterns.
- **Persistent Storage:** Uses C++ I/O streams (`std::ifstream` and `std::ofstream`) to save your custom drum patterns to disk and load them back later.


## 🛠️ Prerequisites
To compile and run this project, you will need:
- A terminal or command prompt environment.

## 📦 Installation & Setup
Follow these steps to download, compile, and run the drum machine on your computer:

```bash
# 1. Clone this repository to your local machine
git clone https://github.com

# 2. Navigate into the project folder
cd SIT102-Drum-Machine

# 3. Compile the C++ source files (replace main.cpp with your file names if needed)
g++ -std=c++11 HD.cpp utilities.cpp -o drum_machine

# 4. Run the executable
./drum_machine
```

## 💾 File I/O Format
When you save a pattern, the application exports a standard text file (e.g., `pattern.txt`). The I/O stream structure saves the data in the following format:
```text
[Kick]  1 0 0 0 1 0 0 0
[Snare] 0 0 1 0 0 0 1 0
[HiHat] 1 1 1 1 1 1 1 1
```

## 👥 Authors
- **Minuka Gurusinghe** - *Student ID: 226076188* - Deakin University
