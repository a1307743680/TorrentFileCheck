#include <cstdio>
#include <clocale>

int main() {
    // setlocale(LC_ALL, "en_US.UTF-8");

    FILE* fp = _wfopen(L"2017战狼2HDTC720P",L"rb");
    printf("%d\n", fp);
    return 0;
}