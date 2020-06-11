#include <cstdio>
#include <iostream>
#include <clocale>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex mutxCre;
mutex mutxCal;

condition_variable conCre;
condition_variable conCal;

void create() {
    while(true) {

    }
}
void calculate() {

}
int main() {
    // setlocale(LC_ALL, "en_US.UTF-8");
    int a =1<<3;
    char c[a];
    char* b= (char*)&a;
    cout<< (a<<3)<<endl;
    cout<< (int)b[0]<<endl;
    cout<< (int)b[1]<<endl;
    std::cout<<sizeof(long)<<endl;
    return 0;
}