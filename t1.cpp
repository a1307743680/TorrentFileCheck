#include <cstdio>
#include <dirent.h>
#include "piece.h"
#include "baseinfo.h"

#include <iostream>
#ifdef WIN32
	#define DIR_SPREATE '\\'
#else
	#define DIR_SPREATE '/'
#endif
using namespace std;

string tpath;
string tfile;

bytes tfdata;
int tflen;
int tfoffset = 0;

int piecelen;
vector<fileinfo*> fileinfos;
vector<piece*> pieces;
bytes piecehashs;
int main (int argc, char ** argv) {

	// for(int i=0; i<argc; i++ )
		// printf("[%2d]:%s\n", i, argv[i]);
	if(argc >=2) {
		char * tm = strrchr(argv[1], DIR_SPREATE);
		tfile = tm+1;
		*tm = 0;
		tpath = argv[1]; 
		// printf("%s\n", tpath.data());
		// printf("%s\n", tfile.data());
		chdir(tpath.data());

	} 
	// printf("%s\n", getcwd(nullptr, 0));
	
	FILE* fp = fopen(tfile.data(), "rb");
	if(!fp) printf("err\n");
	fseek(fp, 0, SEEK_END);
	tflen = ftell(fp);
	// printf("len: %d\n", tflen);
	fseek(fp, 0, SEEK_SET);
	tfdata.resize(tflen);

	fread(&tfdata[0], 1, tflen, fp);
	
	// for(auto it:filedata) {
	// 	printf("%02x", it);
	// }
	// printf("\n\n");
	// for(auto it:filedata) {
	// 	printf("%c", it);
	// }
	// printf("\n%s\n", filedata.data()); 
	Base * root = Base::p();
	Bdic * dinfo = getdic(root, "info");

	Bdic* dfiles = getdic(dinfo->val, "files");
	if(!dfiles) {
		Bdic* dlength = getdic(dinfo->val, "length");
		Bint* length = dynamic_cast<Bint*>(dlength->val);
		int lengthint = length->val;
		Bdic* dname = getdic(dinfo->val, "name.utf-8");
		if(!dname) dname = getdic(dinfo->val, "name");
		Bstr* sname = dynamic_cast<Bstr*>(dname->val);
		string namestr = sname->str();
		
		fileinfo* finf = new fileinfo;
		finf->path = namestr;
		finf->len = lengthint;
		fileinfos.push_back(finf);
		// FILE* fp = tfopen(".\\"+namestr);
		// cout<<"fp "<<fp<<endl;

	}
	else {
		Blis* lfiles = dynamic_cast<Blis*>(dfiles->val);
		for(; lfiles!= nullptr; lfiles = lfiles->next) {
			Bdic *dfile = dynamic_cast<Bdic*>(lfiles->val);
			Bdic* dlength = getdic(dfile, "length");
			Bint* length = dynamic_cast<Bint*>(dlength->val);
			int lengthint = length->val;
			// std::cout<<"length__ "<<lengthint<<endl;
			Bdic* dpath = getdic(dfile, "path.utf-8");
			if(!dpath) dpath = getdic(dfile, "path");
			Blis* lpath = dynamic_cast<Blis*>(dpath->val);
			string filepath = ".";
			for(; lpath != nullptr; lpath = lpath->next) {
				Bstr* ps = dynamic_cast<Bstr*>(lpath->val);
				filepath += DIR_SPREATE + ps->str();
				// cout<<"path____ "<<filepath<<endl;
			}
			fileinfo* finf = new fileinfo;
			finf->path = filepath;
			finf->len = lengthint;
			fileinfos.push_back(finf);
		}
	}
	Bdic* dpiecelen = getdic(dinfo->val, "piece length");
	Bint* ipiecelen = dynamic_cast<Bint*>(dpiecelen->val);
	piecelen = ipiecelen->val;
	// std::cout<<"piece length "<<piecelen<<endl;
	Bdic* dpieces = getdic(dinfo->val, "pieces");
	Bstr* spieces = dynamic_cast<Bstr*>(dpieces->val);
	piecehashs = spieces->bdata();
	// std::cout<<"piece hash "<<piecehashs.size()<<endl;


	/// 
	auto fit = fileinfos.begin();
	int fileoffset = 0;
	for(int i =0; i<piecehashs.size(); i+=20) {
		piece* p = new piece;
		memcpy(p->hashcode.data(), piecehashs.data()+i, 20);
		int plen = piecelen ;
		while (plen!=0) {
			auto fptr = (*fit);
			if(fileoffset >= fptr->len) {
				
			}
			int remainlen = fptr->len - fileoffset;
			if(remainlen > plen) {
				auto fpptr = new filepiece;
				fpptr->len = plen;
				fpptr->offset = fileoffset;
				fpptr->file = fptr;
				
				fileoffset += plen;
				plen = 0;

			}
		}
	}


	return 0;

}