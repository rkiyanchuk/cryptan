#include <stdio.h>
#include <stdlib.h>
#include "uea2.h"
#include "uia2.h"
#define SIZE 16
#define FILESIZE 2500
#define enc 0

void readDataFromFile(void* val,int offset,int readSize);
void print_block(const unsigned char * str,const unsigned char* pointer);

int main(int argc, char** argv){
	u8 *key,*data;
	int i,dir,encLen,count;

#if (enc == 1)
	int bearer;

	key = malloc(SIZE);
	encLen = FILESIZE*8;	
	data = malloc(FILESIZE);
	i = 0;
	readDataFromFile(key,i*SIZE,16);
	readDataFromFile(&bearer,i*SIZE+16,4);
	readDataFromFile(&dir,i*SIZE+20,4);
	readDataFromFile(&bearer,i*SIZE+24,4);
	readDataFromFile(&count,i*SIZE+28,4);
	readDataFromFile(data,i*SIZE+32,FILESIZE);
	uea2(key,count,bearer,dir,data,encLen);
	print_block("keystream",data);
#else
	int fresh;
	u8* mac;
	u64 macLen;

	key = malloc(SIZE);
	encLen = FILESIZE*8;	
	data = malloc(FILESIZE);
	i = 0;
	readDataFromFile(&key,i*SIZE+32+FILESIZE,16);
	readDataFromFile(&count,i*SIZE+FILESIZE+48,4);
	readDataFromFile(&dir,i*SIZE+FILESIZE+52,4);
	readDataFromFile(&fresh,i*SIZE+FILESIZE+56,4);
	macLen = encLen;
	mac = uia2(key,count,fresh,dir,data,macLen);
	print_block("mac",mac);
#endif
}	
