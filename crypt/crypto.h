#ifndef crypto
#define crypto

#include <iostream>
#include <fstream>
//#include <bitset> usado en la implementacion anterior de conversion a binario


int pow(int ,int );//potencia de un entero
void charToBinary(char ,char * ,int );//conversion de un caracter a binario
char notbin(char );//negacion de un caracter binario
char binaryToChar(int,char *);
void printstr(char * ,int );
void encrypt_cfg(int &method, int &seed);
void textToBinary (std::fstream * ifs , std::fstream * ofsb);
void encrypt(std::fstream * ofsb, std::fstream * ofsc,int seed ,int method );
void decrypt(std::fstream * ifs, std::fstream * mfs, int seed , int method );
void binaryToText(std::fstream * mfs, std::fstream * text);
void viewFile(std::fstream * ifs);
void compare_fixed(std::fstream * ifs1,std::fstream * ifs2,int word);
void compare_word(std::fstream * ifs1,std::fstream * ifs2, int max_word);
void countWordsBiggerThan(std::fstream * ifs, int big, int max_word, int & totalCount, int & bigCount, int &  smallCount);
int str_size(char * str);
//std::bitset<8> binary(char ) implementacion vieja de la conversion a binario

#endif
