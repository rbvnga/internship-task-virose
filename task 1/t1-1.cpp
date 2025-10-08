#include <iostream>
using namespace std;

int main(){
    int P, Q, total;
    cin >> P;
    cin >> Q;
    total = P*P + Q*Q + 1;
    if (total % 4 != 0){
        cout << "-1";
    }
    else {
        cout << total/4;
    }
    return 0;
}