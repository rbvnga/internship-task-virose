#include <iostream>
#include <string.h>
using namespace std;

int main(){
    string S;
    cin >> S;
    int x;
    x = S.length();
    for(int i = 0; i< x; i++ ){
        if(S[i] >= 'A' && S[i]<='Z'){
            S[i] += 32;
        }
        else{
            S[i] -=32;
        }
    }
    cout << S;
    
    return 0;
}