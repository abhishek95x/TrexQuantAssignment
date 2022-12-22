#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

void writeToFile(unsigned char arr[]);

void addStockToMap(unsigned char arr[]);

string removeTrailingWhiteSpaces(string &str);

int convertToInt(char arr[]);

int getMessageSize(ifstream &is);

string getStockName(unsigned char arr[],int i);

double getStockPrice(unsigned char arr[],int i);

int getStockUnits(unsigned char arr[],int i);

void addOrder(unsigned char arr[]);

string intToStr(int num);

void writeHourlyVWAP();

long long int getTimeStamp(unsigned char arr[]);