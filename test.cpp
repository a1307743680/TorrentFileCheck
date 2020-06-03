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


    return 0;
}