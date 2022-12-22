#include "Header.h"

using namespace std;


int main(){
    clock_t startTime = clock();
    ifstream inputStream("/Users/abhishek/Downloads/01302019.NASDAQ_ITCH50",ios::in | ios::binary);
    
    while(inputStream){
        int msgSize = getMessageSize(inputStream);
        unsigned char arr[msgSize];
        inputStream.read((char *)arr,sizeof(arr));
        
        //Check if We have entered new hour.
        writeToFile(arr);
        
        //Processing the messages based on their msg type.
        switch (arr[0])
        {
        case int('S'):{
            if(arr[11]==int('C')){
                //Directly write to last hour.
                writeHourlyVWAP();
                clock_t endTime = clock();
                double runtime = double(endTime-startTime)/CLOCKS_PER_SEC;
                cout<<"Total Runtime : "<<fixed<<runtime<<setprecision(5)<<endl;
                return 0;
            }
            break;
        }
        case int('R'):{
            //Stock Directory Message. Storing all Normal stocks in hashmap. 
            if(arr[20]==int('N')){
                addStockToMap(arr);
            }
            break;
        }
        case int('A'):{
            //Add Order with Buying– No MPID Attribution Message.
            if(arr[19]==int('B')){
                addOrder(arr);
            }   
            break;
        }
        case int('F'):{
            //Add Order – MPID Attribution Message.
            if(arr[19]==int('B')){
                addOrder(arr);
            }   
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