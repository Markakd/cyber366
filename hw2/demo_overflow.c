#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char data[0x100];

void bar(){
	system("echo 'hello'");	
}

int foo(char str2[]){
	char array[4];
	strcpy(array, str2);
	return 1;
}

int main(int argc, char* argv[]){
	if (argc != 2)
		exit(0);
	strcpy(data, argv[1]);
	foo(argv[1]);
}
