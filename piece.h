#ifndef BBB
#define BBB
#include <vector>
#include <array>
#include <string>
#include <cwchar>
#include <cstdio>
#include <windows.h>

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

FILE* tfopen(string fpath) {
    wchar_t filepath[512];
    int sflag = MultiByteToWideChar(CP_UTF8,MB_USEGLYPHCHARS,fpath.data(), fpath.length()+1, filepath, 512);
    // MultiByteToWideChar(CP_UTF8, 0, filePath, strlen(filePath)+1, fileRecordList[currentFile].wFilePath,strlen(filePath)+1);
    // std::mbstate_t state = std::mbstate_t(); // initial state
    // int sflag = mbrtowc(filepath, file, fpath.length()+1, &state);
    
    FILE* fp = _wfopen(filepath,L"rb");
    return fp;
    // return nullptr;
}

#endif