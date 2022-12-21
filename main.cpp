#include <iostream>
#include <fstream>

using namespace std;

int main(){
    clock_t start = clock();
    // cout<<"Hi Abhishek"<<endl;
    ifstream is("/Users/abhishek/Downloads/01302019.NASDAQ_ITCH50");

    string line;
    while (std::getline(is, line)){
        cout<<line<<endl;
    }
    // char buff[4];
    // do {
    //     is.read(buff,sizeof(buff));
        
    //     //Process the message
    //     cout<<buff<<endl;
    // }while(is);

    clock_t end = clock();
    double runtime = double(end-start)/CLOCKS_PER_SEC;
    cout<<"Total Runtime : "<<fixed<<runtime<<setprecision(5)<<endl;

    return 0;
}