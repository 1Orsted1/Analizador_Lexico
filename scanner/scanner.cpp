#include "scanner.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <system_error>

const int scanner::udef = -1;
string palabra = "";
string numero = "";
int renglon = 0;


bool scanner::id(){
int actual = 0,prior = udef ;
while(actual != udef){
  char c = read();
  palabra += c;
  prior = actual;
  switch (actual) {
    case 0:
      if(isalpha(c)) actual = 2;
      else if(c == '_') actual = 1; 
      else actual = udef;
      break;
    case 1:
      if(isdigit(c) || c =='_')actual=1;
      else if(isalpha(c)) actual = 2;
      //else if(c == '('){fallback(); actual=2;}
      else actual = udef; 
      break;
    case 2:
      if(isalnum(c) || c == '_') actual = 2;

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
  numero += c;
  prior = actual;
 switch (actual) {
    
   case 0:
	if(isdigit(c))
	  if(c == '0'){
	    noInteger  = true;
	    actual = 1;
	  }else{
	  actual = 1;
	}
	else  actual = udef;
    break;

    case 1:
	if(noInteger == false){
	  //no fue un cero,  es un entero o un real.
	  if(isdigit(c)) actual =1;
	  else if(c == '.') actual = 2;
	  else if(!isspace(c)){actual = 6;}
	  else{actual = udef;}
	}else if(isdigit(c)){
	      i = c -48;
	      if(i >= 0 && i <  8){
		isOctal = true;
		actual = 2;
	      }else{
	       actual = 6;
	      }
	  }else if(c == 'x' || c == 'X'){isHex = true; actual = 2;}
	  else if(c == '.'){actual = 2;}
	  else if(isspace(c)){
	    fallback();
	    actual = 2;
	  }
	  else actual = 6;
		
     break;

   //no fue cero por lo que solo puede ser numero real 
    case 2:
	//fue un numero distinto de 0 al cual siguio de un punto
	if(noInteger == false){
	  if(isdigit(c)) actual = 3;
	  else if(!isspace(c)){actual = 6;}
	  else actual = udef;
	}else{
	  //fue un cero al cual le siguio un numero del 0-7
	  if(isOctal == true){
	    //estado final octal
	      i = c - 48;

	      if(i >= 0 && i <  8){actual = 2;} 
	      else if(!isspace(c)){ actual = 6;}

	      else actual = udef;
	  }
	  //fue un cero el cual le siguio un caracter x o X
	  else if (isHex == true) {
	     if (isxdigit(c) != 0) actual = 4;
	     else if(!isspace(c)){actual = 6;}
	     else {fallback(); actual = 4;}
	  }
	  //fue un cero el cual le siguio un .
	  else if(isdigit(c)){actual = 3;}
	  else if(!isspace(c)){actual = 6;}
	  else if(isspace(c)){actual = udef;}

	}
     break;

     //estado final del real:
     case 3:
	 if(isdigit(c)) actual = 3;
	 else if(!isspace(c)){actual = 6;}
	 else actual = udef;
     break;

     case 4:
	 if (isxdigit(c) != 0) actual = 5;
	 else if(!isspace(c)){actual = 6;}
	 else actual = udef;
     break;
     
     //Estado final hexadecimal
     case 5:
	 if (isxdigit(c) != 0) actual = 4;
	 else if(!isspace(c)){actual = 6;}
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
  if(id())return _id;
  
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


