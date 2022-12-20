#include <iostream>
#include <fstream>

using namespace std;

int main(){
    // cout<<"Hi Abhishek"<<endl;
    ifstream is("file.txt");
    char buff[1];
    do {
        is.read(buff,sizeof(buff));
        //Process the message
        cout<<buff<<endl;
    }while(is);
    return 0;
}