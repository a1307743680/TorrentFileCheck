#ifndef AAA
#define AAA

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

typedef vector<unsigned char> bytes;
extern vector<unsigned char> tfdata;
extern int tflen;
extern int tfoffset;

class Base {
public:
    enum Type {
        ROOT,
        INT,
        STR,
        LIS,
        DIC
    };
public:
    Type t;
public:
    static bool end() {
        if(tfoffset>tflen) return true;
        else if(tfdata.at(tfoffset)=='e') {
            ++tfoffset;
            return true;
        }
        return false;
    }
    static Base* p();
public:
    virtual Type type() {
        return t;
    }
    // void pharse() {
    //     this->next = p();
    // }
};

class Bint :public Base
{
    public:
    Type t = INT;
    int val ;
    public:
    static int getnum() {
        int res = 0;
        bool pon = true;
        for(;tfoffset<=tflen;) {
            char c = tfdata[tfoffset];
            switch (c) {
                case 'e':
                case ':':
                    ++tfoffset;
                    return  pon?res:-res;
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    res = res* 10 + c-'0';
                    ++tfoffset;
                    break;
                case '-': 
                    pon = false;
                    ++tfoffset;
                    break;
                default:
                    return -1;
            }
        }
        return -1;
    }
    static Bint* p() {
        if(end()) return nullptr;
        Bint * res = new Bint;
        res->val = getnum();
        return res;
    }
};

class Bstr: public Base
{
public:
    Type t = STR;
    int pos;
    int len;
public:
    static Bstr* p() {
        if(end()) return nullptr;
        Bstr* res = new Bstr;
        res->len = Bint::getnum();
        res->pos = tfoffset;
        tfoffset += res->len;
        return res;
    }
public:
    string str() {
        return string( (char*) &tfdata[pos], len);
    }
    bytes bdata() {
        bytes res(len);
        memcpy(&res[0], &tfdata[pos],len);
        return res;
    }
};

class Blis: public Base
{
    public:
    Type t = LIS;
    Base * val;
    Blis * next;

    public:
    static Blis* p() {
        if(end()) return nullptr;
        Blis* res = new Blis;
        res->val = Base::p();
        res->next = Blis::p();
        
        return res;         
    }
};

class Bdic: public Base
{
    public:
    Type t = DIC;
    Bstr * key;
    Base * val; 
    Bdic * next;
   
    public:
    static Bdic* p() {
        if(end()) return nullptr;
        Bdic* res = new Bdic;
        res->key = Bstr::p();
        res->val = Base::p();
        res->next = Bdic::p();
        return res;        
    }
};

Base* Base::p() {
    if(end()) return nullptr;
    char c = tfdata[tfoffset];
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return Bstr::p();
        case 'i': 
            ++tfoffset;
            return Bint::p();
        case 'l': 
            ++tfoffset;
            return Blis::p();
        case 'd': 
            ++tfoffset;
            return Bdic::p();
        default:
            return nullptr;
    }

    return nullptr;
}
Bdic* getdic(Base * dnode, string key) {
    Bdic * dic = dynamic_cast<Bdic*> (dnode);
    if(dic == nullptr) return nullptr;
    // std::cout<<" key "<<dic->key->str() <<endl;
    if(dic->key->str() == key) {
        return dic;
    }
    else return getdic(dic->next, key);
    return nullptr;
}

#endif