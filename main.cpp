#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;
struct unigram{
    string word;
    int count;
    unigram():count(0){}
};
class DataSet{
    string* stopWords;
    unigram* uniA;
    int size;
public:

    DataSet():size(10000000){
        uniA=new unigram[size];
        stopWords=new string[571];
    }
    void checkStopWord(){
        for(int i=0;i<size;i++){
            if(uniA[i].count!=0)
                for(int j=0;j<571;j++){
                    if(uniA[i].word==stopWords[j]){
                        uniA[i].count=0;
                    }}
        }
    }
    void insert(string &word,int count){


        if(word.size()!=1){
            int mid=((word.size()))/2;
            int index=word[0]*word[word.size()-1]*word[mid]*word.size();
            index%=size;
            bool sameWord=false;
            while(uniA[index].count!=0&&index<size){
                if(uniA[index].word==word){
                    uniA[index].count+=count;
                    sameWord=true;
                    break;
                }
                index++;
                if(index>=size){
                    index%=size;
                }

            }
            if(!sameWord){
                uniA[index].word=word;
                uniA[index].count=count;
            }
        }
    }

    void sort(){
        int max,preMax=100000,maxIndex;
        for(int j=0;j<10;j++) {
            preMax=max;
            max=0;
            for (int i = 0; i < size; i++) {
                if (uniA[i].count != 0) {
                    if (max < uniA[i].count&&uniA[i].count<preMax) {
                        max = uniA[i].count;
                        maxIndex = i;
                    }
                }
            }
            cout << j+1<<"."<<uniA[maxIndex].word << " " << uniA[maxIndex].count << endl;
        }
    }
    void readFromFile(){
        ifstream iF;
        bool inUni= false;
        iF.open("PublicationsDataSet.txt");
        string uniWord,uniCount;
        while (!iF.eof()){
            getline(iF,uniWord,'"');
            getline(iF,uniWord,'"');
            if(inUni){
                getline(iF,uniCount,':');
                getline(iF,uniCount,',');
                int sum=0;
                int y=0;
                while(y<uniWord.size()) {
                    if ((uniWord[y] >= 'a' && uniWord[y] <= 'z') || (uniWord[y] >= 'A' && uniWord[y] <= 'Z') ||
                        uniWord[y] == 39) {
                        if (uniWord[y] >= 'A' && uniWord[y] < 'Z') {
                            uniWord[y] += 32;
                        }
                        y++;
                    } else {
                        uniWord.erase(uniWord.begin()+y);
                    }
                }
                y=0;
                while(y<uniCount.size()){
                    if(uniCount[y]<'0'||uniCount[y]>'9'){
                        if(uniCount[y]=='}'){
                            if(uniCount[y+1]=='}'){
                                inUni=false;
                                uniCount.erase(uniCount.begin()+y+1,uniCount.end());
                            }
                        }
                        uniCount.erase(uniCount.begin()+y);
                    }
                    else{y++;}
                }
                y=0;
                while(y<uniCount.size()){
                    uniCount[y]-=48;
                    sum+=uniCount[y]*pow(10,uniCount.size()-y-1);
                    y++;
                }
                if(!uniWord.empty()){
                    insert(uniWord,sum);
                }
            }
            if(uniWord=="unigramCount"){
                inUni=true;
            }
        }
    }
    void print(){
        for(int i=0;i<size;i++){
            if(uniA[i].count!=0){
                cout<<uniA[i].word<<" "<<uniA[i].count<<endl;
            }
        }
    }
    void getStopWords(){
        ifstream sW;
        string stopwords;
        int count=0;
        sW.open("stopwords.txt");
        while(!sW.eof()){
            getline(sW,stopwords);
            stopWords[count]=stopwords;
            count++;
        }
    }
};

int main() {
    DataSet dS;
    time_t time=clock();
    dS.readFromFile();
    dS.getStopWords();
    dS.checkStopWord();
    dS.sort();
    cout<<"time:"<<(clock()-time)/1000<<" seconds"<<endl;
    return 0;
}