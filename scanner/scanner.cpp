#include "scanner.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#define agregarC(char) palabra += c;
#define agregarN(char) numero +=c;

const int scanner::udef = -1;
string palabra = "";
string oprts = "";
string numero = "";
int renglon = 0;


bool compararC(char c){
  switch (c) {
    case '(':return true;break;
    case ')':return true;break;
    case '[':return true;break;
    case ']':return true;break;
    case ',':return true;break;
    case '.':return true;break;
    case ';':return true;break;
    case ':':return true;break;
    case '&':return true;break;
    case '|':return true;break;
    case '!':return true;break;
    case '-':return true;break;
  }
  return false;
}

bool scanner::id(){
  int actual = 0,prior = udef ;
  while(actual != udef){
    char c = read();
    prior = actual;
    switch (actual) {
      case 0:
	if(isalpha(c)){agregarC(c); actual = 2;}
	else if(c == '_'){agregarC(c); actual = 1; }
	else actual = udef;
	break;
      case 1:
	if(isdigit(c) || c =='_'){ agregarC(c); actual=1;}
	else if(isalpha(c)){agregarC(c); actual = 2;}
	else actual = udef;
	break;
      case 2:
	if(isalnum(c) || c == '_'){agregarC(c); actual = 2;}
	else actual = udef;
    }
  }

  if(prior == 2){
    fallback();
    success();
    return true;
  }
  fail();
  return false;
}

token scanner::num(){

  int actual = 0,prior = udef, i;
  while(actual != udef){
    char c = read();
    prior = actual;
    switch (actual) {
      case 0:
	if(isdigit(c))
	{
	  if(c == '0'){ agregarN(c); actual = 4;}
	  else{ agregarN(c); actual = 1; }
	}
	else  actual = udef;
	break;
      case 1:
	if(isdigit(c)){agregarN(c); actual =1;}
	else if(c == '.'){agregarN(c); actual = 3;}
	else if(!compararC(c) && !isspace(c) && c !='#'){ actual = 9;}
	else{actual = udef;}
	break;
      case 2:
	if(isdigit(c)){agregarN(c); actual = 3;}
	else if(!isspace(c)){actual = 6;}
	else actual = udef;
	break;
      case 3:
	if(isdigit(c)){agregarN(c); actual = 3;}
	else if(!isspace(c) && !compararC(c) && c != '#'){actual = 9;}
	else actual = udef;
	break;
      case 4:
	if(isdigit(c) && !compararC(c) && c !='#')
	{
	  i = c -48;
	  if(i >= 0 && i <  8){agregarN(c);actual = 8;}
	  else{actual = 9;}
	}
	else if(c == 'x' || c == 'X'){agregarN(c);actual = 5;}
	else if(c == '.'){agregarN(c); actual = 3;}
	else if( compararC(c) || isspace(c) || c =='#' ){fallback(); actual = 8;}
	else actual = 9;
	break;
      case 5:
	if (isxdigit(c)!=0){agregarN(c); actual = 6;}
	else if(!isspace(c)){actual = 9;}
	else {fallback(); actual = 6;}
	break;
      case 6:
	if (isxdigit(c) != 0){agregarN(c); actual = 7;}
	else if(!isspace(c) && !compararC(c) && c != '#'){actual = 6;}
	else actual = udef;
	break;
      case 7:

	if (isxdigit(c) != 0){agregarN(c); actual = 6;}
	else if(!isspace(c) && !compararC(c) &&  c != '#'){actual = 6;}
	else actual = udef;

	break;
      case 8:
	i = c - 48;
	if(i >= 0 && i <  8){agregarN(c);actual = 8;}
	else if(!isspace(c) && !compararC(c) && c !='#'){ actual = 9;}
	else actual = udef;
	break;
      case 9:
	actual = udef;
	break;
    }}
  if(prior == 1 || prior == 3 || prior == 8 || prior == 7){
    fallback();
    success();
  }
  if (prior == 1){return _int;}
  if (prior == 3)return _real;
  if (prior == 8)return _oct;
  if (prior == 7)return _hex;

  fail();
  return _err;
}

token scanner::opr(){
  char c = read();
  oprts = "";
  if(compararC(c)){
    if(c == '[' || c == ']' || c == '(' || c == ')'){oprts = c;return _del;}
    else if(c == '.' || c == ',' || c == ';'){oprts = c;return _punt;}
    else if( c == ':' || c == '&' || c == '|' || c == '!'){oprts = c;return _oprt;}
    else if(c == '-'){
      oprts += c;
      c = read();
      if(c == '>'){oprts += c;return _oprt;}
      else{fallback();}
    }
  }
  fallback();
  return _err;
}

token scanner::reserved() {
  map<string, token> ::iterator i;
  map<string, token> palabrasReservasdas;

  palabrasReservasdas["print"] = _print;
  palabrasReservasdas["true"] = _true;
  palabrasReservasdas["false"] = _false;
  int actual = 0, prior = -1;
  string palabra = "";

  while (actual != udef) {
    char c = read();
    prior = actual;
    if (islower(c)) {palabra +=c; actual = 1;}
    else if( isspace(c) || compararC(c) ||  c == '#' ){actual = udef;}
    else{fail();return _err;
    }
  };
  if (prior == 1) {
    for (i = palabrasReservasdas.begin(); i != palabrasReservasdas.end(); ++i) {
      if (palabra==i->first ) {
	fallback();
	success();
	return i->second  ;
      }

    }
  }
  fail();
  return _err;
}

bool scanner::coment(){
  char c = read();
  if(c == '#'){
    while(c != '\n'){c = read();}
    renglon +=1;
    return true;
  }
  fallback();
  return false;
}


bool scanner::wsp(){
  char c = read();
  while (isspace(c)){
    if(c == '\n'){
      renglon++;
    }
    c = read();
  }
  fallback();
  success();
  numero = "";
  palabra = "";
  return true;
}


bool scanner::eof(){
  char c = read();
  c = read();
  if(c == EOF){
    return !file.eof();
  }
  return file.eof();
}

token scanner::next(){

  wsp();

  if(coment()){return _coment;}

  switch (reserved()) {
    case _print:return _print;break;
    case _true:return _true;break;
    case _false:return _false;break;
    default:break;
  }

  if(id())return _id;

  switch (opr()) {
    case _del:return _del;break;
    case _punt:return _punt;break;
    case _oprt:return _oprt;break;
    default:break;
  }

  switch (num()) {
    case _int:return _int;
    case _real: return _real;
    case _oct: return _oct;
    case _hex: return _hex;
    default: break;
  }

  if(eof())return _eof;
  
  return _err;
}
