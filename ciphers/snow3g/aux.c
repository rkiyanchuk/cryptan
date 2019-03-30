#include <stdio.h>


void print_block_len(const char * str,const char* pointer, int length)
{
	int p;

	printf("%s--> 128'h",str);
	for (p = 0; p<length; p++)
	{
		if(p%4 == 0 && p != 0) printf("_");
		printf("%.2x",*(pointer+p));
	}
	printf("\taddress: %p\n",pointer);
}

void print_block(const char * str,const char* pointer)
{
	print_block_len(str,pointer,16);
}


void readDataFromFile(void* val,int offset,int readSize)
{
	FILE *data_fp;
	int bytes_read;

	data_fp = fopen("halo3.jpg","rb");
	if(data_fp == NULL )
	{
		printf("file does not exist\n");
		exit(0);
	}
	fseek(data_fp,offset,SEEK_SET);
	bytes_read = fread(val,1,readSize,data_fp);
	if(bytes_read != readSize)
	{
		printf("bytes_read != size\n");	
		printf("bytes read: %d\n",bytes_read);
	}
}
