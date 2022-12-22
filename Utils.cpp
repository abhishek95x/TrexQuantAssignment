#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Header.h"

using namespace std;

unordered_map <string,vector<double> > mapStockToVWAP;
int currentHour = 0;
const string WHITESPACE = " \n\r\t\f\v";
const long long int HOUR = 3600000000000;
long long int curr_hour_timestamp = 0;

void addStockToMap(unsigned char arr[]){
    string stock = getStockName(arr,11);
    if(mapStockToVWAP.find(stock)==mapStockToVWAP.end()){
        mapStockToVWAP[stock] = {0,0};
    }
}

void writeToFile(unsigned char arr[]){
    long long int timestamp = getTimeStamp(arr);
    if(timestamp > curr_hour_timestamp){
        writeHourlyVWAP();
        curr_hour_timestamp += HOUR;
        currentHour++;
    }
}

string removeTrailingWhiteSpaces(string &str){
    // size_t start = str.find_first_not_of(WHITESPACE);
    size_t end = str.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

int convertToInt(char arr[]){
    return (int)arr[0]<<8 | (int)arr[1];
}

int getMessageSize(ifstream &is){
    char arr[2];
    is.read(arr,sizeof(arr));
    return convertToInt(arr);
}

string getStockName(unsigned char arr[],int i){
    string stock;
    int k = i+8;
    for(;i<k;i++){
        stock.push_back(arr[i]);
    }
    return removeTrailingWhiteSpaces(stock);
}

double getStockPrice(unsigned char arr[],int i){
    double price;
    
    //unsigned char bytes[] = {arr[i], arr[i+1], arr[i+2], arr[i+3]};
    // memcpy(&price, &bytes, sizeof(price));
    int num = int(arr[i])<<24 | 
                int(arr[i+1])<<16 | 
                int(arr[i+2])<<8 | 
                int(arr[i+3]);

    int decimal = 0;
    for(int i=0;i<4;i++){
        decimal = decimal*10 + num%10;
        num /=10;
    }
    price = num;
    price += double(decimal)/double(10000);
    // cout<<price<<setprecision(5)<<endl;
    return price;
}

int getStockUnits(unsigned char arr[],int i){
    int num = int(arr[i])<<24 | 
                int(arr[i+1])<<16 | 
                int(arr[i+2])<<8 | 
                int(arr[i+3]);
    return num;
}

void addOrder(unsigned char arr[]){
    string stockName = getStockName(arr,24);
    double price = getStockPrice(arr,32);
    int units = getStockUnits(arr,20);

    if(mapStockToVWAP.find(stockName)==mapStockToVWAP.end()){
        return;
    }
    mapStockToVWAP[stockName][0] += price * double(units);
    mapStockToVWAP[stockName][1] += double(units);
}

string intToStr(int num){
    string ret;
    do{
        ret.insert(ret.begin(),char(num%10 + '0'));
        num/=10;
    }while(num>0);
    return ret;
}

void writeHourlyVWAP(){
    ofstream outData;
    string filename = "/Users/abhishek/Downloads/output/Hour_";
    filename += intToStr(currentHour);
    outData.open(filename);
    if(!outData){
        cerr<<"Error : File could not be opened";
        exit(1);
    }
    auto it = mapStockToVWAP.begin();
    while(it!=mapStockToVWAP.end()){
        if(it->second[0]>0){
            double vwap = it->second[0]/it->second[1];
            outData<<it->first<<" "<<(int)vwap<<endl;
        }
        it++;
    }
    outData.close();
}

long long int getTimeStamp(unsigned char arr[]){
    unsigned char b5 = arr[5];
    unsigned char b4 = arr[6];
    unsigned char b3 = arr[7];
    unsigned char b2 = arr[8];
    unsigned char b1 = arr[9];
    unsigned char b0 = arr[10];

    return (long long int)(b5)<<40 |
            (long long int)(b4)<<32 |
            (long long int)(b3)<<24 |
            (long long int)(b2)<<16 |
            (long long int)(b1)<<8 |
            (long long int)(b0);
}