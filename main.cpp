#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
// Library effective with Windows
#include <windows.h>

// Library effective with Linux
#include <unistd.h>
struct unigram {
    std::string word;
    int count;
    unigram() : count(0) {}
};

class DataSet {
    std::string* stopWords;
    unigram* uniA;
    int size;
    int stopWordCount;
    unigram top10[10];
    int minTop;

    unsigned int hash(const std::string& word) {
        unsigned int hash = 5381;
        for(char c : word) hash = ((hash << 5) + hash) + c;
        return hash % size;
    }

    void cleanWord(std::string& word) {
        int wr = 0;
        bool hasLetter = false;
        for(int rd=0; rd<word.size(); ++rd) {
            char c = word[rd];
            if(c >= 'A' && c <= 'Z') {
                c += 32;
                hasLetter = true;
            }
            if(c >= 'a' && c <= 'z') hasLetter = true;
            
            if((c >= 'a' && c <= 'z') || c == '\'') {
                word[wr++] = c;
            }
        }
        // Clear word if it contains only numbers
        if(!hasLetter) wr = 0;
        word.resize(wr);
    }

    bool isStopWord(const std::string& word) {
        int left = 0, right = stopWordCount-1;
     
        while(left <= right) {
            int mid = (left+right)/2;
            if(stopWords[mid] == word) return true;
            if(stopWords[mid] < word) left = mid+1;
            else right = mid-1;
        }
        return false;
    }

    void updateTop(const std::string& word, int count) {
        if(count <= minTop || word.empty()) return;

        for(int i=0; i<10; ++i) {
            if(top10[i].word == word) {
                if(count > top10[i].count) {
                    top10[i].count = count;
                    for(int j=i; j>0 && top10[j].count > top10[j-1].count; --j) {
                        std::swap(top10[j], top10[j-1]);
                    }
                }
                return;
            }
        }

        if(count > minTop) {
            top10[9].word = word;
            top10[9].count = count;
            for(int j=9; j>0 && top10[j].count > top10[j-1].count; --j) {
                std::swap(top10[j], top10[j-1]);
            }
            minTop = top10[9].count;
        }
    }

public:
    DataSet() : size(1048573), stopWordCount(0), minTop(0) {
        uniA = new unigram[size];
        stopWords = new std::string[571];
        for(auto& t : top10) t.count = 0;
    }

    ~DataSet() {
        delete[] uniA;
        delete[] stopWords;
    }

    void insert(const std::string& word, int count) {
        if(word.empty() || count == 0) return;
        
        unsigned int index = hash(word);
        int attempts = 0;
        
        while(attempts++ < size) {
            if(uniA[index].count == 0) {
                uniA[index].word = word;
                uniA[index].count = count;
                updateTop(word, count);
                return;
            }
            if(uniA[index].word == word) {
                uniA[index].count += count;
                updateTop(word, uniA[index].count);
                return;
            }
            index = (index+1) % size;
        }
    }

    void readFromFile() {
        std::ifstream iF("PublicationsDataSet.txt");
        std::string line;
        bool inUni = false;

        while(getline(iF, line)) {
            size_t pos = 0;
            while(pos < line.size()) {
                if(!inUni) {
                    pos = line.find("\"unigramCount\"", pos);
                    if(pos == std::string::npos) break;
                    inUni = true;
                    pos += 14;
                    continue;
                }

                size_t wordStart = line.find('"', pos);
                if(wordStart == std::string::npos) break;
                size_t wordEnd = line.find('"', wordStart+1);
                if(wordEnd == std::string::npos) break;

                std::string word = line.substr(wordStart+1, wordEnd-wordStart-1);
                std::string tempWord = word;
                int wordBegin = tempWord[0];
                cleanWord(word);
                
                size_t countStart = line.find(':', wordEnd);
                if(countStart == std::string::npos) break;
                size_t countEnd = line.find_first_of(",}", countStart);
                if(countEnd == std::string::npos) break;

                int count = 0;
                for(size_t i=countStart+1; i<countEnd; ++i) {
                    char c = line[i];
                    if(c >= '0' && c <= '9') count = count*10 + (c-'0');
                }

                pos = countEnd + 1;
                if(line[countEnd] == '}') inUni = false;

                if(!isStopWord(word) && count > 0 && wordBegin != 92) {
                    insert(word, count);
                }
            }
        }
    }

    void getStopWords() {
        std::ifstream sW("stopwords.txt");
        std::string word;
        while(stopWordCount < 571 && getline(sW, word)) {
            cleanWord(word);
            if(!word.empty()) {
                stopWords[stopWordCount++] = word;
            }
        }
        
        for(int i=1; i<stopWordCount; ++i) {
            std::string key = stopWords[i];
            int j = i-1;
            while(j >= 0 && stopWords[j] > key) {
                stopWords[j+1] = stopWords[j];
                j--;
            }
            stopWords[j+1] = key;
        }
    }

    void printTop() {
        for(int i=0; i<10; ++i) {
            for(int j=i+1; j<10; ++j) {
                if(top10[j].count > top10[i].count) {
                    std::swap(top10[i], top10[j]);
                }
            }
        }

        for(int i=0; i<10; ++i) {
            if(top10[i].count > 0) {
                std::cout << top10[i].word << " " << top10[i].count << "\n";
            }
        }
    }
};

int main() {
    DataSet dS;
    clock_t start = clock();
    
    dS.getStopWords();
    dS.readFromFile();
    dS.printTop();

    std::cout << "Time: " << (clock() - start)/1000 << "s\n";
    sleep(10);
    return 0;
}