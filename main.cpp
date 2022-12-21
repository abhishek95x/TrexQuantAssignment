#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;
//clang++ -std=c++11 main.cpp



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
    return stock;
}

int main(){
    clock_t startTime = clock();
    ifstream inputStream("/Users/abhishek/Downloads/01302019.NASDAQ_ITCH50",ios::in | ios::binary);
    unordered_map <string,vector<float>> stockToVWAP;
    while(inputStream){
        int msgSize = getMessageSize(inputStream);
        unsigned char arr[msgSize];
        inputStream.read((char *)arr,sizeof(arr));
        
        switch (arr[0])
        {
        case int('S'):{
            if(arr[11]==int('C')) return 0; //End of messages
            break;
        }
        case int('R'):{
            //Stock Directory Message.
            string stock = getStockName(arr,11);
            stockToVWAP[stock] = {0,0}; //Numerator,Denominator
            break;
        }
        case int('A'):{
            //Add Order – No MPID Attribution Message.
            break;
        }
        case int('F'):{
            //Add Order – MPID Attribution Message.
            break;
        }
        case int('E'):{
            //Order Executed message for Add Order - No MPID (doesn't contain price)
            break;
        }
        case int('C'):{
            //Order executed with Price message. No MPID
            break;
        }
        case int('X'):{
            //Order Cancelled message. 
            break;
        }
        case int('D'):{
            //Order Delete message. 
            break;
        }
        case int('U'):{
            //Order Replaced message. 
            break;
        }
        case int('P'):{
            //Order Replaced message. 
            break;
        }
        case int('Q'):{
            //Order Replaced message. 
            break;
        }
        case int('B'):{
            //Order Replaced message. 
            break;
        }
        default:
            break;
        }
        
    }
    
    clock_t endTime = clock();
    double runtime = double(endTime-startTime)/CLOCKS_PER_SEC;
    cout<<"Total Runtime : "<<fixed<<runtime<<setprecision(5)<<endl;

    return 0;
}