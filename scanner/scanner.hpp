#ifndef scanner_hpp 
#define scanner_hpp
#include <fstream>

using namespace std;


//typedef enum etoken {_err, _eof, _id, }token;
typedef enum etoken {_err, _eof, _id, _int, _real ,_hex, _oct}token;

class scanner{
  
  char read(){ 
    return file.get(); 
  } 
  void fallback(){ 
    file.seekg(-1, file.cur); 
  }
  void success(){
    pos = file.tellg();
  }
  void fail(){
    file.seekg(pos,file.beg); 
  }

  ifstream file;
  streampos pos;

static const int udef;

  bool id();
  token num();
  bool hex();
  bool oct();
  bool wsp();
  bool eof();

  public:
  scanner(string fname): file(fname), pos(0){}
  ~scanner() { file.close(); }

  token next();

};


#endif
