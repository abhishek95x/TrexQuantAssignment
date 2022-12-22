#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;



/*ORDERS*/

void addStock(unsigned char arr[]);

void addOrder(unsigned char arr[]);

void executeOrder(unsigned char arr[]);

void executeOrderWithPrice(unsigned char arr[]);

void orderCancel(unsigned char arr[]);

void orderReplace(unsigned char arr[]);

void executeTradeMessage(unsigned char arr[]);

void removeBrokenTrade(unsigned char arr[]);

void crossTrade(unsigned char arr[]);




/*IO Operations*/
void writeHourlyVWAP();

void writeToFile(unsigned char arr[]);




/*UTILITIES PROCESSING*/

uint16_t getMessageSize(ifstream &is);

string getStock(unsigned char arr[],int i);

double getPrice(unsigned char arr[],int i);

uint32_t getShares(unsigned char arr[],int i);

uint16_t getStockLocate(unsigned char arr[],int i);

uint64_t getTimeStamp(unsigned char arr[]);

uint64_t getOrderReferenceNumber(unsigned char arr[],int i);


/*UTILITIES OTHERS*/

string intToStr(uint8_t num);

uint16_t convertToInt(char arr[]);

string removeTrailingWhiteSpaces(string &str);


