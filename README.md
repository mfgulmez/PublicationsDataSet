# Publications Data Set Word Frequency Analyzer
A C++ application that performs unigram analysis on a corpus of text data. It counts word frequencies while filtering out common stop words, using an efficient hashing mechanism to store and retrieve words.

## Overview
This project provides a lightweight text analysis tool intended for research or educational purposes, particularly in processing academic publication datasets. It reads input files, filters out stop words, and outputs frequency counts of meaningful words. This tool can help in understanding linguistic patterns or generating features for further natural language processing tasks.

## Project Structure
```plaintext
PublicationsDataSet/
├── CMakeLists.txt              # Build configuration
├── main.cpp                    # Main application logic
├── main.exe                    # Windows executable
├── README.md                   # Project documentation
└── .git/                       # Git version control metadata
```

## Features
- Efficient word counting using custom hash indexing.
- Automatic stop word filtering (571 common words).
- Designed for scalability with large text datasets.

## Usage
- Exract PublicationsDataSet.zip to same place and run main.cpp
- Results from the execution:<br />
![image](https://github.com/user-attachments/assets/134bf333-bb61-46b0-9ffe-11497af9e377)
