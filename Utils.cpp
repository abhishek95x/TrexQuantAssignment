#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Header.h"

using namespace std;


/*============================================================================================================================*/
/*GLOBAL Variabales and Definitions
==============================================================================================================================*/


unordered_map<uint16_t, pair<double,double> > mapStockLocateToVWAP;
unordered_map<uint64_t,pair<uint16_t,pair<double,uint32_t> > > mapReferenceToStockOptions;
unordered_map<uint16_t,string> mapStockLocateToStock; 

uint8_t currentHour = 0;
const string WHITESPACE = " \n\r\t\f\v";
uint64_t HOUR = 3600000000000;
uint64_t curr_hour_timestamp = 0;

/*=============================================================================================================================*/
/*ORDER PARSINGS
===============================================================================================================================*/

/*Add the stock in map*/
void addStock(unsigned char arr[]){
    string stock = getStock(arr,11);
    uint16_t stockLocate = getStockLocate(arr,1);

    if(mapStockLocateToStock.find(stockLocate)==mapStockLocateToStock.end()){
        mapStockLocateToStock.insert({stockLocate,stock});
    }
    if(mapStockLocateToVWAP.find(stockLocate)==mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP.insert({stockLocate,make_pair(0,0)});
    }
}

/*This method is called when a new order is added to books*/
void addOrder(unsigned char arr[]){
    uint16_t stockLocate = getStockLocate(arr,1);
    string stock = getStock(arr,24);
    double price = getPrice(arr,32);
    uint32_t shares = getShares(arr,20);
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);

    if(mapStockLocateToStock.find(stockLocate)==mapStockLocateToStock.end()){
        mapStockLocateToStock.insert({stockLocate,stock});
    }
    if(mapStockLocateToVWAP.find(stockLocate)==mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP.insert({stockLocate,make_pair(0,0)});
    }
    mapReferenceToStockOptions.insert({orderReferenceNumber,make_pair(stockLocate,make_pair(price,shares))});
}

/*This method is called when order on the books is executed*/
void executeOrder(unsigned char arr[]){    
    uint16_t stockLocate = getStockLocate(arr,1);
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);
    uint32_t executedShares = getShares(arr,19);

    if(mapReferenceToStockOptions.find(orderReferenceNumber)!=mapReferenceToStockOptions.end()){
        double price = mapReferenceToStockOptions[orderReferenceNumber].second.first;

        if(mapStockLocateToVWAP.find(stockLocate)!=mapStockLocateToVWAP.end()){
            mapStockLocateToVWAP[stockLocate].first += price * double(executedShares);
            mapStockLocateToVWAP[stockLocate].second += double(executedShares);
        }
        else{
            mapStockLocateToVWAP.insert({stockLocate,make_pair(price * double(executedShares),double(executedShares))});
        }
    }
}

/*This method is used whenever order on the books is executed but there is a change in price*/
void executeOrderWithPrice(unsigned char arr[]){    
    uint16_t stockLocate = getStockLocate(arr,1);
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);
    uint32_t executedShares = getShares(arr,19);
    double price = getPrice(arr,32);

    if(mapStockLocateToVWAP.find(stockLocate)!=mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP[stockLocate].first += price * double(executedShares);
        mapStockLocateToVWAP[stockLocate].second += double(executedShares);
    }
    else{
        mapStockLocateToVWAP.insert({stockLocate,make_pair(price * double(executedShares),double(executedShares))});
    }
    if(mapReferenceToStockOptions.find(orderReferenceNumber)==mapReferenceToStockOptions.end()){
        mapReferenceToStockOptions.insert({orderReferenceNumber,make_pair(stockLocate,make_pair(price,executedShares))});
    }
}

/*This method is called when an order on the books is cancelled*/
void orderCancel(unsigned char arr[]){
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);
    //TODO:: Question : Is cancellation is allowed after execution?
    if(mapReferenceToStockOptions.find(orderReferenceNumber)!=mapReferenceToStockOptions.end()){
        mapReferenceToStockOptions.erase(orderReferenceNumber);
    }

}

