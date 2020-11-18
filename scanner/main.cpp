#include <cstdio>
#include <iostream>
#include <iterator>
#include <string>
#include "scanner.cpp"
#include "scanner.hpp"

extern string numero;
extern string oprts;
extern string palabra;
extern int renglon;

int main(int argc, const char * arvg[]){
  string archivo;
  cout << "Escriba el nombre del archivo porfavor\n=>";
  cin >> archivo;
  scanner s(archivo+".txt");
  token t;

  do{
    t =s.next();
    switch (t) {
      case _id: cout<<"->identificador [ "+palabra+" ] \n";break;
      case _coment: printf("->La linea %i tiene un comentario\n",renglon);break;
      case _int:cout<<"->integer [ "+ numero +" ]\n";break;
      case _real:cout<<"->real [ "+numero+" ]\n";break;
      case _print:cout<<"->palabra reservada print\n";break;
      case _true:cout<<"->palabra reservada true\n";break;
      case _false:cout<<"->palabra reservada false\n";break;
      case _oprt:cout<<"->operador [ "+oprts+" ]\n";break;
      case _punt:cout<<"->puntuacion [ "+oprts+" ]\n";break;
      case _del:cout<<"->delimitador [ "+oprts+" ]\n";break;
      case _oct:cout<<"->octal [ "+numero+" ] \n";break;
      case _hex:cout<<"->hexadecimal [ "+numero+" ] \n";break;
      case _eof: printf("=>Fin del analisis, lineas leidas: %i \n",renglon);break;
      case _err: printf("=>Error en la linea %i \n",renglon+1); break;
      default:printf("bien echo encontraste algo que no debia de pasar \n");
    }

  }while (t != _eof && t != _err);

  return 0;

}
