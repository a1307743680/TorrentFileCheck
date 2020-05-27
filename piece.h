#ifndef BBB
#define BBB
#include <vector>
#include <array>
#include <string>
#include <cwchar>

using namespace std;
typedef std::array<unsigned char,20> sha1hash;

class fileinfo {
public:
    int len;
    string path;
    bool broken = false;
};

class filepiece {
public:
    int fileidx;
    int filepieceidx;
    int len;
    int offset;
    fileinfo* file;
};

class piece {
public:
    sha1hash hashcode;
    vector<filepiece* > files;
};



#endif