/*This message is called when order on the books is replaced*/
void orderReplace(unsigned char arr[]){
    uint16_t stockLocate = getStockLocate(arr,1);
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);
    uint32_t shares = getShares(arr,27);
    double price = getPrice(arr,31);
    uint64_t newOrderReferenceNumber = getOrderReferenceNumber(arr,19);

    if(mapReferenceToStockOptions.find(orderReferenceNumber)!=mapReferenceToStockOptions.end()){
        mapReferenceToStockOptions.erase(orderReferenceNumber);
    }
    if(mapStockLocateToVWAP.find(stockLocate)==mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP.insert({stockLocate,make_pair(0,0)});
    }
    mapReferenceToStockOptions.insert({newOrderReferenceNumber,make_pair(stockLocate,make_pair(price,shares))});
}

/*Execute Non displayable Trade Message*/
void executeTradeMessage(unsigned char arr[]){
    uint16_t stockLocate = getStockLocate(arr,1);
    uint64_t orderReferenceNumber = getOrderReferenceNumber(arr,11);
    uint32_t shares = getShares(arr,20);
    double price = getPrice(arr,32);
    string stock = getStock(arr,24);

    if(mapStockLocateToStock.find(stockLocate)==mapStockLocateToStock.end()){
        mapStockLocateToStock.insert({stockLocate,stock});
    }
    if(mapStockLocateToVWAP.find(stockLocate)!=mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP[stockLocate].first += price * double(shares);
        mapStockLocateToVWAP[stockLocate].second += double(shares);
    }
    else{
        mapStockLocateToVWAP.insert({stockLocate,make_pair(price * double(shares),double(shares))});
    }
    if(mapReferenceToStockOptions.find(orderReferenceNumber)==mapReferenceToStockOptions.end()){
        mapReferenceToStockOptions.insert({orderReferenceNumber,make_pair(stockLocate,make_pair(price,shares))});
    }
}

/*Remove broken trade from VWAP*/
void removeBrokenTrade(unsigned char arr[]){
    uint64_t matchNumber = getOrderReferenceNumber(arr,11);
    uint16_t stockLocate = getStockLocate(arr,1);

    if(mapReferenceToStockOptions.find(matchNumber)!=mapReferenceToStockOptions.end()){
        double price = mapReferenceToStockOptions[matchNumber].second.first;
        uint32_t shares = mapReferenceToStockOptions[matchNumber].second.second;
        if(mapStockLocateToVWAP.find(stockLocate)!=mapStockLocateToVWAP.end()){
            mapStockLocateToVWAP[stockLocate].first -= price * double(shares);
            mapStockLocateToVWAP[stockLocate].second -= double(shares);
        }
        mapReferenceToStockOptions.erase(matchNumber);
    }
}

/*Cross trade*/
void crossTrade(unsigned char arr[]){
    uint64_t matchNumber = getOrderReferenceNumber(arr,31);
    uint16_t stockLocate = getStockLocate(arr,1);
    uint32_t shares = getShares(arr,11);
    double price = getPrice(arr,27);
    string stock = getStock(arr,19);

    if(mapStockLocateToStock.find(stockLocate)==mapStockLocateToStock.end()){
        mapStockLocateToStock.insert({stockLocate,stock});
    }
    if(mapStockLocateToVWAP.find(stockLocate)!=mapStockLocateToVWAP.end()){
        mapStockLocateToVWAP[stockLocate].first += price * double(shares);
        mapStockLocateToVWAP[stockLocate].second += double(shares);
    }
    else{
        mapStockLocateToVWAP.insert({stockLocate,make_pair(price * double(shares),double(shares))});
    }
    if(mapReferenceToStockOptions.find(matchNumber)==mapReferenceToStockOptions.end()){
        mapReferenceToStockOptions.insert({matchNumber,make_pair(stockLocate,make_pair(price,shares))});
    }
    else{
        mapReferenceToStockOptions[matchNumber].second.first = price;
        mapReferenceToStockOptions[matchNumber].second.second = shares;
    }
}


