#include <iostream>

using namespace std;

uint getTrackingNumber(unsigned char ch0,unsigned char ch1){
    uint num = 0;
    num = uint(ch0)<<8 | uint(ch1);
    return num;
}

int main(){
    unsigned char ch0 = uint(1);
    unsigned char ch1 = uint(10);

    cout<<"Tracking num : "<<getTrackingNumber(ch0,ch1)<<endl;
}