#include <cstdio>
#include <iostream>
#include <iterator>
#include <string>
#include "scanner.cpp"
#include "scanner.hpp"

int main(int argc, const char * arvg[]){
string archivo;
cout << "Escriba el nombre del archivo porfavor\n->";
cin >> archivo;
scanner s(archivo+".txt");
token t;

do{
  t =s.next();

switch (t) {

  case _id: cout<<"->Identificador "+palabra+"\n";break;	 
  case _int:cout<<"->integer "+numero+"\n";break;
  case _real:cout<<"->real "+numero+"\n";break;
  case _oct:cout<<"->octal "+numero+"\n";break;
  case _hex:cout<<"->hexadecimal "+numero+"\n";break;
  case _eof: printf("->Fin del analisis, lineas leidas: %i \n",renglon);break;
  case _err: printf("->Error en la linea %i \n",renglon+1); break;
  
  default:printf("bien echo encontraste algo que no debia de pasar \n");
}

}while (t != _eof && t != _err);

return 0;

}