// /*=======================================================================================================================*/
// /*IO OPERATIONS
// =========================================================================================================================*/
void writeToFile(unsigned char arr[]){
    uint64_t timestamp = getTimeStamp(arr);
    if(timestamp > curr_hour_timestamp){
        writeHourlyVWAP();
        curr_hour_timestamp += HOUR;
        currentHour++;
    }
}

void writeHourlyVWAP(){
    ofstream outData;
    string filename = "output/Hour_";
    filename += intToStr(currentHour);
    outData.open(filename);
    if(!outData){
        cerr<<"Error : File could not be opened";
        exit(1);
    }
    auto it = mapStockLocateToVWAP.begin();
    while(it!=mapStockLocateToVWAP.end()){
        if(it->second.first>0){
            double vwap = it->second.first/it->second.second;
            outData<<mapStockLocateToStock[it->first]<<" "<<vwap<<setprecision(5)<<endl;
        }
        it++;
    }
    outData.close();
}



/*=======================================================================================================================*/
/*UTILITIES
=========================================================================================================================*/
uint16_t getStockLocate(unsigned char arr[],int i){
    return uint16_t(arr[i])<<8 | uint16_t(arr[i+1]);
}

string getStock(unsigned char arr[],int i){
    string stock;
    int k = i+8;
    for(;i<k;i++){
        stock.push_back(arr[i]);
    }
    return removeTrailingWhiteSpaces(stock);
}

uint64_t getOrderReferenceNumber(unsigned char arr[],int i){
    uint64_t referenceNumber = uint64_t(arr[i])<<56 |
                                uint64_t(arr[i+1])<<48 |
                                uint64_t(arr[i+2])<<40 |
                                uint64_t(arr[i+3])<<32 |
                                uint64_t(arr[i+4])<<24 |
                                uint64_t(arr[i+5])<<16 |
                                uint64_t(arr[i+6])<<8 |
                                uint64_t(arr[i+7]);
    return referenceNumber;
}

uint16_t getMessageSize(ifstream &is){
    char arr[2];
    is.read(arr,sizeof(arr));
    return convertToInt(arr);
}

double getPrice(unsigned char arr[],int i){
    double price;
    uint32_t num = uint32_t(arr[i])<<24 | 
                uint32_t(arr[i+1])<<16 | 
                uint32_t(arr[i+2])<<8 | 
                uint32_t(arr[i+3]);
    uint32_t decimal = 0;
    for(uint8_t i=0;i<4;i++){
        decimal = decimal*10 + num%10;
        num /=10;
    }
    price = (double)num;
    price += double(decimal)/double(10000);
    return price;
}

uint32_t getShares(unsigned char arr[],int i){
    return uint32_t(arr[i])<<24 | 
            uint32_t(arr[i+1])<<16 | 
            uint32_t(arr[i+2])<<8 | 
            uint32_t(arr[i+3]);
}

uint64_t getTimeStamp(unsigned char arr[]){
    return uint64_t(arr[5])<<40 |
            uint64_t(arr[6])<<32 |
            uint64_t(arr[7])<<24 |
            uint64_t(arr[8])<<16 |
            uint64_t(arr[9])<<8 |
            uint64_t(arr[10]);
}


/*
=================================== OTHER UTILS ============================================================
*/

string intToStr(uint8_t num){
    string ret;
    do{
        ret.insert(ret.begin(),char(num%10 + '0'));
        num/=10;
    }while(num>0);
    return ret;
}

string removeTrailingWhiteSpaces(string &str){
    // size_t start = str.find_first_not_of(WHITESPACE);
    size_t end = str.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

uint16_t convertToInt(char arr[]){
    return (uint16_t)arr[0]<<8 | (uint16_t)arr[1];
}