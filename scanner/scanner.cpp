#include "scanner.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <system_error>
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
	//if(!compararC(c)){actual = udef;}
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
  bool noInteger = false;
  bool isOctal = false;
  bool isHex = false;
  while(actual != udef){
    char c = read();
    prior = actual;
    switch (actual) {
      case 0:
	if(isdigit(c)){

	  if(c == '0'){
	    agregarN(c);
	    noInteger  = true;
	    actual = 1;
	  }else{
	    agregarN(c);
	    actual = 1;
	  }
	}
	else  actual = udef;
	break;

      case 1:
	if(noInteger == false){
	  //no fue un cero,  es un entero o un real.
	  if(isdigit(c)){agregarN(c); actual =1;}
	  else if(c == '.'){agregarN(c); actual = 2;}
	  else if(!compararC(c) && !isspace(c) && c !='#'){ actual = 6;}
	  else{actual = udef;}
	  //aqui abajo fue cero
	}else if(isdigit(c) && !compararC(c) && c !='#'){
	  i = c -48;
	  if(i >= 0 && i <  8){
	    agregarN(c);
	    isOctal = true;
	    actual = 2;
	  }else{
	    actual = 6;
	  }
	}else if(c == 'x' || c == 'X'){
	  agregarN(c);
	  isHex = true;
	  actual = 2;}
	else if(c == '.'){agregarN(c); actual = 2;}
	else if(isspace(c) || compararC(c) || c != '#'){
	  isOctal = true;
	  fallback();
	  actual = 2;
	}
	else actual = 6;

	break;

	//no fue cero por lo que solo puede ser numero real
      case 2:
	//fue un numero distinto de 0 al cual siguio de un punto
	if(noInteger == false){
	  if(isdigit(c)){agregarN(c); actual = 3;}
	  else if(!isspace(c)){actual = 6;}
	  else actual = udef;
	}else{
	  //fue un cero al cual le siguio un numero del 0-7
	  if(isOctal == true){
	    //estado final octal
	    i = c - 48;
	    if(i >= 0 && i <  8){
	      agregarN(c);
	      actual = 2;
	    }else if(!isspace(c) && !compararC(c) && c !='#'){ actual = 6;}
	    else actual = udef;
	  }
	  //fue un cero el cual le siguio un caracter x o X
	  else if (isHex == true) {
	    if (isxdigit(c) != 0){agregarN(c); actual = 4;}
	    else if(!isspace(c)){actual = 6;}
	    else {fallback(); actual = 4;}
	  }
	  //fue un cero el cual le siguio un .
	  else if(isdigit(c)){agregarN(c); actual = 3;}
	  else if(!isspace(c)){actual = 6;}
	  else if(isspace(c)){actual = udef;}
	}
	break;

	//estado final del real:
      case 3:
	if(isdigit(c)){agregarN(c); actual = 3;}
	//por aqui colocar el evaluador;
	else if(!isspace(c) && !compararC(c) && c != '#'){actual = 6;}
	else actual = udef;
	break;

      case 4:
	if (isxdigit(c) != 0){agregarN(c); actual = 5;}
	else if(!isspace(c) && !compararC(c) && c != '#'){actual = 6;}
	else actual = udef;
	break;

	//Estado final hexadecimal
      case 5:
	if (isxdigit(c) != 0){agregarN(c); actual = 4;}
	else if(!isspace(c) && !compararC(c) &&  c != '#'){actual = 6;}
	else actual = udef;
	break;

      case 6:
	actual = udef;
	break;
    }

  }
  if(prior == 1 || prior == 3 || prior == 5 || prior == 2){
    fallback();
    success();
  }
  if (prior == 1){return _int;}
  if (prior == 3)return _real;
  if (prior == 2)return _oct;
  if (prior == 5)return _hex;

  fail();
  return _err;
}


token scanner::opr(){
  char c = read();
  oprts = "";
  //estado de aceptacion
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
    if (islower(c)) {
      palabra +=c;

      actual = 1;
    }else if( isspace(c) || compararC(c) ||  c == '#' ){actual = udef;}
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
    default:
      break;
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


