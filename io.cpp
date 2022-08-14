#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;
int main(){
    string lines;
    string line1;
    ifstream file_("test.txt");
    if(file_.is_open()){
        while(getline(file_,line1)){
            lines += line1;
            //cout << line1 << endl;
        }
        cout << lines;
        file_.close();
    }
    else{
        cout << "File is not open";
    }
    return 0;
}