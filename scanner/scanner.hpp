#ifndef scanner_hpp 
#define scanner_hpp
#include <fstream>

using namespace std;


typedef enum etoken {_err, _eof, _id, _int, _real ,_hex, _oct, _del, _punt, _oprt, _coment, _print, _true, _false}token;

class scanner{
  
  void fail(){
    file.seekg(pos,file.beg); 
  }

  void fallback(){ 
    file.seekg(-1, file.cur); 
  }

  char read(){ 
    return file.get(); 
  } 
 
  void success(){
    pos = file.tellg();
  }
 

  ifstream file;
  streampos pos;

static const int udef;
  token reserved(); 
  token reservadas();
  token opr();
  bool coment(); 
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
