#include "Header.h"

using namespace std;

int main(int argc,char *argv[]){

    if(argc<2){
        cerr<<"Error : Too few arguments. Please Enter both input filename and output directory";
        exit(1);
    }
    string filename = argv[1];
    cout<<"Opening : "<<filename<<endl;

    /*Using the Clock to monitor the permonace of the program*/
    clock_t startTime = clock();

    /*Opening the input stream of the ITCH file*/
    // For reference : /Users/abhishek/Downloads/01302019.NASDAQ_ITCH50
    ifstream inputStream(filename,ios::in | ios::binary);
    
    cout<<"Starting..."<<endl;
    /*Reading the Input Stream*/
    while(inputStream){

        uint16_t msgSize = getMessageSize(inputStream);

        unsigned char arr[msgSize];
        inputStream.read((char *)arr,sizeof(arr));
        
        writeToFile(arr);
        
        /*Processing the messages based on their msg type.*/
        switch (arr[0])
        {
        case int('S'):{
            if(arr[11]==int('C')){
                writeHourlyVWAP();
                clock_t endTime = clock();
                double runtime = double(endTime-startTime)/CLOCKS_PER_SEC;
                cout<<"Total Runtime : "<<fixed<<runtime<<setprecision(5)<<endl;
                return 0;
            }
            break;
        }
        case int('R'):{
            if(arr[20]==int('N')){
                addStock(arr);
            }
            break;
        }
        case int('A'):{
            if(arr[19]==int('B')){
                addOrder(arr);
            }   
            break;
        }
        case int('F'):{
            if(arr[19]==int('B')){
                addOrder(arr);
            }   
            break;
        }
        case int('E'):{
            executeOrder(arr);
            break;
        }
        case int('C'):{
            executeOrderWithPrice(arr);
            break;
        }
        case int('X'):{
            orderCancel(arr);
            break;
        }
        case int('D'):{
            orderCancel(arr);
            break;
        }
        case int('U'):{
            orderReplace(arr);
            break;
        }
        case int('P'):{
            if(arr[19]==int('B')){
                executeTradeMessage(arr);
            }
            break;
        }
        case int('Q'):{
            crossTrade(arr);
            break;
        }
        case int('B'):{
            removeBrokenTrade(arr);
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